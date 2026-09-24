/*
 * BattleTarget_SelectForAction draft: 1864 of 1864 bytes, 77.0% aligned
 * similarity. The normal-order scan tests turn_order->normal[target_index]
 * and reads the unit through slot = &turn_order->normal[target_index],
 * which gives the ROM pre-check ([turn_order, #88]), the in-place walk of
 * turn_order + 88 and both ldrsh loads. Case 2 rolls before selected = 0 so
 * selected crosses only the four sort calls: caller-saves become profitable
 * and it lives in a low register saved at [sp, #0], as in the ROM.
 * Remaining:
 *  - The ROM hoists the 0x100 halfword pool constant into r5 before the
 *    normal scan (pool mid-loop, first pool at +0x40). Ours has 36 real
 *    insns in that loop in the second loop pass (2*i kept live by the GCSE
 *    copy it shares with the mirrored scan, plus the reg34 + 2i and + 88
 *    givs), above the move threshold the mirrored scan (28) passes.
 *    for (slot = turn_order->normal; *slot != 255; slot++) hoists it but lets
 *    GCSE share the test load with the body.
 *  - Sort: the ROM spills the unit_ids base (mov r4, sp; adds r4, #56) to
 *    [sp, #16] and keeps selected in r1; ours holds the base in fp, selected
 *    in r4, so the spill slots shift by one.
 *  - CHECK_EFFECT hp compare picks r0/r1 where the ROM picks r1/r3.
 */
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_EFX.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "FIXED_MATH.H"

#define BattleTarget_SelectForAction Func_080bae40

s32 Func_080772b8(s32 effect);
#define BattleFx_Classify Func_080772b8

struct BattleAiProfile {
    u8 unknown_00[0x35];
    s8 target_strategy;
};

struct BattleAiProfile *Func_08077198(s32 class_id);
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
            && BattleFx_Classify((action)->effect) == 0)                  \
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

s32 BattleTarget_SelectForAction(
    s32 actor_id,
    struct BattleAction *action)
{
    u16 order_positions[6];
    s32 unit_ids[6];
    s32 target_positions[6];
    struct BattleTurnOrder *turn_order;
    struct BattleUnit *unit;
    struct BattleUnit *next_unit;
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
    s32 applies;
    s32 damage_class;
    u32 roll;
    s16 *slot;

    turn_order = BATTLE_TURN_ORDER;
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
        while (turn_order->normal[target_index] != 255) {
            slot = &turn_order->normal[target_index];
            unit_id = *slot;
            if (unit_id != 254) {
                if (action->target_mode != 4 || unit_id == actor_id) {
                    unit_ids[candidate_count] = unit_id;
                    order_positions[candidate_count] =
                        target_index | 0x100;
                    candidate_count++;
                }
            }
            target_index++;
        }
        goto scan_complete;

scan_mirrored_order:
        target_index = 0;
        while (turn_order->mirrored[target_index] != 255) {
            unit_id = turn_order->mirrored[target_index];
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
        unit = BattleUnit_Get(unit_ids[target_index]);
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
        && Func_08077198(BattleUnit_Get(actor_id)->class_id)
                ->target_strategy != 2
        && (u32)((action->target_flags & 0x0f) - 3) <= 2) {
        selected = -1;
        for (target_index = 0;
             target_index < target_count;
             target_index++) {
            for (inner_index = target_index;
                 inner_index < target_count - 1;
                 inner_index++) {
                unit = BattleUnit_Get(unit_ids[inner_index]);
                next_unit = BattleUnit_Get(unit_ids[inner_index + 1]);
                if (Func_08077198(BattleUnit_Get(actor_id)->class_id)
                        ->target_strategy == 0) {
                    value = unit->hp;
                    next_value = next_unit->hp;
                } else {
                    value = unit->max_hp;
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

    return target_positions[(u32)(target_count * Random16()) >> 16];
}
