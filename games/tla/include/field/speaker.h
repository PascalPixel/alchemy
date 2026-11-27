#ifndef ALCHEMY_SPEAKER_H
#define ALCHEMY_SPEAKER_H

/* Speaker bits, above the actor id in the low 12 bits of a message command's
   speaker, as fdMessageShowAtActor reads them, and each sprite's voice and
   Halted lines. */

#include "types.h"

#define SPEAKER_PORTRAIT_LEFT  0x1000 /* the portrait left of the speaker */
#define SPEAKER_PORTRAIT_RIGHT 0x2000 /* the portrait right of the speaker */
#define SPEAKER_WINDOW_ABOVE   0x4000 /* the message window above the speaker */
#define SPEAKER_WINDOW_BELOW   0x8000 /* the message window never above it */
#define SPEAKER_FLAGS		   0xf000 /* the four bits above */
#define SPEAKER_ACTOR_MASK	   0xfff  /* the actor id below them */

/* A row of SpeakerDefinitions: a sprite's voice and the lines it says while
   Halted. */
typedef struct SPEAKER_DEFINITION {
	Uint16 id; /* the sprite id */
	/* The voice: a message plays SE_VOICE_FIRST + value as it types
	   (fdSpeakerGetVoice). */
	Uint8 value;
	/* The pair of lines from MsgPonderEllipsis it says when talked to while
	   Halted with no script for that (fdSpeakerGetHaltedLines). */
	Uint8 haltedLines;
} SPEAKER_DEFINITION;

/* SPEAKER_DEFINITION.value of a sprite that makes no sound as it speaks. */
#define SPEAKER_VOICE_NONE		  255
/* SPEAKER_DEFINITION.haltedLines of a sprite Halt never stops: the monsters
   that talk, and things such as chests, rocks, trees and signposts. */
#define SPEAKER_HALTED_LINES_NONE 255

/* Each sprite's voice and Halted lines, the default last. */
extern SPEAKER_DEFINITION SpeakerDefinitions[];

#endif
