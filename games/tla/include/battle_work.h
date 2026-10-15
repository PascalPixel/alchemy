#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "types.h"
#include "layout_guard.h"
#include "battle_type.h"
#include "battle_event.h"
#include "ram_buffer.h"
#include "../../common/include/battle/battle_session.h"

struct BattleEventState {
    struct BattleEventQueue queue;
    s32 phase;
    s32 eventIndex;
    s32 timer;
    u8 unk154[0x14];
    s16 pendingCue;
    s16 unk16a;
    s32 flags;
};

LAYOUT_SIZE_GUARD(BattleEventState_Size, struct BattleEventState, 0x170);
LAYOUT_OFFSET_GUARD(BattleEventState_PendingCue, struct BattleEventState, pendingCue, 0x168);

struct BattleWork {
    s32 enemyGroup;
    u8 unk004[0x3d];
    u8 partyStatus;
    u8 unk042;
    u8 challengeFlag;
    u8 twoSided;
    u8 encounterMode;
    u8 unk046[2];
    u8 ownerSlots[8];
    u8 linkSide;
    u8 unk051;
    u8 linkPaused;
    u8 unk053[5];
    s16 partyUnits[7];
    s16 enemyIds[7];
    u8 placement[16];
    struct BattleObj slots[14];
    struct BattleActionRecord actions[20];
    u8 unk42c[0x138];
    u16 queuedSummons[2];
    u8 queuedCharges[2];
    s8 queueHead;
    s8 queueCount;
    u16 palette[128];
    s32 brightness;
    u8 unk670[6];
    u16 highlightStep;
    u16 highlightMode;
    u8 unk67a[0x6a];
    struct BattleEventState events;
    u8 unk854[4];
    s32 unk858;
    s32 unk85c;
    u8 unk860[8];
    u8 unk868;
};

LAYOUT_OFFSET_GUARD(
    BattleSession_PartyStatusMode,
    struct BattleWork,
    partyStatus,
    0x041);
LAYOUT_OFFSET_GUARD(BattleWork_ChallengeFlag, struct BattleWork, challengeFlag, 0x043);
LAYOUT_OFFSET_GUARD(BattleSession_TwoSided, struct BattleWork, twoSided, 0x044);
LAYOUT_OFFSET_GUARD(BattleSession_EncounterMode, struct BattleWork, encounterMode, 0x045);
LAYOUT_OFFSET_GUARD(BattleSession_PartyUnits, struct BattleWork, partyUnits, 0x058);
LAYOUT_OFFSET_GUARD(BattleSession_EnemyUnits, struct BattleWork, enemyIds, 0x066);
LAYOUT_OFFSET_GUARD(BattleSession_Placement, struct BattleWork, placement, 0x074);
LAYOUT_OFFSET_GUARD(BattleSession_Slots, struct BattleWork, slots, 0x084);
LAYOUT_OFFSET_GUARD(BattleSession_Actions, struct BattleWork, actions, 0x2ec);
LAYOUT_OFFSET_GUARD(
    BattleSession_QueuedSummons,
    struct BattleWork,
    queuedSummons,
    0x564);
LAYOUT_OFFSET_GUARD(BattleSession_QueueCount, struct BattleWork, queueCount, 0x56b);
LAYOUT_OFFSET_GUARD(BattleSession_HighlightStep, struct BattleWork, highlightStep, 0x676);
LAYOUT_OFFSET_GUARD(BattleSession_HighlightMode, struct BattleWork, highlightMode, 0x678);
LAYOUT_OFFSET_GUARD(BattleSession_Events, struct BattleWork, events, 0x6e4);
LAYOUT_OFFSET_GUARD(BattleSession_Unknown868, struct BattleWork, unk868, 0x868);

#endif
