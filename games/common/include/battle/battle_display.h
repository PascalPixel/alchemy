#ifndef ALCHEMY_BATTLE_DISPLAY_H
#define ALCHEMY_BATTLE_DISPLAY_H

#include "battle_session.h"

#include "types.h"
#include "layout_guard.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct BattleCamera
{
	s32	 eye[3];
	s32	 pos[3];
	s32	 unk18;
	s32 *followPos;
	s32	 distance;
	u8	 unk24[16];
	u16	 pitch;
	u16	 yaw;
	u8	 unk38[20];
};

LAYOUT_SIZE_GUARD(BattleCamera_Size, struct BattleCamera, 0x4c);
LAYOUT_OFFSET_GUARD(BattleCamera_Position, struct BattleCamera, pos, 0x0c);
LAYOUT_OFFSET_GUARD(BattleCamera_FollowPosition, struct BattleCamera, followPos, 0x1c);
LAYOUT_OFFSET_GUARD(BattleCamera_Pitch, struct BattleCamera, pitch, 0x34);
LAYOUT_OFFSET_GUARD(BattleCamera_Yaw, struct BattleCamera, yaw, 0x36);

extern struct BattleCamera *gCameraWork;

struct BattlePresentationTransition
{
	s32 targetYaw;
	s32 frames;
	u8	unk08[8];
	s32 active;
	s32 flag;
	s32 unk18;
	s32 unk1c;
};

struct BattlePresentationWork
{
	s32 kind;
	s32 side;
	s32 actor;
	s32 firstTarget;
	s32 initialValue;
	s32 count;
	s32 variant;
	s32 flags;
	u8	unk20[4];
	s16 actors[8];
	u8	params[8][4];
};

LAYOUT_SIZE_GUARD(BattlePresentationTransition_Size, struct BattlePresentationTransition, 0x20);
LAYOUT_OFFSET_GUARD(
	BattlePresentationTransition_Active, struct BattlePresentationTransition, active, 0x10);
LAYOUT_SIZE_GUARD(BattlePresentationWork_Size, struct BattlePresentationWork, 0x54);
LAYOUT_OFFSET_GUARD(BattlePresentationWork_Actors, struct BattlePresentationWork, actors, 0x24);

struct BattleBackgroundView
{
	u8	unk00[8];
	s32 mode;
	s32 secondMode;
	s32 busy;
};

struct BattleAffine
{
	u16 pa, pb, pc, pd;
	s32 x, y;
};

struct BattleAffineHdma
{
	u32					page;
	u8					unk04[12];
	struct BattleAffine affine;
	u16					lines[2][160];
};

LAYOUT_SIZE_GUARD(BattleBackgroundView_Size, struct BattleBackgroundView, 0x14);
LAYOUT_OFFSET_GUARD(BattleBackgroundView_Mode, struct BattleBackgroundView, mode, 0x08);
LAYOUT_SIZE_GUARD(BattleAffineHdma_Size, struct BattleAffineHdma, 0x2a0);
LAYOUT_OFFSET_GUARD(BattleAffineHdma_Affine, struct BattleAffineHdma, affine, 0x10);
LAYOUT_OFFSET_GUARD(BattleAffineHdma_Lines, struct BattleAffineHdma, lines, 0x20);

struct BattlePlan;
#if GAME_TLA
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused);

s32 BattlePres_BuildTargetList(struct BattlePlan *plan, struct BattlePresentationWork *work);

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale);

s32	 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action);
s32	 BattleObject_IsValidId(u32 objectId);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode);
void BattlePres_SetActorRecordMode(s32 actor, s32 mode);
void BattleObjPut(void);
#endif
#if GAME_TLA
void BattleFx_InitializeModeFar(s32 *context);
#endif
#if GAME_TLA
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void Palette_StepFadeTransfer(void);
#endif
struct BattleEffectArgument;
#if GAME_TLA
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);
#endif

#endif
