#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

#include "edition.h"
#include "fixed_math.h"
#include "types.h"

struct BattleEffectParams;
void BattleEffect_RunParticleStreams(struct BattleEffectParams *effect,
                                     s32 mode);
/// The battle dissolves into darkness and a sun climbs over a ring of orbs
/// while burning drops rain, then the scene bursts in embers and flashes.
void BattleEffect_RunMeteor(struct BattleEffectParams *effect);
/// As Meteor, with the user throwing two great flames.
///
/// Abilities: Outer Space.
void BattleEffect_RunOuterSpace(struct BattleEffectParams *effect);
void BattleEffect_RunCybele(struct BattleEffectParams *effect);

/// Fills a size by size/2 byte map with circles of growing radius, each
/// shaded by the cosine of its radius (0 to 63) times scale (16.16).
void BattleEffect_DrawShadedCircles(u8 *map, s32 size, FIXED scale);
/// BattleEffect_DrawShadedCircles for a size by size map, each circle drawn in
/// all four quarters.
void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale);

void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack,
                                s32 wait);

/// The flags of a composite draw: clip the picture at the canvas's edges, and
/// mirror it.
enum {
  COMPOSITE_CLIP_X = 1,
  COMPOSITE_CLIP_Y = 2,
  COMPOSITE_FLIP_X = 4,
  COMPOSITE_FLIP_Y = 8,
  COMPOSITE_FLIP = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};
/// BattleEffect_BuildBlitter builds additive for 0, max for 1, transparent for
/// 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };
s32 BattleEffect_BuildBlitter(s32 slot, s32 width_shift, s32 height_shift,
                              s32 flags, u32 mode);

typedef s32 (*BattleEffectDrawRectangle)(void *destination, const void *source,
                                         s32 x, s32 y, s32 width, s32 height);
typedef BattleEffectDrawRectangle DrawRectangle;
typedef BattleEffectDrawRectangle DrawRectangleFn;

#endif
