#ifndef ALCHEMY_BATTLE_PRESENTATION_H
#define ALCHEMY_BATTLE_PRESENTATION_H

#include "TYPES.H"

struct BattleCamera {
    u8 unknown_00[12];
    s32 pos[3];
    s32 unknown_18;
    s32 *follow_pos;
    s32 distance;
    u8 unknown_24[16];
    u16 pitch;
    u16 yaw;
};

struct BattlePresentationTransition {
    s32 target_yaw;
    s32 frames;
    u8 unknown_08[12];
    s32 flag;
};

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32 BattlePresentation_RunAction(s16 *action);
s32 BattlePresentation_RunPairedUnitTransition(s16 *action);
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
