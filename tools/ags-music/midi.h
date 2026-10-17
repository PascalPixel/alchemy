/*
 * midi.h
 *
 * Parsed MIDI tracks in original event order. Each event owns its payload;
 * freeMidi releases the complete hierarchy.
 */
#ifndef AGS_MIDI_H
#define AGS_MIDI_H
#include "../common/common.h"
typedef struct
{
	int64_t tick;
	size_t order;
	unsigned status, meta;
	Buffer data;
} MidiEvent;
typedef struct
{
	MidiEvent *events;
	size_t count;
} Track;
typedef struct
{
	Track *tracks;
	size_t count;
	unsigned division;
} Midi;
Midi readMidi(const Buffer *data);
void freeMidi(Midi *midi);
#endif
