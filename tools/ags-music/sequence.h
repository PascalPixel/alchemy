/*
 * sequence.h
 *
 * Named sound commands and their encoding state. MIDI timing produces events;
 * the sequence writer validates and encodes them into an Assembly.
 */
#ifndef AGS_SEQUENCE_H
#define AGS_SEQUENCE_H
#include "midi.h"
#include "../common/assembly.h"
typedef struct
{
	char *kind;
	Strings args;
} SequenceEvent;
typedef struct
{
	int running, encoded, key, velocity;
	bool afterRest;
} SequenceState;
typedef struct
{
	int64_t tick, length;
} TimeSignature;
extern const unsigned durations[49];
SequenceEvent sequenceEvent(const char *kind);
SequenceEvent sequenceControl(const char *kind, int64_t value);
SequenceEvent parseSequenceEvent(const char *text);
void addArgument(SequenceEvent *sequenceEvent, int64_t value);
int64_t eventArgument(const SequenceEvent *sequenceEvent, size_t index);
unsigned boundedArgument(const SequenceEvent *sequenceEvent, size_t index,
						 unsigned maximum);
void freeSequenceEvent(SequenceEvent *sequenceEvent);
void emitSequenceEvent(Assembly *assembly, const SequenceEvent *sequenceEvent,
					   SequenceState *state);
void encodeTrack(Assembly *assembly, const Track *track,
				 const TimeSignature *meter, size_t meterCount);
#endif
