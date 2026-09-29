#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_EVENT.H"

/* One queued action: the acting unit and what it does. */
struct BattleActionRecord {
    s16 unit_id;                    /* 0x00 */
    u8 unknown_02[2];
    u16 value;                      /* 0x04 */
    s16 kind;                       /* 0x06 */
    u8 unknown_08[8];
};

/* What the party has won so far: coins, experience and up to four items. */
struct BattleSpoils {
    s32 coins;                      /* 0x00 */
    s32 experience;                 /* 0x04 */
    s32 defeated;                   /* 0x08 */
    u16 items[4];                   /* 0x0c */
};

/* Queued battle events and the state of their playback. */
struct BattleEventState {
    struct BattleEventQueue queue;  /* 0x000 */
    s32 phase;                      /* 0x148 */
    s32 event_index;                /* 0x14c */
    s32 timer;                      /* 0x150 */
    u8 display_entry[12];           /* 0x154 */
    s32 display_source;             /* 0x160 */
    s32 actor_id;                   /* 0x164 */
    s32 pending_cue;                /* 0x168, played before the next actor */
    s32 flags;                      /* 0x16c, bit 0: defeats earn spoils */
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
    u8 unknown_42c[0x104];
    struct BattleSpoils spoils;     /* 0x530 */
    u16 palette[128];               /* 0x544, BG palette 6 at full level */
    s32 brightness;                 /* 0x644 */
    u16 background;                 /* 0x648 */
    u8 unknown_64a[0x0a];
    struct BattlePlan plan;         /* 0x654 */
    struct BattleEventState events; /* 0x6b8 */
};

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_SIZE_GUARD(BattleSpoils_Size, struct BattleSpoils, 0x14);
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
LAYOUT_OFFSET_GUARD(BattleSession_Spoils, struct BattleSession, spoils, 0x530);
LAYOUT_OFFSET_GUARD(BattleSession_Palette, struct BattleSession, palette, 0x544);
LAYOUT_OFFSET_GUARD(
    BattleSession_Brightness,
    struct BattleSession,
    brightness,
    0x644);
LAYOUT_OFFSET_GUARD(
    BattleSession_Background,
    struct BattleSession,
    background,
    0x648);
LAYOUT_OFFSET_GUARD(BattleSession_Plan, struct BattleSession, plan, 0x654);
LAYOUT_OFFSET_GUARD(BattleSession_Events, struct BattleSession, events, 0x6b8);
LAYOUT_OFFSET_GUARD(BattleEventState_Phase, struct BattleEventState, phase, 0x148);
LAYOUT_OFFSET_GUARD(BattleEventState_Flags, struct BattleEventState, flags, 0x16c);

extern struct BattleSession *gBattleWork;

void Sys_SetMode(s32 mode);

#endif
