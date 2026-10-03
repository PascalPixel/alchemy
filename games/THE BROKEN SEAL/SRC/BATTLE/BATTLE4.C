#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "SYSTEM.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_WORK.H"
#include "MOTION_OBJECT.H"
#include "CHARACTER.H"
#include "ANIMSPR.H"

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
void Map_RenderAllAnimatedTileFramesFar(void **, s32);

/* The native scan reads the session through its original halfword view. */
struct SlotArray { s16 items[64]; };

/* Takes a defeated unit out of the party or enemy list, leaving the removed
 * mark in its place, and cancels the actions queued for it. */
void BattleActor_RemoveFromLists(s32 actor)
{
    struct BattleSession *work;
    s32 i;
    u32 j;
    s32 unit;

    work = gBattleWork;
    Owner_GetStateFar(actor)->status_12a = BATTLE_UNIT_ABSENT;
    for (i = 0; ; i++) {
        if (work->party_units[i] == actor) {
            work->party_units[i] = BATTLE_UNIT_REMOVED;
            goto removed;
        }
        if (work->party_units[i] == BATTLE_UNIT_LIST_END)
            break;
    }
    /* FAKEMATCH: the enemy scan is a goto loop inside a block that runs once,
     * which keeps the loop pass off it. Written as a for like the party scan
     * it compiles to a pointer walk with the 0xfe held in a register. */
    do {
        j = 0;
again:
        unit = work->enemy_units[j];
        if (unit == actor) {
            work->enemy_units[j] = BATTLE_UNIT_REMOVED;
            goto removed;
        }
        j++;
        if (unit == BATTLE_UNIT_LIST_END)
            return;
        goto again;
    } while (0);
removed:
    Summon_ReleaseCharge(actor);
    for (j = 0; j < 20; j++) {
        if (work->actions[j].unit_id == actor)
            work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
    }
}

void BattleMotion_InitializeActorRecords(s32 id)
{
    void *items[4];
    struct BattleUnit *state;
    struct AnimationObject *item;
    struct AnimationEntry *child;
    s32 index;

    state = Owner_GetStateFar(id);
    index = 0;
    while ((item = GetMotionRecord(GetBattleObjectSlot(id)->object, index)) != 0) {
        if (state->status_12a != BATTLE_UNIT_ENEMY)
            AnimationObjects_SelectAnimationFar(item, 4);
        else
            AnimationObjects_SelectAnimationFar(item, 5);
        index++;
    }

    if (state->status_12a == BATTLE_UNIT_ENEMY) {
        index = 0;
        while ((item = GetMotionRecord(GetBattleObjectSlot(id)->object, index)) != 0) {
            child = item->entries[0];
            items[index] = item;
            child->param = 6;
            child->frame = BATTLE_UNIT_LIST_END;
            index++;
        }
        WaitFrames(4);
        BattleActor_RemoveFromLists(id);
        Map_RenderAllAnimatedTileFramesFar(items, index);
        ActivateBattleObjectSlot(id);
    }
}

s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{
    /* The named party/enemy arrays changed this scan's instruction order
       and extent; retain its existing halfword view. */
    u16 positions[6];
    struct SlotArray *order;
    s16 *entry;
    u16 *cursor;
    s32 value;
    s32 count;
    s32 index;
    s32 slot;
    s32 tail;
    s32 offset;

    count = 0;
    order = (struct SlotArray *)gBattleWork;

    if (require_living_unit != 0) {
        for (;;) {
            index = 0;
            slot = (u32)&((struct BattleSession *)0)->party_units / sizeof(s16);
            if (order->items[slot] != 255) {
                entry = order->items;
                do {
                    value = entry[slot];
                    if (value != 254) {
                        if (Owner_GetStateFar(value)->hp != 0) {
                            positions[count] = index | 0x100;
                            count++;
                        }
                    }
                    slot++;
                    index++;
                } while (entry[slot] != 255);
            }
            goto pick;
        }
    } else {
        index = 0;
        slot = (u32)&((struct BattleSession *)0)->enemy_units / sizeof(s16) - 1;
        tail = (u32)&((struct BattleSession *)0)->enemy_units / sizeof(s16) - 1;
        offset = tail * 2;
        entry = (s16 *)(order->items + 1);
        if (*(s16 *)((char *)entry + offset) != 255) {
            cursor = (u16 *)entry;
            do {
                if ((s16)cursor[slot] != 254) {
                    positions[count] = index | 0x180;
                    count++;
                }
                slot++;
                tail++;
                index++;
            } while (entry[tail] != 255);
        }
    }

pick:
    if (count == 0)
        return 0;
    return positions[(u32)(Random16() * count) >> 16];
}

s32 BattleFx_IsReviveFar(s32 effect);

struct EnemyDefinition *Owner_GetRecordFar(s32 class_id);

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
        if ((unit)->element_modifier[0] > 0)                                   \
            (count)++;                                                        \
        if ((unit)->element_modifier[1] > 0)                                   \
            (count)++;                                                        \
        if ((unit)->element_modifier[2] > 0)                                   \
            (count)++;                                                        \
        if ((unit)->element_modifier[3] > 0)                                   \
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
            case 7:                                                           \
            case 8:                                                           \
                if ((action)->power != 0 && (unit)->hp != 0)                  \
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
                if ((action)->power != 0 && (unit)->hp != 0)                  \
                    (applies)++;                                              \
                break;                                                        \
            }                                                                 \
        }                                                                     \
    }

/* Picks the target of ACTOR_ID's action and returns its order position:
   gathers the units on the side the target mode names, keeps those the
   effect can still change (or, when it changes nothing, those its damage
   class can reach), and picks one at random. A single-target action of
   damage class 3 to 5 from a unit whose AI strategy is not 2 first sorts
   them by current HP (strategy 0) or maximum HP and weights the pick toward
   the first of up to four. -2: no unit on that side; -1: none qualifies. */
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
                    order_positions[candidate_count] = 0x100;
                    order_positions[candidate_count] |= target_index;
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
                    order_positions[candidate_count] = 0x180;
                    order_positions[candidate_count] |= target_index;
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
