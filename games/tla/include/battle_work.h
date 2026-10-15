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
    s32 event_index;
    s32 timer;
    u8 unknown_154[0x14];
    s16 pending_cue;
    s16 unknown_16a;
    s32 flags;
};

LAYOUT_SIZE_GUARD(BattleEventState_Size, struct BattleEventState, 0x170);
LAYOUT_OFFSET_GUARD(BattleEventState_PendingCue, struct BattleEventState, pending_cue, 0x168);

struct BattleWork {
    s32 enemy_group;
    u8 unknown_004[0x3d];
    u8 party_status;
    u8 unknown_042;
    u8 challenge_flag;
    u8 two_sided;
    u8 encounter_mode;
    u8 unknown_046[2];
    u8 owner_slots[8];
    u8 link_side;
    u8 unknown_051;
    u8 link_paused;
    u8 unknown_053[5];
    s16 party_units[7];
    s16 enemy_ids[7];
    u8 placement[16];
    struct BattleObj slots[14];
    struct BattleActionRecord actions[20];
    u8 unknown_42c[0x138];
    u16 queued_summons[2];
    u8 queued_charges[2];
    s8 queue_head;
    s8 queue_count;
    u16 palette[128];
    s32 brightness;
    u8 unknown_670[6];
    u16 highlight_step;
    u16 highlight_mode;
    u8 unknown_67a[0x6a];
    struct BattleEventState events;
    u8 unknown_854[4];
    s32 unknown_858;
    s32 unknown_85c;
    u8 unknown_860[8];
    u8 unknown_868;
};

LAYOUT_OFFSET_GUARD(
    BattleSession_PartyStatusMode,
    struct BattleWork,
    party_status,
    0x041);
LAYOUT_OFFSET_GUARD(BattleWork_ChallengeFlag, struct BattleWork, challenge_flag, 0x043);
LAYOUT_OFFSET_GUARD(BattleSession_TwoSided, struct BattleWork, two_sided, 0x044);
LAYOUT_OFFSET_GUARD(BattleSession_EncounterMode, struct BattleWork, encounter_mode, 0x045);
LAYOUT_OFFSET_GUARD(BattleSession_PartyUnits, struct BattleWork, party_units, 0x058);
LAYOUT_OFFSET_GUARD(BattleSession_EnemyUnits, struct BattleWork, enemy_ids, 0x066);
LAYOUT_OFFSET_GUARD(BattleSession_Placement, struct BattleWork, placement, 0x074);
LAYOUT_OFFSET_GUARD(BattleSession_Slots, struct BattleWork, slots, 0x084);
LAYOUT_OFFSET_GUARD(BattleSession_Actions, struct BattleWork, actions, 0x2ec);
LAYOUT_OFFSET_GUARD(
    BattleSession_QueuedSummons,
    struct BattleWork,
    queued_summons,
    0x564);
LAYOUT_OFFSET_GUARD(BattleSession_QueueCount, struct BattleWork, queue_count, 0x56b);
LAYOUT_OFFSET_GUARD(BattleSession_HighlightStep, struct BattleWork, highlight_step, 0x676);
LAYOUT_OFFSET_GUARD(BattleSession_HighlightMode, struct BattleWork, highlight_mode, 0x678);
LAYOUT_OFFSET_GUARD(BattleSession_Events, struct BattleWork, events, 0x6e4);
LAYOUT_OFFSET_GUARD(BattleSession_Unknown868, struct BattleWork, unknown_868, 0x868);

#endif
