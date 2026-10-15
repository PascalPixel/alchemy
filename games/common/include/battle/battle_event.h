#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "edition.h"
#include "layout_guard.h"

enum
{
	BATTLE_EVENT_CAPACITY = 64
};

enum BattlePlaybackPhase
{
	BATTLE_PLAYBACK_IDLE = 0,
	BATTLE_PLAYBACK_RESOLVE_TARGET = 1,
	BATTLE_PLAYBACK_DISPATCH = 2,
	BATTLE_PLAYBACK_READY = 4
};

struct BattleEventQueue
{
	u8	opcodes[BATTLE_EVENT_CAPACITY];
	u32 operands[BATTLE_EVENT_CAPACITY];
	s32 target_index;
	s32 count;
};

LAYOUT_SIZE_GUARD(BattleEventQueue_Size, struct BattleEventQueue, 0x148);
LAYOUT_OFFSET_GUARD(BattleEventQueue_Operands, struct BattleEventQueue, operands, 0x040);
LAYOUT_OFFSET_GUARD(BattleEventQueue_TargetIndex, struct BattleEventQueue, target_index, 0x140);
LAYOUT_OFFSET_GUARD(BattleEventQueue_Count, struct BattleEventQueue, count, 0x144);

enum BattleEventOpcode
{
	BATTLE_EVENT_UNIT = 0,
	BATTLE_EVENT_NUM = 1,
	BATTLE_EVENT_ITEM = 2,
	BATTLE_EVENT_ACT = 3,
	BATTLE_EVENT_MESSAGE = 4,
	BATTLE_EVENT_MESSAGE_ADD = 5,
	BATTLE_EVENT_MARK = 6,
	BATTLE_EVENT_CLEAR = 7,
	BATTLE_EVENT_OBJ_BEGIN = 8,
	BATTLE_EVENT_OBJ_RESET = 9,
	BATTLE_EVENT_REDRAW = 10,
	BATTLE_EVENT_OBJ_END = 11,
	BATTLE_EVENT_OBJ_EFFECT = 12,
	BATTLE_EVENT_SCR = 13,
	BATTLE_EVENT_SE = 14
};

#if GAME_TLA
s32 BattleActor_DestroyTemporaryObject(s32 unit_id);
#endif

#if GAME_TLA
void BattleEvPush(u32 opcode, u32 operand);
#else
#endif
#if GAME_TLA
u32 BattleEv_DispatchQueued(void);
#endif

#endif
