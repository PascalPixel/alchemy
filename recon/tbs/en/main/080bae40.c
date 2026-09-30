/* 2026-09-30 (Mercury): 1864 of 1864 bytes, 4 differing halfwords (was 195
   at 1868 bytes): the plain three-statement swap and a separate this_unit
   for the sort fix the size and every sort register; left are two ldrsh
   scratch registers in CHECK_EFFECT_TARGET's damage-class switch (case 9 r0
   for r1, case 0 r1 for r3), reload's round-robin choice, which the ROM's
   picks match only for a case order that its layout contradicts. */
/* 2026-09-30: 4 of 1864 bytes differ (1864 vs 1864). The sort reads its
 * two units into their own locals (this_unit, next_unit): the ROM keeps the
 * filter loop's unit in r5 but the sort's first unit in r8, so they are
 * different variables. The swap is the plain three-statement swap, which
 * gives the ROM's pointer walk of unit_ids[inner + 1] and the size.
 * Remaining: the damage-class switch's ldrsh scratch registers. Reload
 * hands scratches out round-robin over its spill registers; here case 9
 * gets r0 and case 0 r1 where the ROM has r1 and r3 (case 2/3 r4, case 0's
 * second load r4 and case 1/4/5/7/8 r0 agree). The ROM's picks are exactly
 * what reload gives when it meets the cases in the order 2/3, 1/4/5/7/8,
 * 9, 0, but that source order lays the cases out in that order too (all 24
 * case orders tried). Also tried: pp > 0, if ((unit)->pp), hp > 0,
 * max_hp > hp, !(applies), & 15, split hp == 0 test, u32/u8 damage_class,
 * switch on the expression, u32 applies, every declaration slot for
 * this_unit: none moves the scratch. */
/* 2026-09-29: the anonymous struct type of the mark local is now named
 * TargetMark at file scope, because the permuter's reprint dropped the
 * inline struct body. Eight minutes of permutation: 2688 -> 2064; with the
 * natural spellings kept here (the next unit id read through pointer
 * arithmetic into a local before the swap, applies declared after
 * damage_class, Random16() * target_count) it is 2066: 73 register-only, 6
 * stack-only, 9 operand, 8 reordered, 6 inserted, 4 deleted. Reading
 * unit_ids[inner_index + 1] by index instead costs 5903. */
/* 2026-09-29: Owner_GetRecordFar carries the build's name; alchemy permute
 * scores 2688, from 2728. */
/*
 * BattleTarget_SelectForAction draft: 1864 of 1864 bytes, 81.0% aligned
 * similarity. The normal-order scan tests turn_order->party_units[target_index]
 * and reads the unit through slot = &turn_order->party_units[target_index],
 * which gives the ROM pre-check ([turn_order, #88]), the in-place walk of
 * turn_order + 88 and both ldrsh loads. The 0x100 mark goes through a
 * one-halfword struct set beside its use, so loop.c hoists it as a HImode
 * pool load into r5 and both pools land where the ROM has them (the loop is
 * over the move threshold for a bare constant). Case 2 rolls before
 * selected = 0, so selected crosses only the four sort calls and is
 * caller-saved at [sp, #0] as in the ROM.
 * Remaining (all register choice, same instructions):
 *  - Both scans: the ROM uses r0/r2/r4 for the pointer setup and r6 for the
 *    zero, ours r6/r0/r2 and r3; setting the mark before the loop instead
 *    gives the ROM registers but schedules its load into the pre-check.
 *  - Sort: the ROM spills the unit_ids base (mov r4, sp; adds r4, #56) to
 *    [sp, #16] and keeps selected in r1; ours holds the base in fp and
 *    selected in r4, so the spill slots shift by one. Declaration order moves
 *    nothing (380 permutations).
 *  - CHECK_EFFECT hp compare picks r0/r1 where the ROM picks r1/r3.
 */
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_EFX.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "FIXED_MATH.H"

s32 BattleFx_IsReviveFar(s32 effect);

struct BattleAiProfile {
    u8 unknown_00[0x35];
    s8 target_strategy;
};

struct BattleAiProfile *Owner_GetRecordFar(s32 class_id);
u32 Random16(void);

#define COUNT_PARTIAL_CURES(unit, count)                                      \
    {                                                                         \
        if ((unit)->delusion != 0)                                            \
            (count)++;                                                        \
        if ((u8)(unit)->confusion != 0)                                       \
            (count)++;                                                        \
        if ((unit)->charm != 0)                                               \
            (count)++;                                                        \
        if ((unit)->sleep != 0)                                               \
            (count)++;                                                        \
        if ((unit)->psy_seal != 0)                                            \
            (count)++;                                                        \
        if ((unit)->death_count != 0)                                         \
            (count)++;                                                        \
    }

#define COUNT_ALL_CURES(unit, count)                                          \
    {                                                                         \
        COUNT_PARTIAL_CURES((unit), (count));                                 \
        if ((unit)->evil_spirit != 0)                                         \
            (count)++;                                                        \
        if ((unit)->poison != 0)                                              \
            (count)++;                                                        \
    }

#define COUNT_POSITIVE_MODIFIERS(unit, count)                                 \
    {                                                                         \
        if ((unit)->attack_modifier > 0)                                      \
            (count)++;                                                        \
        if ((unit)->defense_modifier > 0)                                     \
            (count)++;                                                        \
        if ((unit)->res_modifier > 0)                                         \
            (count)++;                                                        \
        if ((s8)(unit)->status_12c > 0)                                       \
            (count)++;                                                        \
        if ((s8)(unit)->status_12d > 0)                                       \
            (count)++;                                                        \
        if ((s8)(unit)->status_12e > 0)                                       \
            (count)++;                                                        \
        if ((s8)(unit)->status_12f > 0)                                       \
            (count)++;                                                        \
    }

#define CHECK_EFFECT_TARGET(unit, action, applies, damage_class)              \
    {                                                                         \
        (applies) = 0;                                                        \
        switch ((action)->effect) {                                           \
        case EFX_ATK_UP2:                                                     \
        case EFX_ATK_UP1:                                                     \
            if ((unit)->attack_modifier + 1 <= 4)                             \
                (applies) = 1;                                                \
            if ((unit)->attack_modifier_turns == 1)                           \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_ATK_DOWN2:                                                   \
        case EFX_ATK_DOWN1:                                                   \
            if ((unit)->attack_modifier - 1 >= -4)                            \
                (applies) = 1;                                                \
            if ((unit)->attack_modifier_turns == 1)                           \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_DEF_UP2:                                                     \
        case EFX_DEF_UP1:                                                     \
            if ((unit)->defense_modifier + 1 <= 4)                            \
                (applies) = 1;                                                \
            if ((unit)->defense_modifier_turns == 1)                          \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_DEF_DOWN2:                                                   \
        case EFX_DEF_DOWN1:                                                   \
            if ((unit)->defense_modifier - 1 >= -4)                           \
                (applies) = 1;                                                \
            if ((unit)->defense_modifier_turns == 1)                          \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_RES_UP2:                                                     \
        case EFX_RES_UP1:                                                     \
            if ((unit)->res_modifier + 1 <= 4)                                \
                (applies) = 1;                                                \
            if ((unit)->res_modifier_turns == 1)                              \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_RES_DOWN2:                                                   \
        case EFX_RES_DOWN1:                                                   \
            if ((unit)->res_modifier - 1 >= -4)                               \
                (applies) = 1;                                                \
            if ((unit)->res_modifier_turns == 1)                              \
                (applies)++;                                                  \
            break;                                                            \
        case EFX_CURE_POISON:                                                 \
            if ((unit)->poison != 0)                                          \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_CURE_PART:                                                   \
            COUNT_PARTIAL_CURES((unit), (applies));                           \
            break;                                                            \
        case EFX_HEAL_60:                                                     \
        case EFX_HEAL_30:                                                     \
            if ((unit)->hp < (unit)->max_hp)                                  \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_BUFF_CLEAR:                                                  \
            COUNT_POSITIVE_MODIFIERS((unit), (applies));                      \
            break;                                                            \
        case EFX_CURE_ALL:                                                    \
            COUNT_ALL_CURES((unit), (applies));                               \
            break;                                                            \
        case EFX_DEATH_CURSE:                                                 \
            if ((unit)->death_count == 0)                                     \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_POISON:                                                      \
            if ((unit)->poison == 0)                                          \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_VENOM:                                                       \
            if ((unit)->poison <= 1)                                          \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_STUN:                                                        \
            if ((unit)->stun == 0)                                            \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_SLEEP:                                                       \
            if ((unit)->sleep == 0)                                           \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_EVIL_SPIRIT:                                                 \
            if ((unit)->evil_spirit == 0)                                     \
                (applies) = 1;                                                \
            break;                                                            \
        case EFX_REVIVE_FULL:                                                 \
        case EFX_REVIVE_HALF:                                                 \
        case EFX_REVIVE_80:                                                   \
            if ((unit)->hp == 0)                                              \
                (applies) = 100;                                              \
            break;                                                            \
        case 0:                                                               \
        case 1:                                                               \
        case 2:                                                               \
            break;                                                            \
        default:                                                              \
            (applies) = 1;                                                    \
            break;                                                            \
        }                                                                     \
        if ((unit)->hp == 0                                                   \
            && BattleFx_IsReviveFar((action)->effect) == 0)                  \
            (applies) = 0;                                                    \
        if ((applies) == 0) {                                                 \
            (damage_class) = ((action)->target_flags & 0x0f) - 1;             \
            switch (damage_class) {                                           \
            case 2:                                                           \
            case 3:                                                           \
                if ((unit)->hp != 0)                                          \
                    (applies)++;                                              \
                break;                                                        \
            case 9:                                                           \
                if ((unit)->pp != 0)                                          \
                    (applies)++;                                              \
                break;                                                        \
            case 0:                                                           \
                if ((unit)->hp != 0 && (unit)->hp < (unit)->max_hp)            \
                    (applies)++;                                              \
                break;                                                        \
            case 1:                                                           \
            case 4:                                                           \
            case 5:                                                           \
            case 7:                                                           \
            case 8:                                                           \
                if ((action)->power != 0 && (unit)->hp != 0)                  \
                    (applies)++;                                              \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    }

struct TargetMark { u16 v; };

s32 BattleTarget_SelectForAction(
    s32 actor_id,
    struct BattleAction *action)
{
    u16 order_positions[6];
    s32 unit_ids[6];
    s32 target_positions[6];
    struct BattleSession *turn_order;
    struct BattleUnit *unit;
    struct BattleUnit *next_unit;
    struct BattleUnit *this_unit;
    s32 candidate_count;
    s32 target_index;
    s32 target_count;
    s32 inner_index;
    s32 selected;
    s32 unit_id;
    s32 value;
    s32 next_value;
    s32 temp;
    s32 opposing;
    s32 damage_class;
    s32 applies;
    u32 roll;
    s16 *slot;
    struct TargetMark mark;

    turn_order = gBattleWork;
    target_count = 0;
    candidate_count = 0;

    if (action->target_mode != 0) {
        opposing = 0;
        if (action->target_mode == 2 || action->target_mode == 4)
            opposing = 1;

        if ((u32)actor_id > 7) {
            if (opposing != 0)
                goto scan_mirrored_order;
        } else if (opposing == 0) {
            goto scan_mirrored_order;
        }

        target_index = 0;
        while (turn_order->party_units[target_index] != 255) {
            slot = &turn_order->party_units[target_index];
            unit_id = *slot;
            if (unit_id != 254) {
                if (action->target_mode != 4 || unit_id == actor_id) {
                    unit_ids[candidate_count] = unit_id;
                    mark.v = 0x100;
                    order_positions[candidate_count] =
                        target_index | mark.v;
                    candidate_count++;
                }
            }
            target_index++;
        }
        goto scan_complete;

scan_mirrored_order:
        target_index = 0;
        while (turn_order->enemy_units[target_index] != 255) {
            unit_id = turn_order->enemy_units[target_index];
            if (unit_id != 254) {
                if (action->target_mode != 4 || unit_id == actor_id) {
                    unit_ids[candidate_count] = unit_id;
                    order_positions[candidate_count] =
                        target_index | 0x180;
                    candidate_count++;
                }
            }
            target_index++;
        }
    }

scan_complete:

    if (candidate_count == 0)
        return -2;

    for (target_index = 0;
         target_index < candidate_count;
         target_index++) {
        unit = Owner_GetStateFar(unit_ids[target_index]);
        CHECK_EFFECT_TARGET(unit, action, applies, damage_class);
        if (applies != 0) {
            unit_ids[target_count] = unit_ids[target_index];
            target_positions[target_count] = order_positions[target_index];
            target_count++;
        }
    }

    if (target_count == 0)
        return -1;

    if (action->target_mode == 1
        && action->range == 1
        && Owner_GetRecordFar(Owner_GetStateFar(actor_id)->class_id)
                ->target_strategy != 2
        && (u32)((action->target_flags & 0x0f) - 3) <= 2) {
        selected = -1;
        for (target_index = 0;
             target_index < target_count;
             target_index++) {
            for (inner_index = target_index;
                 inner_index < target_count - 1;
                 inner_index++) {
                this_unit = Owner_GetStateFar(unit_ids[inner_index]);
                next_unit = Owner_GetStateFar(unit_ids[inner_index + 1]);
                if (Owner_GetRecordFar(Owner_GetStateFar(actor_id)->class_id)
                        ->target_strategy == 0) {
                    value = this_unit->hp;
                    next_value = next_unit->hp;
                } else {
                    value = this_unit->max_hp;
                    next_value = next_unit->max_hp;
                }
                if (value < next_value) {
                    temp = unit_ids[inner_index];
                    unit_ids[inner_index] = unit_ids[inner_index + 1];
                    unit_ids[inner_index + 1] = temp;
                    temp = target_positions[inner_index];
                    target_positions[inner_index] =
                        target_positions[inner_index + 1];
                    target_positions[inner_index + 1] = temp;
                }
            }
        }

        switch (target_count) {
        case 1:
            selected = 0;
            break;
        case 2:
            roll = (u32)(11 * Random16()) >> 16;
            selected = 0;
            if (roll > 5)
                selected = 1;
            break;
        case 3:
            selected = (u32)(15 * Random16()) >> 16;
            if (selected <= 5)
                selected = 0;
            else if (selected <= 10)
                selected = 1;
            else
                selected = 2;
            break;
        case 4:
            selected = (u32)(18 * Random16()) >> 16;
            if (selected <= 5)
                selected = 0;
            else if (selected <= 10)
                selected = 1;
            else if (selected <= 14)
                selected = 2;
            else
                selected = 3;
            break;
        }

        if (selected >= 0)
            return target_positions[selected];
    }
    return target_positions[(u32)(Random16() * target_count) >> 16];
}
