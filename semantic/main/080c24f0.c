typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct RewardTotals_080c24f0 {
    s32 primary;
    s32 secondary;
    s32 encounters;
    u16 rare_items[4];
};

struct BattleGlobals_080c24f0 {
    u8 unknown_000[0x10];
    u16 character_order[6];
    u8 unknown_01C[0x20];
    u16 earliest_character;
    u16 ordering_state;
    u8 unknown_040[0x4F0];
    struct RewardTotals_080c24f0 rewards;
};

struct Combatant_080c24f0 {
    u8 unknown_000[0xF];
    u8 reward_level;
    u8 unknown_010[0x118];
    u8 character;
    u8 unavailable;
};

struct CharacterRewards_080c24f0 {
    u8 unknown_00[0x4C];
    u16 primary;
    s16 rare_item;
    s16 rare_chance_shift;
    u16 secondary;
};

u32 Func_080022f4(u32, u32);
s32 Func_080022ec(s32, s32);
u32 Func_08004458(void);
struct Combatant_080c24f0 *Func_08077008(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);
struct CharacterRewards_080c24f0 *Func_08077198(u8);
u32 Func_080771a0(void);
u32 Func_080c2470(u32);

/*
 * Accumulate the rewards for one defeated non-party combatant.
 *
 * Randomized rewards roll one die per adjusted reward level, then enforce a
 * floor of 30% of the character's base reward.  The optional rare item is
 * admitted only once and replaces the least valuable of four retained items.
 */
s32 Func_080c24f0(s32 actor, s32 randomized) {
    struct Combatant_080c24f0 *combatant = Func_08077008(actor);
    struct BattleGlobals_080c24f0 *globals =
        *(struct BattleGlobals_080c24f0 **)0x03001E74;
    struct RewardTotals_080c24f0 *totals = &globals->rewards;
    struct CharacterRewards_080c24f0 *rewards;
    s32 order_index = 0;
    s32 earliest = 0;

    if ((u32)actor <= 7U) {
        return -1;
    }
    if (combatant->unavailable != 0) {
        return -2;
    }

    while (order_index < 6 &&
           globals->character_order[order_index] != combatant->character) {
        order_index++;
    }
    if (order_index != 6) {
        earliest = order_index;
    }

    if (globals->ordering_state != 2) {
        if (earliest < globals->earliest_character) {
            globals->earliest_character = earliest;
        }
        if (totals->encounters != 0) {
            globals->ordering_state = 1;
        }
    }
    totals->encounters++;

    if (Func_080770c0(0x173) != 0) {
        return 0;
    }

    Func_080770c8(combatant->character + 0x600);
    rewards = Func_08077198(combatant->character);

    if (randomized != 0) {
        if (rewards->primary != 0) {
            s32 bonus = 0;
            s32 roll = 0;
            s32 minimum;

            while (roll <
                   (s32)((u8)Func_080022f4(
                       combatant->reward_level,
                       10) + 1)) {
                bonus += (6U * Func_08004458() >> 16) + 1;
                roll++;
            }

            minimum = Func_080022ec(rewards->primary * 3, 10);
            if (bonus < minimum) {
                bonus = minimum;
            }
            totals->primary += bonus + rewards->primary;
        }

        if (rewards->secondary != 0) {
            s32 bonus = 0;
            s32 roll = 0;
            s32 minimum;

            while (roll <
                   (s32)((u8)Func_080022f4(
                       combatant->reward_level,
                       10) + 1)) {
                bonus += (4U * Func_08004458() >> 16) + 1;
                roll++;
            }

            minimum = Func_080022ec(rewards->secondary * 3, 10);
            if (bonus < minimum) {
                bonus = minimum;
            }
            totals->secondary += bonus + rewards->secondary;
        }
    } else {
        totals->primary += rewards->primary;
        totals->secondary += rewards->secondary;
    }

    if (rewards->rare_item != 0 && rewards->rare_chance_shift != 0) {
        s32 slot;

        for (slot = 0; slot < 4; slot++) {
            if (totals->rare_items[slot] == rewards->rare_item) {
                break;
            }
        }

        if (slot == 4) {
            s32 shift = rewards->rare_chance_shift;
            u32 chance;

            if (randomized != 0) {
                shift -= 2;
            }
            if (shift < 0) {
                shift = 0;
            }

            /*
             * The original ARM register shift yields zero at 32 or above.
             * Avoid relying on undefined oversized shifts in C.
             */
            chance = shift < 32 ? 0x20000U >> shift : 0;
            if (chance > (u16)Func_080771a0()) {
                s32 least_value = 0x40000000;
                s32 least_slot = -1;

                for (slot = 0; slot < 4; slot++) {
                    s32 value =
                        (s32)Func_080c2470(totals->rare_items[slot]);

                    if (value < least_value) {
                        least_value = value;
                        least_slot = slot;
                    }
                }

                if ((s32)Func_080c2470((u32)rewards->rare_item) >
                    least_value) {
                    totals->rare_items[least_slot] = rewards->rare_item;
                }
            }
        }
    }

    return 0;
}
