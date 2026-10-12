#ifndef ALCHEMY_UNIT_BATTLE_BATTLE_H
#define ALCHEMY_UNIT_BATTLE_BATTLE_H

#include "types.h"

struct BattleActionRecord;
struct BattleEventState;
struct BattlePresentationWork;
struct BattleSummonState;
struct BattleTransitionEntry;
struct BtlAct;
struct BtlObj;
struct BtlPlan;
struct FixedPointPosition;
struct MotionObject;

s32 DebugParty_LoadPreset(s32 preset);
void DebugBattle_ViewMessages(void);
void DebugBattle_RunMenu(void);
void BattlePresentation_UpdateCamera(void);
void BattleIntro_AnnounceEncounter(s32 enemy_count);
void BattleParty_CollectUnitList(void);
s32 BattlePresentation_InitializeTilePattern(void);
void BattlePresentation_ClearBg0VerticalOffset(void);
void Party_ReservedNoOp5B14(s32 unused);
s32 BattleParty_ResetActiveRuntimeFields(void);
s32 BattleParty_ReconcileDjinnRecovery(void);
struct BattleSummonState *BattleSummon_UpdateAvailability(void);
void BattleSummon_ReservedNoOp(void);
s32 UpdateNameEntries(void);
s32 LinkBattle_SendParty(void);
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source);
s32 BattlePres_WaitSync(void);
s32 BattlePres_SyncTurn(void);
s32 BattleParty_AssignMemberSlots(void);
s32 Battle_ClearSerialTransferPayload(void);
s32 Battle_RunEncounter(s32 enemy_group);
void BattlePres_AdjustCameraByShoulderKeys(void);
void BattlePres_AdjustCameraByShoulderKeysAlt(void);
s32 BattleUnit_TickDefenseModifier(s32 unit_id);
s32 BattleUnit_TickResistanceModifier(s32 unit_id);
s32 BattleUnit_TickDelusion(s32 unit_id);
s32 BattleUnit_TickCharm(s32 unit_id);
s32 BattleUnit_TickStun(s32 unit_id);
s32 BattleUnit_TickSleep(s32 unit_id);

#define MES_PUSH(m) BtlEvPush(BEV_MES, (u32)(m))

// L・R を押しているあいだカメラの yaw を 512 ずつ動かす。同じ関数がふたつある
#define DEFINE_ADJUST_CAMERA(name)                                                           \
	void name(void)                                                                          \
	{                                                                                        \
		struct BattleCamera					*cam = WORK_SLOT(HEAP_SLOT_CAMERA);              \
		struct BattlePresentationTransition *trans = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND); \
                                                                                             \
		if ((gKeysHeld & KEY_L) != 0)                                                        \
		{                                                                                    \
			cam->yaw += 512;                                                                 \
		}                                                                                    \
		if ((gKeysHeld & KEY_R) != 0)                                                        \
		{                                                                                    \
			cam->yaw -= 512;                                                                 \
		}                                                                                    \
		if (trans->flag == 0)                                                                \
		{                                                                                    \
			BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);                        \
		}                                                                                    \
	}

#endif
