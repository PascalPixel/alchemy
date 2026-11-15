#ifndef ALCHEMY_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_FIELD_SCREEN_TRANSITION_H

/// @file
/// What games/tla/src/field/field_sequence_screen_transition.c declares for the
/// files that use it, written once.

#include "types.h"
#include "resource_ids.h"
#include "battle/battle.h"

/// A row of gSpeakerDefinitions, as The Broken Seal's: a sprite's voice and
/// the lines it says while Halted.
struct SpeakerDefinition {
  u16 id; ///< the sprite id
  /// The voice: a message plays SE_VOICE_FIRST + value as it types
  /// (Speaker_GetVoice).
  u8 value;
  /// The pair of lines from MsgPonderEllipsis it says when talked to while
  /// Halted with no event for that (Speaker_GetHaltedLines).
  u8 haltedLines;
};

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

struct FadeView {
  u8 unused[228]; ///< no code names it
  s16 camera[4];
};

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

/// A shape of WINDOW_TRANSITION_SHAPE: WINOUT and WININ while opening (swapped
/// while closing), then up to three runs of lines down the screen, each a
/// count of lines and either a WIN_RANGE for all of them or, when 0, the window
/// edges from (x0, y0) on the first line toward (x1, y1) on the last.
struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

struct MapLayerFlags {
  u8 unused[0x14]; ///< no code names it
  u16 flags;
};

/// SpeakerDefinition.value of a sprite that makes no sound as it speaks.
#define SPEAKER_VOICE_NONE 0xff

extern struct SpeakerDefinition gSpeakerDefinitions[];

extern const s32 gFieldEffect_ParticleEmitterScript[];

extern const s32 gBattleEffect_MarkerParticleScript[];
s32 Field_IsRevealActive(struct Actor *interpreter);

extern const s32 gFieldEffect_RisingPulseScript[];

extern u16 gFieldEffect_ChannelCurve[];

extern u16 gFieldEffect_ChannelCurveLight[];

extern u16 gFieldEffect_ChannelCurveContrast[];

extern const u8 gScreenTransition_NoiseWidths[];

extern const s8 gScreenTransition_EdgeOffsets[];

extern struct TransitionShape gScreenTransition_Shapes[];

extern const u8 gScreenTransition_DitherTable[];

#endif
