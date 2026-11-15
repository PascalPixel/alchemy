#ifndef ALCHEMY_MODULE_FIELD_SCREEN_TRANSITION_H
#define ALCHEMY_MODULE_FIELD_SCREEN_TRANSITION_H

/// @file
/// What games/tla/src/field/field_sequence_screen_transition.c declares for the
/// files that use it, written once.

#include "types.h"
#include "battle/battle.h"

struct ObjectAnimationState;

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

struct BattleRenderObject {
  u8 unk00[0x50];
  struct ObjectAnimationState *animation;
  u8 kind;
};

struct ParticleEmitter {
  u8 padding[8];
  struct ParticlePosition position;
  u8 padding2[20];
  s32 travelOffset;
  u8 padding3[41];
  u8 active;
};

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[2];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

struct ParticleSource {
  u8 padding[8];
  struct ParticlePosition position;
};

struct FadeActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u16 unk14;
  s16 floor;
  u8 unk18[10];
  u8 terrainId;
};

struct FadeView {
  u8 unk00[228];
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

struct TransitionShape {
  u16 first;
  u16 second;
  struct TransitionSpan spans[3];
};

struct MapLayerFlags {
  u8 unk00[0x14];
  u16 flags;
};

extern struct SpeakerDefinition gSpeakerDefinitions[];

extern const u8 gFieldEffect_ParticleEmitterScript[];

extern const u8 gBattleEffect_MarkerParticleScript[];

extern const u8 gFieldEffect_RisingPulseScript[];

extern u16 gFieldEffect_ChannelCurve[];

extern u16 gFieldEffect_ChannelCurveLight[];

extern u16 gFieldEffect_ChannelCurveContrast[];

extern const u8 gScreenTransition_NoiseWidths[];

extern const s8 gScreenTransition_EdgeOffsets[];

extern struct TransitionShape gScreenTransition_Shapes[];

extern const u8 gScreenTransition_DitherTable[];

extern char ResourceId_Headings;

#endif
