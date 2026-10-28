/*
 * sequence.c
 *
 * Encode sound-driver commands and derive their running status.
 * Events carry named commands and editable arguments. The encoder validates
 * their arity and range, then omits status or note parameters only when the
 * previous emitted state makes them redundant.
 */
#include "sequence.h"
#include <errno.h>

const unsigned durations[] = {
	0,	1,	2,	3,	4,	5,	6,	7,	8,	9,	10, 11, 12, 13, 14, 15, 16,
	17, 18, 19, 20, 21, 22, 23, 24, 28, 30, 32, 36, 40, 42, 44, 48, 52,
	54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96};
static const char *controls[] = {"priority",
								 "tempo",
								 "key_shift",
								 "voice",
								 "volume",
								 "pan",
								 "pitch_bend",
								 "pitch_bend_range",
								 "lfo_speed",
								 "lfo_delay",
								 "modulation_depth",
								 "modulation_type",
								 NULL,
								 NULL,
								 "tuning"};
static int
controlOpcode(const char *name)
{
	for (unsigned i = 0; i < 15; i++)
		if (controls[i] && !strcmp(controls[i], name))
			return 0xba + i;
	return -1;
}

static int64_t
signedNumber(const char *text)
{
	errno = 0;
	char *end;
	long long n = strtoll(text, &end, 10);
	if (!*text || *end || errno)
		fail("invalid sequence integer: %s", text);
	return n;
}

int64_t
eventArgument(const SequenceEvent *event, size_t i)
{
	if (i >= event->args.len)
		fail("%s is missing an argument", event->kind);
	return signedNumber(event->args.items[i]);
}

unsigned
boundedArgument(const SequenceEvent *event, size_t i, unsigned max)
{
	int64_t n = eventArgument(event, i);
	if (n < 0 || (uint64_t) n > max)
		fail("%s argument out of range", event->kind);
	return (unsigned) n;
}

/*
 * Wait and note opcodes share this fixed duration table; unsupported lengths
 * must be split by the timing layer rather than rounded.
 */
static unsigned
durationCode(int64_t n)
{
	for (unsigned i = 0; i < 49; i++)
		if (durations[i] == n)
			return i;
	fail("duration %lld is not in the sound driver's duration table", (long long) n);
}

SequenceEvent
sequenceEvent(const char *kind)
{
	return (SequenceEvent){copyString(kind), {0}};
}
void
addArgument(SequenceEvent *e, int64_t n)
{
	char *s = formatString("%lld", (long long) n);
	addString(&e->args, s);
	free(s);
}

SequenceEvent
sequenceControl(const char *kind, int64_t n)
{
	SequenceEvent e = sequenceEvent(kind);
	addArgument(&e, n);
	return e;
}

void
freeSequenceEvent(SequenceEvent *event)
{
	free(event->kind);
	freeStrings(&event->args);
	*event = (SequenceEvent){0};
}

SequenceEvent
parseSequenceEvent(const char *text)
{
	Strings parts = split(text, ' ');
	if (!identifier(parts.items[0]))
		fail("invalid sequence event: %s", text);
	SequenceEvent e = sequenceEvent(parts.items[0]);
	for (size_t i = 1; i < parts.len; i++)
	{
		if (!identifier(parts.items[i]))
			signedNumber(parts.items[i]);
		addString(&e.args, parts.items[i]);
	}
	freeStrings(&parts);
	return e;
}

static void
arity(const SequenceEvent *event, size_t n)
{
	if (event->args.len != n)
		fail("%s needs %zu arguments", event->kind, n);
}

static unsigned
parameter(const SequenceEvent *event, size_t i, const char *kind)
{
	int64_t n = eventArgument(event, i);
	bool sign = !strcmp(kind, "key_shift") || !strcmp(kind, "pan") ||
				!strcmp(kind, "pitch_bend") || !strcmp(kind, "tuning");
	if (n < (sign ? -128 : 0) || n > (sign ? 127 : 255))
		fail("%s parameter out of range", kind);
	return (uint8_t) n;
}

/*
 * Validate the complete command before relying on running status. Pointer
 * commands create relocations, so target addresses remain the linker's job.
 */
static void
encodeEvent(Assembly *assembly, const SequenceEvent *event, int *running)
{
	const char *kind = event->kind;
	if (!strcmp(kind, "label"))
	{
		arity(event, 1);
		labelAt(assembly, event->args.items[0], assembly->bytes.len, false);
	}
	else if (!strcmp(kind, "wait"))
	{
		arity(event, 1);
		byte(&assembly->bytes, 0x80 + durationCode(eventArgument(event, 0)));
	}
	else if (!strcmp(kind, "fine") || !strcmp(kind, "pattern_end"))
	{
		arity(event, 0);
		byte(&assembly->bytes, !strcmp(kind, "fine") ? 0xb1 : 0xb4);
	}
	else if (!strcmp(kind, "goto") || !strcmp(kind, "pattern") ||
			 !strcmp(kind, "repeat"))
	{
		bool repeat = !strcmp(kind, "repeat");
		arity(event, repeat ? 2 : 1);
		byte(&assembly->bytes, repeat				   ? 0xb5
							   : !strcmp(kind, "goto") ? 0xb2
													   : 0xb3);
		if (repeat)
			byte(&assembly->bytes, boundedArgument(event, 0, 255));
		addPointer(assembly, event->args.items[repeat ? 1 : 0], 0);
	}
	else if (!strcmp(kind, "note") || !strcmp(kind, "note_running"))
	{
		if (!event->args.len || event->args.len > 4)
			fail("note needs duration and up to three parameters");
		int opcode = 0xcf + (int) durationCode(eventArgument(event, 0));
		bool run = !strcmp(kind, "note_running");
		if (run &&
			(event->args.len == 1 || (*running >= 0 && *running != opcode)))
			fail("invalid running note status");
		if (!run)
			byte(&assembly->bytes, opcode);
		for (size_t i = 1; i < event->args.len; i++)
			byte(&assembly->bytes, boundedArgument(event, i, 127));
		*running = opcode;
	}
	else if (!strcmp(kind, "note_end") || !strcmp(kind, "note_end_running"))
	{
		bool run = !strcmp(kind, "note_end_running");
		if (event->args.len > 1 || (run && event->args.len != 1))
			fail("invalid note end parameters");
		if (run && *running >= 0 && *running != 0xce)
			fail("invalid running note end status");
		if (!run)
			byte(&assembly->bytes, 0xce);
		if (event->args.len)
			byte(&assembly->bytes, boundedArgument(event, 0, 127));
		*running = 0xce;
	}
	else
	{
		bool run = !strcmp(kind, "control_running");
		arity(event, run ? 2 : 1);
		const char *name = run ? event->args.items[0] : kind;
		int opcode = controlOpcode(name);
		if (opcode < 0)
			fail("unsupported sequence command: %s", name);
		unsigned value = parameter(event, run ? 1 : 0, name);
		if (run && (opcode == 0xbb || value >= 128 ||
					(*running >= 0 && *running != opcode)))
			fail("invalid running control status");
		if (!run)
			byte(&assembly->bytes, opcode);
		byte(&assembly->bytes, value);
		if (opcode != 0xbb)
			*running = opcode;
	}
}
/* Derive the sound driver's running status from editable MIDI events, not markers. */
/*
 * Forget inherited note parameters across control-flow boundaries. A called
 * pattern or branch can change the state seen by the following command.
 */
void
emitSequenceEvent(Assembly *assembly, const SequenceEvent *event,
				  SequenceState *state)
{
	const char *kind = event->kind;
	SequenceEvent rebuilt = {0};
	const SequenceEvent *out = event;
	if (!strcmp(kind, "note") || !strcmp(kind, "note_running"))
	{
		if (event->args.len < 3)
			fail("MIDI note needs duration, key and velocity");
		int key = boundedArgument(event, 1, 127),
			velocity = boundedArgument(event, 2, 127),
			opcode = 0xcf + (int) durationCode(eventArgument(event, 0));
		rebuilt = sequenceEvent("note");
		addArgument(&rebuilt, eventArgument(event, 0));
		if (state->velocity != velocity)
		{
			addArgument(&rebuilt, key);
			addArgument(&rebuilt, velocity);
		}
		else if (state->key != key)
			addArgument(&rebuilt, key);
		if (state->afterRest && state->running == opcode &&
			rebuilt.args.len > 1)
		{
			free(rebuilt.kind);
			rebuilt.kind = copyString("note_running");
		}
		state->running = opcode;
		state->key = key;
		state->velocity = velocity;
		out = &rebuilt;
	}
	else if (!strcmp(kind, "label") || !strcmp(kind, "pattern") ||
			 !strcmp(kind, "repeat") || !strcmp(kind, "goto"))
	{
		state->running = state->key = state->velocity = -1;
	}
	else if (!strcmp(kind, "control_running") ||
			 !strcmp(kind, "note_end_running"))
		fail("running status is derived by the converter");
	else if (!strcmp(kind, "note_end"))
	{
		if (state->running == 0xce && event->args.len == 1)
		{
			rebuilt =
				sequenceControl("note_end_running", eventArgument(event, 0));
			out = &rebuilt;
		}
		state->running = 0xce;
	}
	else
	{
		int opcode = controlOpcode(kind);
		if (opcode >= 0 && opcode != 0xbb)
		{
			if (state->afterRest && state->running == opcode &&
				event->args.len == 1 && parameter(event, 0, kind) < 128)
			{
				rebuilt = sequenceEvent("control_running");
				addString(&rebuilt.args, kind);
				addString(&rebuilt.args, event->args.items[0]);
				out = &rebuilt;
			}
			state->running = opcode;
		}
	}
	encodeEvent(assembly, out, &state->encoded);
	if (strcmp(kind, "pattern_end"))
		state->afterRest = !strcmp(kind, "wait");
	freeSequenceEvent(&rebuilt);
}
