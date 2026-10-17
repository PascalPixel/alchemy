/*
 * track.c
 *
 * Derive note lengths, pattern timing, and rests from MIDI tracks.
 * Pattern brackets remove expanded playback time from the output timeline.
 * Keep original ticks for note lengths and bar boundaries, and retain source
 * order when several events share a tick.
 */
#include "sequence.h"
typedef struct
{
	SequenceEvent event;
	int64_t tick, raw;
	size_t order;
	bool grid, pending;
} Node;

/*
 * Registered-parameter selection is stateful. Ignore the initial bend-range
 * preamble, but retain subsequent changes as engine controls.
 */
static SequenceEvent
midiControl(const MidiEvent *e, int parameter[2], bool preamble)
{
	const uint8_t *v = e->data.data;
	unsigned status = e->status & 240;
	if (status == 192)
		return sequenceControl("voice", v[0]);
	if (status == 224)
		return sequenceControl("pitch_bend", v[1]);
	if (status != 176)
		return (SequenceEvent){0};
	switch (v[0])
	{
	case 7:
		return sequenceControl("volume", v[1]);
	case 10:
		return sequenceControl("pan", v[1]);
	case 1:
		return sequenceControl("modulation_depth", v[1]);
	case 101:
		parameter[0] = v[1];
		break;
	case 100:
		parameter[1] = v[1];
		break;
	case 6:
		if (parameter[0] == 0 && parameter[1] == 0)
		{
			if (!preamble)
				return sequenceControl("pitch_bend_range", v[1]);
		}
		else if (parameter[0] == 0 && parameter[1] == 1)
			return sequenceControl("tuning", v[1]);
		else
			fail("MIDI data entry names no registered parameter");
		break;
	case 0:
	case 38:
	case 91:
		break;
	default:
		fail("MIDI controller %u is not a sequence command", v[0]);
	}
	return (SequenceEvent){0};
}

static int
compareNodes(const void *left, const void *right)
{
	const Node *a = left, *b = right;
	if (a->tick != b->tick)
		return a->tick < b->tick ? -1 : 1;
	return a->order < b->order ? -1 : a->order > b->order;
}

/*
 * Split rests at meter changes and bar boundaries, then use the longest
 * representable duration for each piece.
 */
static void
waits(Assembly *a, SequenceState *state, int64_t start, int64_t length,
	  const TimeSignature *meter, size_t count)
{
	if (start < 0 || length < 0)
		fail("MIDI rest starts before its stream");
	int64_t end = start + length;
	while (start < end)
	{
		size_t i = 0;
		while (i + 1 < count && meter[i + 1].tick <= start)
			i++;
		int64_t stop =
			meter[i].tick +
			((start - meter[i].tick) / meter[i].length + 1) * meter[i].length;
		if (i + 1 < count && stop > meter[i + 1].tick)
			stop = meter[i + 1].tick;
		if (stop > end)
			stop = end;
		int64_t gap = stop - start;
		while (gap)
		{
			unsigned index = 48;
			while (durations[index] > gap)
				index--;
			SequenceEvent e = sequenceControl("wait", durations[index]);
			emitSequenceEvent(a, &e, state);
			freeSequenceEvent(&e);
			gap -= durations[index];
		}
		start = stop;
	}
}

/*
 * Collect and pair notes before sorting: note-off supplies duration in the
 * original timeline, while pattern brackets adjust only output timing.
 */
void
encodeTrack(Assembly *a, const Track *track, const TimeSignature *meter,
			size_t meterCount)
{
	Node *nodes = NULL;
	size_t count = 0;
	int depth = 0, parameter[2] = {-1, -1};
	int64_t bracket = 0, removed = 0;
	bool preamble = true;
	for (size_t i = 0; i < track->count; i++)
	{
		const MidiEvent *e = track->events + i;
		SequenceEvent derived = {0};
		const char *text = (char *) e->data.data;
		unsigned status = e->status & 240;
		if (e->status == 255 && e->meta == 0x51)
		{
			if (e->data.len != 3)
				fail("MIDI tempo needs three bytes");
			uint64_t micros = (unsigned) e->data.data[0] << 16 |
							  (unsigned) e->data.data[1] << 8 | e->data.data[2];
			if (!micros)
				fail("zero MIDI tempo");
			uint64_t numerator = UINT64_C(7200000000) * 280896,
					 denominator = UINT64_C(16777216) * micros;
			derived = sequenceControl(
				"tempo",
				(int64_t) ((2 * numerator + denominator) / (2 * denominator)));
		}
		else if (e->status < 240 && status != 128 && status != 144)
			derived = midiControl(e, parameter, preamble);
		if (e->status == 255 || status == 144)
			preamble = false;
		if (derived.kind)
		{
			if (!depth)
			{
				nodes = resize(nodes, count + 1, sizeof(Node));
				nodes[count++] = (Node){
					derived, e->tick - removed, e->tick, e->order, true, false};
			}
			else
				freeSequenceEvent(&derived);
			continue;
		}
		if (e->status == 255 && e->meta == 7)
		{
			if (!strcmp(text, "pattern+"))
			{
				if (!depth)
					bracket = e->tick;
				depth++;
			}
			else if (!strcmp(text, "pattern-"))
			{
				if (--depth < 0)
					fail("MIDI pattern bracket underflow");
				if (!depth)
					removed += e->tick - bracket;
			}
		}
		else if (e->status == 255 && e->meta == 6)
		{
			SequenceEvent marked = parseSequenceEvent(text);
			const char *controls[] = {"tempo",
									  "voice",
									  "volume",
									  "pan",
									  "modulation_depth",
									  "pitch_bend",
									  "pitch_bend_range",
									  "tuning"};
			for (size_t c = 0; c < 8; c++)
				if (!strcmp(marked.kind, controls[c]))
					fail("MIDI marker duplicates a channel control");
			if (depth)
			{
				freeSequenceEvent(&marked);
				continue;
			}
			nodes = resize(nodes, count + 1, sizeof(Node));
			nodes[count++] = (Node){
				marked, e->tick - removed, e->tick, e->order, true, false};
		}
		else if (e->status < 240 && (status == 144 || status == 128))
		{
			unsigned key = e->data.data[0], velocity = e->data.data[1];
			if (status == 144 && velocity)
			{
				SequenceEvent note = sequenceControl("note", 0);
				addArgument(&note, key);
				addArgument(&note, velocity);
				nodes = resize(nodes, count + 1, sizeof(Node));
				nodes[count++] = (Node){note,	  e->tick - removed, e->tick,
										e->order, depth == 0,		 true};
			}
			else
			{
				size_t n = 0;
				while (n < count && (!nodes[n].pending ||
									 eventArgument(&nodes[n].event, 1) != key))
					n++;
				if (n == count)
					fail("MIDI note-off has no note-on");
				free(nodes[n].event.args.items[0]);
				nodes[n].event.args.items[0] =
					formatString("%lld", (long long) (e->tick - nodes[n].raw));
				nodes[n].pending = false;
			}
		}
	}
	if (depth)
		fail("MIDI pattern bracket is unclosed");
	for (size_t i = 0; i < count; i++)
		if (nodes[i].pending)
			fail("MIDI has an unclosed note-on");
	if (count > 1)
		qsort(nodes, count, sizeof(Node), compareNodes);
	SequenceState state = {-1, -1, -1, -1, false};
	int64_t cursor = 0;
	for (size_t i = 0; i < count; i++)
	{
		Node *n = nodes + i;
		if (n->grid)
		{
			if (n->tick < cursor)
				fail("MIDI event order moves backwards");
			int64_t gap = n->tick - cursor;
			waits(a, &state, n->raw - gap, gap, meter, meterCount);
			emitSequenceEvent(a, &n->event, &state);
			cursor = n->tick;
		}
		freeSequenceEvent(&n->event);
	}
	free(nodes);
}
