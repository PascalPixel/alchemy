#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

#include "battle_session.h"

#include "types.h"
#include "layout_guard.h"

struct BattleCamera {
    s32 eye[3];
    s32 pos[3];
    s32 unknown_18;
    s32 *follow_pos;
    s32 distance;
    u8 unknown_24[16];
    u16 pitch;
    u16 yaw;
    u8 unknown_38[20];
};

LAYOUT_SIZE_GUARD(BattleCamera_Size, struct BattleCamera, 0x4c);
LAYOUT_OFFSET_GUARD(BattleCamera_Position, struct BattleCamera, pos, 0x0c);
LAYOUT_OFFSET_GUARD(BattleCamera_FollowPosition, struct BattleCamera, follow_pos, 0x1c);
LAYOUT_OFFSET_GUARD(BattleCamera_Pitch, struct BattleCamera, pitch, 0x34);
LAYOUT_OFFSET_GUARD(BattleCamera_Yaw, struct BattleCamera, yaw, 0x36);

extern struct BattleCamera *gCameraWork;

struct BattlePresentationTransition {
    s32 target_yaw;
    s32 frames;
    u8 unknown_08[8];
    s32 active;
    s32 flag;
    s32 unknown_18;
    s32 unknown_1c;
};

struct BattlePresentationWork {
    s32 kind;
    s32 side;
    s32 actor;
    s32 first_target;
    s32 initial_value;
    s32 count;
    s32 variant;
    s32 flags;
    u8 unknown_20[4];
    s16 actors[8];
    u8 params[8][4];
};

LAYOUT_SIZE_GUARD(BattlePresentationTransition_Size, struct BattlePresentationTransition, 0x20);
LAYOUT_OFFSET_GUARD(BattlePresentationTransition_Active, struct BattlePresentationTransition, active, 0x10);
LAYOUT_SIZE_GUARD(BattlePresentationWork_Size, struct BattlePresentationWork, 0x54);
LAYOUT_OFFSET_GUARD(BattlePresentationWork_Actors, struct BattlePresentationWork, actors, 0x24);

struct BattleBackgroundView {
    u8 unknown_00[8];
    s32 mode;
    s32 second_mode;
    s32 busy;
};

struct BattleAffine {
    u16 pa, pb, pc, pd;
    s32 x, y;
};

struct BattleAffineHdma {
    u32 page;
    u8 unknown_04[12];
    struct BattleAffine affine;
    u16 lines[2][160];
};

LAYOUT_SIZE_GUARD(BattleBackgroundView_Size, struct BattleBackgroundView, 0x14);
LAYOUT_OFFSET_GUARD(BattleBackgroundView_Mode, struct BattleBackgroundView, mode, 0x08);
LAYOUT_SIZE_GUARD(BattleAffineHdma_Size, struct BattleAffineHdma, 0x2a0);
LAYOUT_OFFSET_GUARD(BattleAffineHdma_Affine, struct BattleAffineHdma, affine, 0x10);
LAYOUT_OFFSET_GUARD(BattleAffineHdma_Lines, struct BattleAffineHdma, lines, 0x20);

struct BtlPlan;
s32 BattlePres_RunActorEntries(struct BtlPlan *plan, s32 unused);

s32 BattlePres_BuildTargetList(
    struct BtlPlan *plan, struct BattlePresentationWork *work);

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 object_id);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode);
void BattlePres_SetActorRecordMode(s32 actor, s32 mode);
void BtlObjPut(void);
void BattleFx_InitializeModeFar(s32 *context);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void Palette_StepFadeTransfer(void);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
