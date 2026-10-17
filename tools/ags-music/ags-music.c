/*
 * ags-music.c
 *
 * Assemble the conductor layout and its timed music streams.
 * The conductor describes stream labels, alignment, and song headers. MIDI
 * tracks supply their events; sequence.c owns sound-command encoding. Validate
 * all pointer targets before publishing the completed assembly.
 */
#include "sequence.h"

static const char *
field(const Strings *fields, const char *key)
{
	size_t n = strlen(key);
	const char *value = NULL;
	for (size_t i = 2; i < fields->len; i++)
		if (!strncmp(fields->items[i], key, n) && fields->items[i][n] == '=')
		{
			if (value)
				fail("repeated sequence header field: %s", key);
			value = fields->items[i] + n + 1;
		}
	if (!value)
		fail("missing sequence header field: %s", key);
	return value;
}

/*
 * Use the first text event as the layout and retain ordered meter changes.
 * Insert common time at tick zero when the MIDI omits an initial signature.
 */
static const char *
readConductor(const Midi *midi, TimeSignature **signatures, size_t *count)
{
	const char *skeleton = NULL;
	TimeSignature *meter = NULL;
	size_t meterCount = 0;
	for (size_t i = 0; i < midi->tracks[0].count; i++)
	{
		MidiEvent *e = midi->tracks[0].events + i;
		if (e->status != 255)
			continue;
		if (e->meta == 0x7f && e->data.len >= 15 &&
			!memcmp(e->data.data, "alchemy-mid2agb\0", 15))
			fail("MIDI contains a retired sidecar directive");
		if (e->meta == 1 && !skeleton)
			skeleton = (char *) e->data.data;
		if (e->meta == 0x58)
		{
			if (e->data.len < 2 || !e->data.data[0] || e->data.data[1] > 7)
				fail("invalid MIDI time signature");
			int64_t span = (int64_t) e->data.data[0] * midi->division * 4,
					denominator = INT64_C(1) << e->data.data[1];
			if (span % denominator ||
				(meterCount && meter[meterCount - 1].tick >= e->tick))
				fail("time signature does not fit the MIDI grid");
			meter = resize(meter, meterCount + 1, sizeof(TimeSignature));
			meter[meterCount++] = (TimeSignature){e->tick, span / denominator};
		}
	}
	if (!meterCount || meter[0].tick)
	{
		meter = resize(meter, meterCount + 1, sizeof(TimeSignature));
		memmove(meter + 1, meter, meterCount * sizeof(TimeSignature));
		meter[0] = (TimeSignature){0, (int64_t) midi->division * 4};
		meterCount++;
	}
	if (!skeleton)
		fail("MIDI conductor skeleton is missing");
	*signatures = meter;
	*count = meterCount;
	return skeleton;
}

/*
 * A song header contains one tone-bank pointer followed by its track pointers.
 * Tone banks are external; track targets are checked against emitted labels.
 */
static void
sequenceHeader(Assembly *assembly, const Strings *fields, Strings *toneBanks)
{
	const char *block = field(fields, "block_count"),
			   *priority = field(fields, "priority"),
			   *reverb = field(fields, "reverb"),
			   *tone = field(fields, "tone_bank"),
			   *tracks = field(fields, "tracks");
	if (fields->len != 7)
		fail("sequence header has unexpected fields");
	if (number(block, 10, 255))
		fail("nonzero sequence block count is unsupported");
	Strings list = split(tracks, ',');
	size_t count = 0;
	for (size_t j = 0; j < list.len; j++)
		count += *list.items[j] != 0;
	if (!count || count > 16)
		fail("sequence header needs 1..16 tracks");
	labelAt(assembly, fields->items[1], assembly->bytes.len, true);
	byte(&assembly->bytes, count);
	byte(&assembly->bytes, 0);
	byte(&assembly->bytes, number(priority, 10, 255));
	byte(&assembly->bytes, number(reverb, 10, 255));
	addPointer(assembly, tone, 0);
	if (!hasString(toneBanks, tone))
		addString(toneBanks, tone);
	for (size_t j = 0; j < list.len; j++)
		if (*list.items[j])
			addPointer(assembly, list.items[j], 0);
	freeStrings(&list);
}

int
main(int argc, char **argv)
{
	const char *usage = "usage: ags-music INPUT.mid OUTPUT.s";
	startTool(argc, argv, usage);
	if (argc != 3)
		fail("%s", usage);
	Buffer input = readFile(argv[1]);
	Midi midi = readMidi(&input);
	free(input.data);
	TimeSignature *meter;
	size_t meterCount;
	const char *skeleton = readConductor(&midi, &meter, &meterCount);
	Strings lines = split(skeleton, '\n');
	if (strcmp(lines.items[0], "smsh-sequence 1"))
		fail("unsupported MIDI conductor skeleton");
	if (lines.len && !*lines.items[lines.len - 1])
		free(lines.items[--lines.len]);
	Assembly a = {0};
	size_t streamIndex = 0;
	Strings toneBanks = {0};
	for (size_t i = 1; i < lines.len; i++)
	{
		Strings f = split(lines.items[i], ' ');
		if (!f.len)
			fail("empty skeleton row");
		if (!strcmp(f.items[0], "stream") && f.len == 2)
		{
			labelAt(&a, f.items[1], a.bytes.len, false);
			streamIndex++;
			Track empty = {0};
			encodeTrack(&a,
						streamIndex < midi.count ? midi.tracks + streamIndex
												 : &empty,
						meter, meterCount);
		}
		else if (!strcmp(f.items[0], "align") && f.len == 3)
		{
			size_t n = number(f.items[1], 10, 256);
			if (n < 2)
				fail("sequence alignment must be 2..256");
			alignAssembly(&a, n, number(f.items[2], 10, 255));
		}
		else if (!strcmp(f.items[0], "header") && f.len >= 2)
		{
			sequenceHeader(&a, &f, &toneBanks);
		}
		else
			fail("unsupported conductor skeleton line: %s", lines.items[i]);
		freeStrings(&f);
	}
	for (size_t i = 0; i < a.pointerCount; i++)
	{
		bool found = hasString(&toneBanks, a.pointers[i].symbol);
		for (size_t j = 0; j < a.labelCount; j++)
			found |= !strcmp(a.labels[j].name, a.pointers[i].symbol);
		if (!found)
			fail("undefined sequence symbol: %s", a.pointers[i].symbol);
	}
	Buffer out = {0};
	print(&out, "\t.section .rodata\n");
	appendAssemblySource(&out, &a);
	writeFile(argv[2], &out);
	free(out.data);
	freeAssembly(&a);
	freeStrings(&toneBanks);
	freeStrings(&lines);
	free(meter);
	freeMidi(&midi);
	return 0;
}
