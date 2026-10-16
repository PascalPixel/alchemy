#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

// The Lost Age's own copy of games/common/include/battle/battle_display.h: its
// presentation work holds nine actors, where The Broken Seal's holds eight.

#include "battle_session.h"

#include "types.h"
#include "static_assert.h"
#include "edition.h"

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

struct BattlePresentationWork {
  s32 kind;
  s32 side;
  s32 actor;
  s32 firstTarget;
  s32 initialValue;
  s32 count;
  s32 variant;
  s32 flags;
  u8 unk20[4];
  s16 actors[9];
  u8 params[8][4];
};

STATIC_ASSERT(sizeof(struct BattlePresentationTransition) == 0x20);
STATIC_ASSERT(offsetof(struct BattlePresentationTransition, active) == 0x10);
STATIC_ASSERT(sizeof(struct BattlePresentationWork) == 0x58);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, actors) == 0x24);
STATIC_ASSERT(offsetof(struct BattlePresentationWork, params) == 0x36);

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
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused);

s32 BattlePres_BuildTargetList(struct BattlePlan *plan,
                               struct BattlePresentationWork *work);

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattlePresentation_RunPairedUnitTransition(
    struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 objectId);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode);
void BattlePres_SetActorRecordMode(s32 actor, s32 mode);
void BattleObjPut(void);
void BattleFx_InitializeModeFar(s32 *context);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void Palette_StepFadeTransfer(void);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
