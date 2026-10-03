#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

#include "BATTLE_SESSION.H"

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

struct BattleCamera {
    u8 unknown_00[12];
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
    s32 active;                    /* 0x10, an escape presentation is playing */
    s32 flag;
};

/* The action effect prepared from a plan and consumed by the presentation
   dispatchers. Its target storage is distinct from an individual effect
   argument: presentation reserves 24 ids. */
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
    s16 actors[24];
};

LAYOUT_SIZE_GUARD(BattlePresentationTransition_Size, struct BattlePresentationTransition, 0x18);
LAYOUT_OFFSET_GUARD(BattlePresentationTransition_Active, struct BattlePresentationTransition, active, 0x10);
LAYOUT_SIZE_GUARD(BattlePresentationWork_Size, struct BattlePresentationWork, 0x54);
LAYOUT_OFFSET_GUARD(BattlePresentationWork_Actors, struct BattlePresentationWork, actors, 0x24);

/* Background modes share heap slot 44 with the other presentation work. */
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

/* Each page supplies one BG2CNT value per display line. */
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

struct BattlePlan;
s32 BattlePres_BuildTargetList(
    struct BattlePlan *plan, struct BattlePresentationWork *work);

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattlePresentation_RunAction(s16 *action);
s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action);
s32 BattleObject_IsValidId(u32 object_id);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode);
void BattlePres_SetActorRecordMode(s32 actor, s32 mode);
void BattleActor_CommitPlacement(void);
void BattleFx_InitializeModeFar(s32 *context);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void Palette_StepFadeTransfer(void);
struct BattleEffectArgument;
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

#endif
