#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_SESSION.H"

/* The advance arrow as the session keeps it: an OAM entry in the making. */
struct BattlePromptSprite {
    u32 unknown_00;
    union {
        u32 word;
        struct {
            unsigned y : 8;
            unsigned flags : 8;
            unsigned x : 9;
            unsigned high : 7;
        } bits;
    } attributes;
    union {
        u32 word;
        struct {
            unsigned tile : 10;
            unsigned high : 22;
        } bits;
    } tile;
};

/* Queued battle events and the state of their playback. */
struct BattleEventState {
    struct BattleEventQueue queue;  /* 0x000 */
    s32 phase;                      /* 0x148 */
    s32 event_index;                /* 0x14c */
    s32 timer;                      /* 0x150 */
    struct BattlePromptSprite display_entry; /* 0x154 */
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
    s16 sprite_slots[6];            /* 0x004, unit holding each sprite slot */
    u16 enemy_classes[6];           /* 0x010 */
    u32 summon_masks[6];            /* 0x01c */
    s8 summon_channels[6];          /* 0x034 */
    u8 unknown_03a[2];
    u16 first_defeated;             /* 0x03c, lowest slot defeated */
    u16 defeat_state;               /* 0x03e */
    u8 summon_count;                /* 0x040 */
    u8 party_status_mode;           /* 0x041 */
    u8 unknown_042;
    u8 flags_043;                   /* 0x043, bit 1: an enemy challenged */
    u8 two_sided;                   /* 0x044, a linked battle */
    u8 encounter_mode;              /* 0x045 */
    u8 unknown_046[2];
    u8 owner_slots[8];              /* 0x048, member slot per owner, 0xff none */
    u8 link_side;                   /* 0x050 */
    u8 link_misses;                 /* 0x051, consecutive missed link frames */
    u8 link_paused;                 /* 0x052 */
    u8 unknown_053;
    s32 prompt_slot;                /* 0x054, the advance arrow's tile slot */
    s16 party_units[7];             /* 0x058 */
    s16 enemy_units[7];             /* 0x066 */
    struct BattleObjectSlot slots[14]; /* 0x074, object records in formation order */
    u8 placement[14];               /* 0x2dc, standing place per unit */
    u8 unknown_2ea[2];
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

LAYOUT_SIZE_GUARD(BattleSession_Size, struct BattleSession, 0x828);

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
LAYOUT_OFFSET_GUARD(BattleSession_Slots, struct BattleSession, slots, 0x074);
LAYOUT_OFFSET_GUARD(BattleSession_Placement, struct BattleSession, placement, 0x2dc);
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

/* Reset hands on the value left in r0; callers retain its integer return. */
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_WaitForReady(void);

void Sys_SetMode(s32 mode);

#endif
