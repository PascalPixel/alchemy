#ifndef ALCHEMY_BATTLE_SESSION_H
#define ALCHEMY_BATTLE_SESSION_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Records both games keep in their battle sessions. Each game's
   BATTLE_WORK.H lays out its own session around them. */

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

LAYOUT_SIZE_GUARD(BattleActionRecord_Size, struct BattleActionRecord, 0x10);
LAYOUT_SIZE_GUARD(BattleSpoils_Size, struct BattleSpoils, 0x14);

#endif
