#ifndef ALCHEMY_SPEAKER_H
#define ALCHEMY_SPEAKER_H

/* @file */
/* The speaker lookup's view of an actor's sprite, the speaker */
/* definitions, and the palette fade's heap slot. */

#include "sounds.h"
#include "static_assert.h"
#include "types.h"
#include "field/field_sequence.h"
#include "graphics/palette_buffers.h"
#include "system.h"

/* An actor's animated sprite (SPR), as far as the */
/* pointer to its sprite id. */
typedef struct SPEAKER_SPR {
	u8 beforeEntries[40]; /* Sprite's fields before its layers */
	/* SPR.entries[0], its first layer, whose first halfword is the */
	/* sprite id it shows (SPR_ANIMATION.anmId); null for none. */
	s16 *firstEntry;
} SPEAKER_SPR;

/* SPEAKER_DEFINITION.value of a voice: the first of its four blip sounds, less
 */
/* SE_VOICE_FIRST. */
#define SPEAKER_VOICE(sound) ((sound) - SE_VOICE_FIRST)
/* SPEAKER_DEFINITION.value of a sprite with no voice. */
#define SPEAKER_VOICE_NONE 255
/* SPEAKER_DEFINITION.haltedLines of a sprite Halt never stops: the monsters */
/* that talk, and things such as chests, rocks, trees and signposts. */
#define SPEAKER_HALTED_LINES_NONE 255

/* A speaker definition, a row of SpeakerDefinitions: a sprite, its */
/* voice (SPEAKER_VOICE_NONE for none) and the lines it says while Halted. */
typedef struct SPEAKER_DEFINITION {
	u16 id; /* the sprite id */
	/* The voice, SPEAKER_VOICE of the first of its four blip sounds: a message
	 */
	/* plays SE_VOICE_FIRST + value as it types (fdSpeakerGetVoice). */
	u8 value;
	/* The pair of lines from MsgPonderEllipsis it says when talked to while */
	/* Halted with no event for that (scTalkToActor): pair n is messages */
	/* MsgPonderEllipsis + 2n and the one after, one picked at random. */
	u8 haltedLines;
} SPEAKER_DEFINITION;

/* The palette fade, heap slot HEAP_SLOT_PALETTE_FADE under its own name. */
#define PaletteFadeState (*(PALETTE_FADE **)&HeapSlots[HEAP_SLOT_PALETTE_FADE])

#endif
