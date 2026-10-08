#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "edition.h"
#include "layout_guard.h"

enum { BATTLE_EVENT_CAPACITY = 64 };

enum BattlePlaybackPhase {
    BATTLE_PLAYBACK_IDLE = 0,
    BATTLE_PLAYBACK_RESOLVE_TARGET = 1,
    BATTLE_PLAYBACK_DISPATCH = 2,
    BATTLE_PLAYBACK_READY = 4
};

struct BattleEventQueue {
    u8 opcodes[BATTLE_EVENT_CAPACITY];
    u32 operands[BATTLE_EVENT_CAPACITY];
    s32 target_index;
    s32 count;
};

LAYOUT_SIZE_GUARD(
    BattleEventQueue_Size,
    struct BattleEventQueue,
    0x148);
LAYOUT_OFFSET_GUARD(
    BattleEventQueue_Operands,
    struct BattleEventQueue,
    operands,
    0x040);
LAYOUT_OFFSET_GUARD(
    BattleEventQueue_TargetIndex,
    struct BattleEventQueue,
    target_index,
    0x140);
LAYOUT_OFFSET_GUARD(
    BattleEventQueue_Count,
    struct BattleEventQueue,
    count,
    0x144);

enum BattleEventOpcode {
    BEV_UNIT = 0,
    BEV_NUM = 1,
    BEV_ITEM = 2,
    BEV_ACT = 3,
    BEV_MES = 4,
    BEV_MES_ADD = 5,
    BEV_MARK = 6,
    BEV_CLR = 7,
    BEV_OBJ_BGN = 8,
    BEV_OBJ_RES = 9,
    BEV_REDRAW = 10,
    BEV_OBJ_END = 11,
    BEV_OBJ_EFX = 12,
    BEV_SCR = 13,
    BEV_SE = 14
};

s32 BattleActor_DestroyTemporaryObject(s32 unit_id);

#if GAME_TLA
void BtlEvPush(u32 opcode, u32 operand);
#else
u32 BtlEvPush(u32 opcode, u32 operand);
#endif
u32 BattleEv_DispatchQueued(void);

#endif
