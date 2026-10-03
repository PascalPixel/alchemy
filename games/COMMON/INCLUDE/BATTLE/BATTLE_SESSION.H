#ifndef ALCHEMY_BATTLE_SESSION_H
#define ALCHEMY_BATTLE_SESSION_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Records both games keep in their battle sessions. Each game's
   BATTLE_WORK.H lays out its own session around them. */

struct MotionObject;
struct SpriteEntry;

struct BattleObjectSlot {
    struct MotionObject *object;
    u16 resource;  /* the unit's sprite resource */
    u16 overlay;   /* extra sprite resources registered on the unit's object */
    u16 animation;
    u16 effect;
    s32 anchor_x;
    s32 anchor_z;
    s32 palette;
    s32 scale;
    u16 active_conditions; /* the status icons this unit can display */
    u8 selected_condition;
    s8 cycle_timer;
    struct SpriteEntry *animation_entry; /* of the animation and effect resources */
    struct SpriteEntry *effect_entry;
    s16 active;
    s16 fading; /* set while a defeated unit dissolves */
};

/* One queued action: the acting unit and what it does. */
struct BattleActionRecord {
    s16 unit_id;                    /* 0x00 */
    u8 unknown_02[2];
    u16 priority;                   /* 0x04, signed when ordering turns */
    s16 command;                    /* 0x06, command dispatch selector */
    s16 parameter;                  /* 0x08 */
    s16 target;                     /* 0x0a */
    s16 range;                      /* 0x0c */
    s16 unknown_0e;
};

/* What the party has won so far: coins, experience and up to four items. */
struct BattleSpoils {
    s32 coins;                      /* 0x00 */
    s32 experience;                 /* 0x04 */
    s32 defeated;                   /* 0x08 */
    u16 items[4];                   /* 0x0c */
};

LAYOUT_SIZE_GUARD(BattleObjectSlot_Size, struct BattleObjectSlot, 0x2c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_AnchorX, struct BattleObjectSlot, anchor_x, 0x0c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Conditions, struct BattleObjectSlot, active_conditions, 0x1c);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_CycleTimer, struct BattleObjectSlot, cycle_timer, 0x1f);
LAYOUT_OFFSET_GUARD(BattleObjectSlot_Active, struct BattleObjectSlot, active, 0x28);

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Priority, struct BattleActionRecord, priority, 0x04);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Command, struct BattleActionRecord, command, 0x06);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Parameter, struct BattleActionRecord, parameter, 0x08);
LAYOUT_OFFSET_GUARD(BattleActionRecord_Target, struct BattleActionRecord, target, 0x0a);
LAYOUT_SIZE_GUARD(BattleSpoils_Size, struct BattleSpoils, 0x14);

#endif
