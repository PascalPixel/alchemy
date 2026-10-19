#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

/// @file
/// The battle camera and the presentation's transition, background and affine
/// work.

#include "battle_session.h"
#include "battle_effect_argument.h"

#include "types.h"
#include "static_assert.h"
#include "edition.h"
#include "farcall.h"

struct BattleCamera {
  s32 eye[3];
  s32 pos[3];
  s32 unk18;
  s32 *followPos;
  s32 distance;
  u8 unk24[16];
  u16 pitch;
  u16 yaw;
  u8 unk38[20];
};

STATIC_ASSERT(sizeof(struct BattleCamera) == 0x4c);
STATIC_ASSERT(offsetof(struct BattleCamera, pos) == 0x0c);
STATIC_ASSERT(offsetof(struct BattleCamera, followPos) == 0x1c);
STATIC_ASSERT(offsetof(struct BattleCamera, pitch) == 0x34);
STATIC_ASSERT(offsetof(struct BattleCamera, yaw) == 0x36);

extern struct BattleCamera *gCameraWork;

struct BattlePresentationTransition {
  s32 targetYaw;
  s32 frames;
  u8 unk08[8];
  s32 active;
  s32 flag;
  s32 unk18;
  s32 unk1c;
};

/// The effect updates its command while the presentation keeps animation state.
struct BattlePresentationWork {
  struct BattleEffectArgument effect;
  u8 savedAnimationParameters[8][4];
};

STATIC_ASSERT(sizeof(struct BattlePresentationTransition) == 0x20);
STATIC_ASSERT(offsetof(struct BattlePresentationTransition, active) == 0x10);
STATIC_ASSERT(sizeof(struct BattlePresentationWork) == 0x54);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, effect.actors) == 0x24);

struct BattleBackgroundView {
  u8 unk00[8];
  s32 mode;
  s32 secondMode;
  s32 busy;
};

struct BattleAffine {
  u16 pa, pb, pc, pd;
  s32 x, y;
};

struct BattleAffineHdma {
  u32 page;
  u8 unk04[12];
  struct BattleAffine affine;
  u16 lines[2][160];
};

STATIC_ASSERT(sizeof(struct BattleBackgroundView) == 0x14);
STATIC_ASSERT(offsetof(struct BattleBackgroundView, mode) == 0x08);
STATIC_ASSERT(sizeof(struct BattleAffineHdma) == 0x2a0);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, affine) == 0x10);
STATIC_ASSERT(offsetof(struct BattleAffineHdma, lines) == 0x20);

struct BattlePlan;
struct BattleEffectArgument;

#endif
