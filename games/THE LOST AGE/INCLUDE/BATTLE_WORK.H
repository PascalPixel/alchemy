#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_EVENT.H"
#include "RAM_BUFFER.H"
#include "../../COMMON/INCLUDE/BATTLE/BATTLE_SESSION.H"

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
    u8 unknown_046[0x12];
    s16 party_units[7];             /* 0x058 */
    s16 enemy_units[7];             /* 0x066 */
    u8 unknown_074[0x278];
    struct BattleActionRecord actions[20]; /* 0x2ec */
    u8 unknown_42c[0x138];
    /* Summons queued by the effect that calls the next one in. */
    u16 queued_summons[2];          /* 0x564 */
    u8 queued_charges[2];           /* 0x568 */
    s8 queue_head;                  /* 0x56a */
    s8 queue_count;                 /* 0x56b */
    u8 unknown_56c[0x178];
    struct BattleEventQueue events; /* 0x6e4 */
    u8 unknown_82c[0x3c];
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
