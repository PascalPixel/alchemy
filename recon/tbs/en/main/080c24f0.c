/* Draft, not exact (2026-09-24): 564 of 564 bytes, 260 differing
   halfwords (mostly a two-halfword shift). Every call, field and branch
   lines up with the listing. Residual: the ROM keeps `earned` in fp for
   the whole function; here it is spilled to a stack slot (sub sp, #4),
   which shifts everything after the prologue. The ROM also reloads
   unit->class_id and rec->item inside the two search loops (the first is
   still strength-reduced). Tried: separate locals per loop, goto loops,
   unsigned id test, the formation load order. */
#include "TYPES.H"
#include "BATTLE_TYPES.H"

#define BattleEnemy_RecordDefeat Func_080c24f0

/* Counts one defeated enemy toward the battle spoils: its coins and
   experience (randomly raised in proportion to the enemy's level when the
   party earned them), the formation slot it came from, and a chance at its
   item drop, which replaces the least valuable drop so far. */

struct BattleSpoils {
    s32 coins;
    s32 experience;
    s32 defeated;
    u16 items[4];
};

struct BattleFormation {
    u8 unknown_00[0x10];
    u16 enemies[6];                 /* 0x10 */
    u8 unknown_1c[0x20];
    u16 first_slot;                 /* 0x3c */
    u16 defeat_state;               /* 0x3e */
    u8 unknown_40[0x4f0];
    struct BattleSpoils spoils;     /* 0x530 */
};

struct EnemyRecord {
    u8 unknown_00[0x4c];
    u16 coins;                      /* 0x4c */
    s16 item;                       /* 0x4e */
    s16 item_chance;                /* 0x50 */
    u16 experience;                 /* 0x52 */
};

extern struct BattleFormation *Data_03001e74;

struct BattleUnit *Owner_GetStateFar(s32);
struct EnemyRecord *Owner_GetRecordFar(s32);
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
u32 Random16(void);
u32 BattleRandom16Far(void);
s32 Math_Div(s32, s32);
u32 Math_DivU(u32, u32);
s32 Item_EncodeBankedId(s32);

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
    struct BattleUnit *unit;
    struct EnemyRecord *rec;
    struct BattleFormation *formation;
    struct BattleSpoils *spoils;
    s32 i;
    s32 slot;
    s32 bonus;
    s32 base;
    s32 chance;
    s32 lowest;
    s32 lowest_slot;
    s32 value;

    unit = Owner_GetStateFar(unit_id);
    formation = Data_03001e74;
    spoils = &formation->spoils;
    if ((u32)unit_id < 8)
        return -1;
    if (unit->class_index != 0)
        return -2;
    slot = 0;
    for (i = 0; formation->enemies[i] != unit->class_id; ) {
        i++;
        if (i > 5)
            break;
    }
    if (i != 6)
        slot = i;
    if (formation->defeat_state != 2) {
        if (slot < formation->first_slot)
            formation->first_slot = slot;
        if (spoils->defeated != 0)
            formation->defeat_state = 1;
    }
    spoils->defeated++;
    if (GameFlag_TestFar(0x173) != 0)
        return 0;
    GameFlag_SetBitFar(unit->class_id + 0x600);
    rec = Owner_GetRecordFar(unit->class_id);
    if (earned != 0) {
        if (rec->coins != 0) {
            bonus = 0;
            for (i = 0; i < (u8)Math_DivU(unit->level, 10) + 1; i++)
                bonus += ((Random16() * 6) >> 16) + 1;
            base = rec->coins;
            value = Math_Div(base * 3, 10);
            if (bonus < value)
                bonus = value;
            spoils->coins += bonus + base;
        }
        if (rec->experience != 0) {
            bonus = 0;
            for (i = 0; i < (u8)Math_DivU(unit->level, 10) + 1; i++)
                bonus += ((Random16() * 4) >> 16) + 1;
            base = rec->experience;
            value = Math_Div(base * 3, 10);
            if (bonus < value)
                bonus = value;
            spoils->experience += bonus + base;
        }
    } else {
        spoils->coins += rec->coins;
        spoils->experience += rec->experience;
    }
    if (rec->item == 0 || rec->item_chance == 0)
        return 0;
    for (i = 0; spoils->items[i] != rec->item; ) {
        i++;
        if (i > 3)
            break;
    }
    if (i != 4)
        return 0;
    chance = rec->item_chance;
    if (earned != 0)
        chance -= 2;
    if (chance < 0)
        chance = 0;
    if ((0x20000 >> chance) <= (s32)(BattleRandom16Far() & 0xffff))
        return 0;
    lowest = 0x40000000;
    lowest_slot = -1;
    for (i = 0; i <= 3; i++) {
        value = Item_EncodeBankedId(spoils->items[i]);
        if (value < lowest) {
            lowest = value;
            lowest_slot = i;
        }
    }
    if (Item_EncodeBankedId(rec->item) > lowest)
        spoils->items[lowest_slot] = rec->item;
    return 0;
}
