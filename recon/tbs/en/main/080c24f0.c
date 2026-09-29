/* 2026-09-29 alchemy permute: score 5390 to 520 on the permuter's scorer
   (0 is exact); remaining 11 register-only, 1 operand, 7 reordered. Kept
   rewrites: both linear searches as a first test and a do-while (3435 on
   their own), slot declared after chance, and the drop test and the
   lowest-value test with their operands swapped; then by hand the
   permuter-only spellings reverted at equal score, plain divisions for the
   level and floor, and the battle work by its linked name. What is left is
   the drop test: the reference computes the threshold 0x20000 >> chance
   before the random call and keeps it in r5, while the swapped test
   computes it after. Spelling the test threshold first realigns the whole
   function's allocation (3435), so the rest of this model only fits under
   the swapped test; a later seed-7 search focused on the tail found
   nothing below 520. */
/* Draft, not exact (2026-09-27): complete extent [080c24f0,080c2724),
   564 reference bytes. Current fixed-spread helper model: 568 bytes,
   273 differing halfwords, 163 aligned edits; topology still different.
   Original baseline: 564 bytes, 260 halfwords, 183 aligned edits.
   Residual: the ROM keeps `earned` in fp for the whole function; here it
   is spilled to a stack slot (sub sp, #4). The ROM also reloads
   unit->class_id and rec->item inside the two search loops (the first is
   still strength-reduced). Tried: separate locals per loop, goto loops,
   unsigned id test, the formation load order. */
/* H1 (2026-09-27): complete normalized baseline is 564/564 bytes,
 * 260 differing halfwords, 183 aligned edits and a 4-byte spill frame.
 * Exact AWARD_SPOILS/LEVEL_UP/APPLY_LEVEL_GAINS establish the shared reward
 * totals, byte level and halfword reward inputs. Both ROM reward blocks keep
 * a pointer to the selected u16 field across RNG calls, reload it afterwards,
 * and apply the same 30-percent floor. Test that calculation as one inline
 * helper with a field pointer, unit and random spread. Prediction: field
 * lifetime in r9, earned retained in fp, no spill frame; preserve all calls.
 * One coherent followup maximum; no declaration or loop spelling sweep.
 * H1 result: 568/564 bytes, 256 differing halfwords, 165 aligned edits.
 * The reward pointer now lives in r9 and the bonus/counter use r6/r5 as
 * referenced, but earned still spills. The parameterized spread additionally
 * keeps six in fp and emits MUL instead of the ROM's shifts/add. Reject H1
 * as canonical despite its aggregate improvement. Preserve it in Git before
 * the single followup: separate fixed-spread coin/experience helpers, since
 * the ROM and exact level-growth family use literal multipliers per award.
 * H2 result: 568/564 bytes, 273 differing halfwords, 163 aligned edits.
 * Fixed-spread helpers restore both reference RNG arithmetic sequences;
 * reward-field r9 and bonus/counter r6/r5 persist. Keep this local lifetime
 * recovery as the best justified draft, not as evidence of a near match.
 * Earned still spills, unit is r7 instead of r8, and spoils is r8 instead
 * of r7. Both search loops still cache comparison fields unlike the ROM.
 * Budget exhausted. Further award-helper spelling cannot address those
 * independent loop/alias and whole-owner lifetime disagreements. No credit.
 */
#include "TYPES.H"
#include "BATTLE_TYPES.H"

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

extern struct BattleFormation *gBattleWork;

struct BattleUnit *Owner_GetStateFar(s32);
struct EnemyRecord *Owner_GetRecordFar(s32);
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
u32 Random16(void);
u32 BattleRandom16Far(void);
s32 Item_EncodeBankedId(s32);

static __inline__ s32 BattleEnemy_CalculateCoinReward(
    struct BattleUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 6) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(
    struct BattleUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 4) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
    struct BattleUnit *unit;
    struct EnemyRecord *rec;
    struct BattleFormation *formation;
    struct BattleSpoils *spoils;
    s32 i;
    s32 chance;
    s32 slot;
    s32 lowest;
    s32 lowest_slot;
    s32 value;

    unit = Owner_GetStateFar(unit_id);
    formation = gBattleWork;
    spoils = &formation->spoils;
    if ((u32)unit_id < 8)
        return -1;
    if (unit->class_index != 0)
        return -2;
    slot = 0;
    i = 0;
    if (formation->enemies[0] != unit->class_id) {
        do {
            if (++i > 5)
                break;
        } while (formation->enemies[i] != unit->class_id);
    }
    if (i != 6)
        slot = i;
    if (formation->defeat_state != 2) {
        if (slot < formation->first_slot)
            formation->first_slot = slot;
        if (spoils->defeated)
            formation->defeat_state = 1;
    }
    spoils->defeated++;
    if (GameFlag_TestFar(0x173))
        return 0;
    GameFlag_SetBitFar(unit->class_id + 0x600);
    rec = Owner_GetRecordFar(unit->class_id);
    if (earned != 0) {
        if (rec->coins != 0)
            spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
        if (rec->experience != 0)
            spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
    } else {
        spoils->coins += rec->coins;
        spoils->experience += rec->experience;
    }
    if (rec->item == 0 || rec->item_chance == 0)
        return 0;
    i = 0;
    if (spoils->items[0] != rec->item) {
        do {
            if (++i > 3)
                break;
        } while (spoils->items[i] != rec->item);
    }
    if (i != 4)
        return 0;
    chance = rec->item_chance;
    if (earned != 0)
        chance -= 2;
    if (chance < 0)
        chance = 0;
    if ((s32)(BattleRandom16Far() & 0xffff) >= (0x20000 >> chance))
        return 0;
    lowest = 0x40000000;
    lowest_slot = -1;
    for (i = 0; i <= 3; i++) {
        value = Item_EncodeBankedId(spoils->items[i]);
        if (lowest > value) {
            lowest = value;
            lowest_slot = i;
        }
    }
    if (Item_EncodeBankedId(rec->item) > lowest)
        spoils->items[lowest_slot] = rec->item;
    return 0;
}
