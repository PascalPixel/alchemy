#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* One queued action: the acting unit and what it does. */
struct BattleActionRecord {
    s16 unit_id;                    /* 0x00 */
    u8 unknown_02[2];
    u16 value;                      /* 0x04 */
    s16 kind;                       /* 0x06 */
    u8 unknown_08[8];
};

/*
 * The battle session gBattleWork points to. Unit lists hold unit ids ending
 * at 255; a removed unit's entry is 254 until the list is compacted.
 */
struct BattleSession {
    s32 enemy_group;                /* 0x000 */
    u8 unknown_004[0x3d];
    u8 party_status_mode;           /* 0x041 */
    u8 unknown_042;
    u8 flags_043;                   /* 0x043, bit 1: an enemy challenged */
    u8 unknown_044[0x14];
    s16 party_units[7];             /* 0x058 */
    s16 enemy_units[7];             /* 0x066 */
    u8 unknown_074[0x278];
    struct BattleActionRecord actions[20]; /* 0x2ec */
};

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_OFFSET_GUARD(
    BattleSession_PartyStatusMode,
    struct BattleSession,
    party_status_mode,
    0x041);
LAYOUT_OFFSET_GUARD(BattleSession_Flags043, struct BattleSession, flags_043, 0x043);
LAYOUT_OFFSET_GUARD(
    BattleSession_PartyUnits,
    struct BattleSession,
    party_units,
    0x058);
LAYOUT_OFFSET_GUARD(
    BattleSession_EnemyUnits,
    struct BattleSession,
    enemy_units,
    0x066);
LAYOUT_OFFSET_GUARD(BattleSession_Actions, struct BattleSession, actions, 0x2ec);

extern void *gBattleWork;

void Sys_SetMode(s32 mode);

#endif
