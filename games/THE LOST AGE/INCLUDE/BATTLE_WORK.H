#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_EVENT.H"
#include "RAM_BUFFER.H"
#include "../../COMMON/INCLUDE/BATTLE/BATTLE_SESSION.H"

/* The event playback state, ☀️'s with the cue a halfword and one more
   halfword beside it. */
struct BattleEventState {
    struct BattleEventQueue queue;  /* 0x000 */
    s32 phase;                      /* 0x148 */
    s32 event_index;                /* 0x14c */
    s32 timer;                      /* 0x150 */
    u8 unknown_154[0x14];
    s16 pending_cue;                /* 0x168, played before the next actor */
    s16 unknown_16a;
    s32 flags;                      /* 0x16c */
};

LAYOUT_SIZE_GUARD(BattleEventState_Size, struct BattleEventState, 0x170);
LAYOUT_OFFSET_GUARD(BattleEventState_PendingCue, struct BattleEventState, pending_cue, 0x168);

/*
 * The Lost Age's battle session, which its heap slot 9 holds. It opens as
 * The Broken Seal's does; the summon queue and the event queue lie later.
 * Only the fields its battle code has shown are named.
 */
struct BattleSession {
    s32 enemy_group;                /* 0x000 */
    u8 unknown_004[0x3d];
    u8 party_status_mode;           /* 0x041 */
    u8 unknown_042[2];
    u8 two_sided;                   /* 0x044 */
    u8 encounter_mode;              /* 0x045 */
    u8 unknown_046[0x0a];
    u8 link_side;                   /* 0x050 */
    u8 unknown_051;
    u8 link_paused;                 /* 0x052 */
    u8 unknown_053[5];
    s16 party_units[7];             /* 0x058 */
    s16 enemy_units[7];             /* 0x066 */
    u8 placement[16];               /* 0x074, standing place per unit */
    struct BattleObjectSlot slots[14]; /* 0x084, object records in formation order */
    struct BattleActionRecord actions[20]; /* 0x2ec */
    u8 unknown_42c[0x138];
    /* Summons queued by the effect that calls the next one in. */
    u16 queued_summons[2];          /* 0x564 */
    u8 queued_charges[2];           /* 0x568 */
    s8 queue_head;                  /* 0x56a */
    s8 queue_count;                 /* 0x56b */
    u8 unknown_56c[0x178];
    struct BattleEventState events; /* 0x6e4 */
    u8 unknown_854[0x14];
    u8 unknown_868;                 /* 0x868 */
};

LAYOUT_OFFSET_GUARD(
    BattleSession_PartyStatusMode,
    struct BattleSession,
    party_status_mode,
    0x041);
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
