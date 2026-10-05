#ifndef ALCHEMY_BATTLE_EVENT_H
#define ALCHEMY_BATTLE_EVENT_H

#include "LAYOUT_GUARD.H"

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
    BATTLE_EVENT_UNIT = 0,
    BATTLE_EVENT_VALUE = 1,
    BATTLE_EVENT_ITEM = 2,
    BATTLE_EVENT_ACTION = 3,
    BATTLE_EVENT_TEXT = 4,
    BATTLE_EVENT_TEXT_CONTINUE = 5,
    BATTLE_EVENT_MARK = 6,
    BATTLE_EVENT_RESET = 7,
    BATTLE_EVENT_ACTOR_BEGIN = 8,
    BATTLE_EVENT_ACTOR_RESOLVE = 9,
    BATTLE_EVENT_REFRESH = 10,
    BATTLE_EVENT_ACTOR_FINISH = 11,
    BATTLE_EVENT_ACTOR_EFFECT = 12,
    BATTLE_EVENT_SCRIPT_UPDATE = 13,
    BATTLE_EVENT_SOUND = 14
};

s32 BattleActor_DestroyTemporaryObject(s32 unit_id);

u32 BattleEv_Push(u32 opcode, u32 operand);
u32 BattleEv_DispatchQueued(void);

#endif
