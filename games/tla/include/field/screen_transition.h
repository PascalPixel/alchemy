#ifndef ALCHEMY_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_FIELD_SCREEN_TRANSITION_H

/// @file
/// What the field sequence's parts share with the files that define their
/// tables: the speakers' voices and Halted lines, the field effects' scripts
/// and colour curves, the screen transitions' shapes and window values, and
/// the views of an actor and of the map state that the ground mask reads.

#include "types.h"
#include "resource_ids.h"
#include "battle/battle.h"

/// A row of gSpeakerDefinitions: a sprite's voice and the lines it says while
/// Halted.
struct SpeakerDefinition {
  u16 id; ///< the sprite id
  /// The voice: a message plays SE_VOICE_FIRST + value as it types
  /// (Speaker_GetVoice).
  u8 value;
  /// The pair of lines from MsgPonderEllipsis it says when talked to while
  /// Halted with no event for that (Speaker_GetHaltedLines).
  u8 haltedLines;
};

/// An actor (struct Actor) as the ground mask reads it: its place, its ground
/// height's halves and its layer.
struct FadeActor {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
  u16 groundHeightFraction; ///< Actor.groundHeight's fraction
  s16 floor;                ///< its whole pixels
  u8 unusedAfterFloor[10];  ///< no code names it
  u8 terrainId;
};

/// The map state (struct MapState) as the ground mask reads its view position:
/// viewX and viewZ as halfwords, the second of each its whole pixels.
struct FadeView {
  u8 unused[228]; ///< MapState up to viewX
  s16 camera[4];
};

/// A run of lines of a window transition's shape (TransitionShape): its count
/// of lines and its WIN_RANGE, or when that is 0 the window edges it moves
/// between.
struct TransitionSpan {
  u16 count;
  u16 value;
  u8 x0;
  u8 x1;
  u8 y0;
  u8 y1;
};

/// WININ or WINOUT showing every layer in both windows (bits 6 and 14 set too).
#define TRANSITION_SHOW_ALL 0x7f7f
/// WININ or WINOUT showing every layer in its low byte (window 0, or outside
/// both windows; bit 6 set too) and nothing in its high byte.
#define TRANSITION_SHOW_LOW_BYTE 0x007f

/// A shape of WINDOW_TRANSITION_SHAPE: WINOUT and WININ while opening (swapped
/// while closing), then up to three runs of lines down the screen, each a
/// count of lines and either a WIN_RANGE for all of them or, when 0, the window
/// edges from (x0, y0) on the first line toward (x1, y1) on the last.
struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

/// SpeakerDefinition.value of a sprite that makes no sound as it speaks.
#define SPEAKER_VOICE_NONE 255
/// SpeakerDefinition.haltedLines of a sprite Halt never stops: the monsters
/// that talk, and things such as chests, rocks, trees and signposts.
#define SPEAKER_HALTED_LINES_NONE 255

/// Each sprite's voice and Halted lines, the default last.
extern struct SpeakerDefinition gSpeakerDefinitions[];

/// The found item's picture over the leader: growing in from half size, then
/// pulsing to 1.5 times its size and resting, over and over.
extern const s32 gItemDisplay_GrowInScript[];
extern const s32 gItemDisplay_PulseScript[];

/// A Reveal marker's script, which releases it once Field_IsRevealActive
/// returns 0, Reveal having ended.
extern const s32 gReveal_MarkerScript[];
s32 Field_IsRevealActive(struct Actor *interpreter);

/// The palette tints' colour channel curves: each maps a 5-bit channel to a
/// palette fade channel, shifted up 10 bits.
extern u16 gFieldEffect_ChannelCurve[];
extern u16 gFieldEffect_ChannelCurveLight[];
extern u16 gFieldEffect_ChannelCurveContrast[];

/// The width of the bars of WINDOW_TRANSITION_BARS, by progress.
extern const u8 gScreenTransition_NoiseWidths[];
/// The wobble of the circle edge of WINDOW_TRANSITION_RIPPLE.
extern const s8 gScreenTransition_EdgeOffsets[];
/// The window shape of each step of WINDOW_TRANSITION_SHAPE.
extern struct TransitionShape gScreenTransition_Shapes[];
/// 0 to 63 shuffled: the order in which the dissolve sets the pixels.
extern const u8 gScreenTransition_DitherTable[];

#endif
