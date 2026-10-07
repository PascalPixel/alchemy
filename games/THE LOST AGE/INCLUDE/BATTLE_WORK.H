#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_EVENT.H"
#include "RAM_BUFFER.H"
#include "../../COMMON/INCLUDE/BATTLE/BATTLE_SESSION.H"

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

struct BattleSession {
    s32 enemy_group;
    u8 unknown_004[0x3d];
    u8 party_status_mode;
    u8 unknown_042;
    u8 flags_043;
    u8 two_sided;
    u8 encounter_mode;
    u8 unknown_046[2];
    u8 owner_slots[8];
    u8 link_side;
    u8 unknown_051;
    u8 link_paused;
    u8 unknown_053[5];
    s16 party_units[7];
    s16 enemy_units[7];
    u8 placement[16];
    struct BattleObjectSlot slots[14];
    struct BattleActionRecord actions[20];
    u8 unknown_42c[0x138];
    u16 queued_summons[2];
    u8 queued_charges[2];
    s8 queue_head;
    s8 queue_count;
    u8 unknown_56c[0x178];
    struct BattleEventState events;
    u8 unknown_854[4];
    s32 unknown_858;
    s32 unknown_85c;
    u8 unknown_860[8];
    u8 unknown_868;
};

LAYOUT_OFFSET_GUARD(
    BattleSession_PartyStatusMode,
    struct BattleSession,
    party_status_mode,
    0x041);
LAYOUT_OFFSET_GUARD(BattleSession_Flags043, struct BattleSession, flags_043, 0x043);
LAYOUT_OFFSET_GUARD(BattleSession_TwoSided, struct BattleSession, two_sided, 0x044);
LAYOUT_OFFSET_GUARD(BattleSession_EncounterMode, struct BattleSession, encounter_mode, 0x045);
LAYOUT_OFFSET_GUARD(BattleSession_PartyUnits, struct BattleSession, party_units, 0x058);
LAYOUT_OFFSET_GUARD(BattleSession_EnemyUnits, struct BattleSession, enemy_units, 0x066);
LAYOUT_OFFSET_GUARD(BattleSession_Placement, struct BattleSession, placement, 0x074);
LAYOUT_OFFSET_GUARD(BattleSession_Slots, struct BattleSession, slots, 0x084);
LAYOUT_OFFSET_GUARD(BattleSession_Actions, struct BattleSession, actions, 0x2ec);
LAYOUT_OFFSET_GUARD(
    BattleSession_QueuedSummons,
    struct BattleSession,
    queued_summons,
    0x564);
LAYOUT_OFFSET_GUARD(BattleSession_QueueCount, struct BattleSession, queue_count, 0x56b);
LAYOUT_OFFSET_GUARD(BattleSession_Events, struct BattleSession, events, 0x6e4);
LAYOUT_OFFSET_GUARD(BattleSession_Unknown868, struct BattleSession, unknown_868, 0x868);

#endif
