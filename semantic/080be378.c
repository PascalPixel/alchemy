#include "battle_command.h"

/*
 * One battle-command function spans 080be378..080bf1d8.  Its manifest rows
 * share one 48-byte frame and epilogue; inter-row `bl` edges are local control
 * flow, while 080bea9c, 080bef58, and 080bf1e8 are literal pools.
 */

s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080030f8(u32 frames);
void Func_08009080(u32 handle, s32 mode);
void Func_08009088(u32 handle, s32 value);
void Func_08015118(void);
void Func_08015120(s32 value, s32 slot);
void Func_080151c8(s32 message_id);
struct BattlePendingActions *Func_08077000(s32 side);
struct ItemDefinition *Func_08077018(u32 item);
s32 Func_08077078(struct BattleUnit *unit, s32 mode);
s32 Func_08077118(s32 action_id, u32 mask);
s32 Func_08077160(struct BattleUnit *unit);
s32 Func_08077170(s32 unit);
s8 Func_08077178(s32 actor, s32 member, s32 kind, s32 effect, s32 scale);
void Func_080771b0(s32 unit, s32 row, s32 column);
void Func_080771c0(s32 unit, s32 row, s32 column);
void Func_080771c8(s32 unit, s32 row, s32 column);
struct BattleSummonDefinition *Func_080771e0(s32 slot);
s32 Func_080771e8(s32 row, s32 column);
s32 Func_08077208(s32 unit, s32 row, s32 column);
s32 Func_08077210(s32 unit, s32 row, s32 column);
s32 Func_080772f8(struct BattleUnit *unit);
struct BattleUnit **Func_080b7dd0(s32 unit);
void *Func_080b9a44(s32 value);
void Func_080bb65c(void);
void Func_080bb8d8(void);
void Func_080bb938(void);
s32 Func_080bd3c8(s32 action_id);
void Func_080bd808(s32 frames);
void Func_080bdfec(void);
void Func_080be02c(void);
s32 Func_080be070(s32 object_id);
s32 Func_080be0b4(u32 object_id, u8 *element_counts);
void Func_080c10e8(s32 mode, s32 value);
void Func_080c1798(s32 unit, s32 row, s32 mode, s32 value);
u32 Func_080c23e8(s32 class_index);
void Func_080f9010(s32 sound_id);

/*
 * The ROM passes context_end in r9.  The explicit trailing argument preserves
 * that hidden input until a compiler route can reproduce the register ABI.
 */
s32 Func_080be18c(s32 action_id, void *const *context_end);

s32 Func_080be378(struct BattleCommandRequest *request,
                  struct BattlePlan *plan)
{
    /* Func_080be18c reads this five-word context backwards from context_end. */
    void *context[5];
    void *const *context_end = &context[5];

    struct BattleUnit *unit;
    struct BattleUnit *target_unit = 0;
    struct BattleAction *action = 0;
    struct BattleAction *action_cost;
    struct ItemDefinition *item;
    struct BattleSummonDefinition *summon;
    struct BattlePendingActions *pending;
    struct BattlePendingAction *entry;
    u8 element_counts[4];
    s32 action_id = 1;
    s32 message_id = 0;
    u32 index;
    s32 pending_count;
    s32 value;
    s32 random_threshold;
    s32 adjustment_step;

    unit = BattleUnit_Get(request->actor_id);

    context[0] = unit;
    context[1] = request;
    context[2] = BATTLE_TURN_ORDER;
    context[3] = Func_080b9a44(request->unknown_0a);
    context[4] = plan;

    Func_080bdfec();

    plan->actor_id = (u8)request->actor_id;
    plan->pending_amount_60 = 0;
    plan->target_count = 0;
    plan->presentation_flags = 0;
    plan->failure = 0;
    plan->range_index = 4;

    Func_08015118();

    if (unit->hp == 0) {
        return -2;
    }

    /*
     * Auto-battle sweep.  With the auto flag live and option 0x16d set, walk
     * the published turn order and re-queue every entry the action filter does
     * not already reject.  Bit 2 of the option word selects the mirrored list.
     */
    if (BATTLE_AUTO_MODE != 0 && BattleFlag_Test(0x16d) != 0 &&
        (BATTLE_OPTIONS & 0x100) != 0) {
        s32 use_mirror = (BATTLE_OPTIONS & 4) != 0;
        s32 offset = 100;
        s32 order_id;
        u8 *order_base = (u8 *)context[2];

        for (;;) {
            if (use_mirror) {
                order_id = *(s16 *)(order_base + 2 + offset);
            } else {
                order_id = *(s16 *)(order_base + offset - 12);
            }
            if (order_id == 255) {
                break;
            }
            if (order_id != 254 &&
                Func_08077118(order_id, 0xc0000000) == 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, order_id);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, order_id);
            }
            offset += 2;
        }

        Func_080bb938();
        return -2;
    }

    Func_08015118();

    unit = context[0];

    /* A queued forced action consumes itself and aborts the manual command. */
    if (unit->forced_action != 0) {
        unit->forced_action = 0;
        Func_08015120(request->actor_id, 1);
        Func_080151c8(0x880);
        return -1;
    }

    if (unit->status_13c != 0) {
        Func_08015120(request->actor_id, 1);
        Func_080151c8(0x858);
        return -1;
    }

    if (unit->status_13b != 0) {
        Func_08015120(request->actor_id, 1);
        Func_080151c8(0x857);
        return -1;
    }

    /*
     * Restrained units may only defend (command 3); the restraint lapses on
     * three quarters of the frames.
     */
    if ((unit->restraint & 1) != 0 && request->command != 3 &&
        (BattleRandom_Next() & 3) == 0) {
        Func_08015120(request->actor_id, 1);
        Func_080151c8(0x859);
        return -1;
    }

    if (request->command == 8) {
        return -2;
    }

    for (index = 0; index < BATTLE_TARGET_CAPACITY; index++) {
        plan->target_modifiers[index] = 0;
    }
    for (index = 0; index < BATTLE_TARGET_CAPACITY; index++) {
        plan->target_results[index] = -1;
    }

    /*
     * The original indexes a 100-entry jump table with the command and calls
     * the out-of-range handler (the 080bee00 tail) for anything above 99.
     * Entries 9..98 all point at that same tail.
     */
    if ((u32)request->command > 99) {
        goto action_selected;
    }

    switch (request->command) {
    case 0:
        /* Attack. */
        unit = context[0];
        action_id = Func_08077160(unit);
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        if (action_id != 1) {
            goto resolve_default_action;
        }
        Func_08015120(request->actor_id, 1);
        value = Func_08077078(unit, 1);
        Func_08015120(value, 2);
        Func_080151c8(0x819);
        Func_080bb8d8();
        Func_08015120(action_id, 4);
        message_id = 0x81a;
        goto emit_message;

    case 1:
        /* Use Psynergy. */
        action_id = request->parameter;
        action_cost = BattleAction_Get(action_id);
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor_id, 1);
        Func_08015120(action_id, 4);
        Func_080151c8(0x83e);

        unit = context[0];
        value = 1;
        if (unit->pp < (s16)action_cost->pp_cost) {
            plan->failure = 2;
            value = 0;
        }
        if (unit->status_13d != 0) {
            plan->failure = 1;
            value = 0;
        }
        if (value == 0) {
            goto action_selected;
        }

        plan->failure = 0;
        unit = context[0];
        unit->pp = (s16)((u16)unit->pp - action_cost->pp_cost);
        BattleUnit_UpdateRatios(request->actor_id);
        if (unit->pp < 0) {
            unit->pp = 0;
        }
        if (unit->pp > unit->max_pp) {
            unit->pp = unit->max_pp;
        }
        goto action_selected;

    case 2:
        /* Use an item. */
        if (request->parameter < 0) {
            Func_08015120(request->actor_id, 1);
            Func_080151c8(0x81b);
            return -1;
        }

        unit = context[0];
        item = Func_08077018(unit->inventory[request->parameter]);
        action_id = item->action_id;

        /*
         * An item is usable when it publishes an action and its slot is not
         * marked broken/equipped-locked (bit 0x400 of the inventory word).
         */
        if (action_id == 0 ||
            (((struct BattleUnit *)context[0])
                 ->inventory[request->parameter] & 0x400) != 0) {
            Func_08015120(request->actor_id, 1);
            Func_080151c8(0x816);
            unit = context[0];
            if ((s8)unit->guard_level != 0) {
                return -1;
            }
            /* An unusable action falls back to the first guard level. */
            unit->guard_level = 1;
            return -1;
        }

        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor_id, 1);
        unit = context[0];
        Func_08015120(unit->inventory[request->parameter], 2);

        message_id = 0x817;
        if (item->usability == 2 || item->usability == 0) {
            if (item->type == 3 || item->type == 1 ||
                (item->type >= 6 && item->type <= 8)) {
                message_id = 0x818;
            }
        }
        goto emit_message;

    case 3:
    case 7:
        /* Defend and the alias of it that shares the jump-table slot. */
        Func_08015120(request->actor_id, 1);
        Func_080151c8(0x816);
        return -1;

    case 4:
        /* Use a Djinn. */
        action_id = request->parameter;
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor_id, 1);
        Func_08015120(action_id, 4);

        action = BattleAction_Get(action_id);
        message_id = ((action->target_flags & 15) == 6) ? 0x8f1 : 0x8f0;

        /*
         * Per-Djinni announcement.  The original is a comparison tree
         * over the action id; the ranges below reproduce it exactly.
         */
        if (action_id == 224) {
            message_id = 0x83e;
        } else if (action_id >= 435 && action_id <= 436) {
            message_id = 0x8f2;
        } else if (action_id >= 437 && action_id <= 441) {
            message_id = 0x8fb;
        } else if (action_id >= 442 && action_id <= 444) {
            message_id = 0x8f0;
        } else if (action_id == 472) {
            message_id = 0x8fc;
        } else if (action_id == 488) {
            message_id = 0x8fd;
        } else {
            switch (action_id) {
            case 492: message_id = 0x8ff; break;
            case 494: message_id = 0x8fa; break;
            case 495: message_id = 0x8fe; break;
            case 499: message_id = 0x8f9; break;
            case 500: message_id = 0x8f7; break;
            case 501: message_id = 0x8f8; break;
            case 503: message_id = 0x900; break;
            case 504: message_id = 0x901; break;
            case 508: message_id = 0x902; break;
            default:  break;
            }
        }
        goto emit_message;

    case 5: {
        /* Change position or leave the encounter. */
        s32 row = ((s32)((u32)request->parameter << 16) >> 24) & 15;
        s32 column = request->parameter & 255;

        action_id = Func_080771e8(row, column);

        if (Func_08077208(request->actor_id, row, column) != 0) {
            /* The destination is already reachable; treat it as an action. */
            if (Func_080be18c(action_id, context_end) == -1) {
                return -1;
            }
            Func_080771c8(request->actor_id, row, column);
            action = BattleAction_Get(action_id);
            Func_08015120(request->actor_id, 1);
            Func_08015120(action_id, 4);
            Func_080151c8(0x83f);
            plan->range_index = action->damage_class;
            goto action_selected;
        }

        if (Func_08077210(request->actor_id, row, column) == 0) {
            Func_08015120(request->actor_id, 1);
            Func_08015120(action_id, 4);
            Func_080f9010(114);
            Func_080151c8(0x85b);
            Func_080030f8(60);
            return -1;
        }

        /* Leaving the encounter: tear the battle scene down and unwind. */
        BattleAction_Get(action_id);
        Func_080c10e8(0, 0);
        Func_080771b0(request->actor_id, row, column);
        Func_080771c0(request->actor_id, row, column);
        BattleUnit_Recalculate(request->actor_id);
        Func_080bdfec();
        Func_080bd808(30);
        BattleEvent_Push(BATTLE_EVENT_UNIT, request->actor_id);
        BattleEvent_Push(
            BATTLE_EVENT_ACTION,
            ((row * 5) << 2) + column + 300);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        BattleEvent_Push(BATTLE_EVENT_REFRESH, 0);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x897);
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, request->actor_id);
        Func_080f9010(212);
        Func_08009080(*(u32 *)Func_080b7dd0(request->actor_id), 3);
        Func_08009088(*(u32 *)Func_080b7dd0(request->actor_id), 32);
        Func_080c1798(request->actor_id, row, 3, 0);
        Func_080be02c();
        return -2;
    }

    case 6: {
        /* Summon. */
        s32 matched = 0;

        summon = Func_080771e0(request->parameter);
        Func_080be0b4(request->actor_id, element_counts);

        pending = Func_08077000((u16)request->actor_id > 7 ? 1 : 0);
        entry = pending->entries;

        /*
         * Pay the summon's per-element requirement.  The loop stops at the
         * first element that is short, so a partial match leaves the already
         * overwritten counters behind exactly as the original does.
         */
        if (element_counts[0] >= summon->element_costs[0]) {
            index = 0;
            for (;;) {
                element_counts[index] = summon->element_costs[index];
                matched++;
                if (matched > 3) {
                    break;
                }
                index++;
                if (element_counts[index] < summon->element_costs[index]) {
                    break;
                }
            }
        }

        action_id = summon->action_id;
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }

        if (matched != 4) {
            Func_08015120(request->actor_id, 1);
            Func_08015120(action_id, 4);
            Func_080151c8(0x842);
            return -1;
        }

        Func_08015120(request->actor_id, 1);
        Func_08015120(action_id, 4);
        Func_080151c8(0x841);

        /* Reserve the pending djinn that fund the summon. */
        pending_count = pending->entry_count;
        for (index = 0; index < pending_count; index++) {
            entry = &pending->entries[index];
            if (entry->state == -1 && Func_080be070(entry->object_id) != 0) {
                u8 remaining = element_counts[entry->element];

                if (remaining != 0) {
                    entry->state = (s8)254;
                    element_counts[entry->element] = (u8)(remaining - 1);
                }
            }
            pending_count = pending->entry_count;
        }
        goto action_selected;
    }

    case 8:
        return -2;

    case 99:
        /* Flee. */
        if ((u16)request->actor_id <= 7) {
            Func_080151c8(0x843);
        } else {
            Func_08015120(request->actor_id, 1);
            Func_080151c8(0x846);
        }
        Func_080bb65c();
        plan->outcome = 7;
        return 0;

    default:
        break;
    }

    goto action_selected;

emit_message:
    Func_080151c8(message_id);

action_selected:
    /* 080bee00 */
    if (action_id != 1) {
        goto resolve_default_action;
    }

    /* 080bee08: the plain attack path. */
    plan = context[4];
    target_unit = BattleUnit_Get(plan->target_ids[0]);
    plan->action_id = 1;
    plan->range_index = Func_08077170(request->actor_id);
    plan->outcome = 2;

    unit = context[0];
    if (unit->class_bonus_disabled == 0) {
        plan->presentation_flags = Func_080c23e8(unit->class_id) | 0x4000;
    } else {
        plan->presentation_flags = 0;
        if (unit->class_id <= 5) {
            switch (unit->class_id) {
            case 0:
            case 5:
                plan->presentation_flags = 0x4001;
                break;
            case 1:
                plan->presentation_flags = 0x4001;
                break;
            case 2:
            case 3:
                plan->presentation_flags = 0x4004;
                break;
            default:
                /* class 4 leaves the flags at zero */
                break;
            }
        }
    }

    Func_08015120(request->actor_id, 1);
    Func_080151c8(0x814);

    /*
     * Auto-battle delay tuning.  Only a live, unafflicted, auto-flagged target
     * enters the adjustment; note that the adjustment is only reached when the
     * auto flag itself reads zero, so both stores below write zero.  That is
     * what the original does and it is preserved rather than folded away.
     */
    if (target_unit->hp != 0 && target_unit->status_13c == 0 &&
        target_unit->status_13b == 0 && target_unit->forced_action == 0) {
        u8 auto_flag = target_unit->status_13a;

        if (auto_flag == 0) {
            unit = context[0];
            if (unit->status_138 != 0 &&
                (BattleRandom_Next() & 255) <= 152) {
                plan->target_adjustments[0] = auto_flag;
            }
            if ((BattleRandom_Next() & 31) == 0) {
                plan->target_adjustments[0] = 0;
            }
        }
    }

    if (BattleFlag_Test(366) != 0) {
        plan->target_adjustments[0] = 0;
    }

    if (target_unit->hp == 0) {
        goto finish;
    }

    if ((BattleRandom_Next() & 31) != 0) {
        /*
         * 080bef88: rare stumble check.  The per-unit rate is scaled into the
         * 16-bit random range before comparing.
         */
        value = Func_080022ec((s32)((u32)Func_080772f8(context[0]) << 16), 200);
        if (value > (BattleRandom_Next() & 0xffff)) {
            plan->target_modifiers[0] = 1;
        }
        goto finish;
    }

    plan->target_modifiers[0] = 1;
    goto finish;

resolve_default_action:
    /* 080befb4: everything that is not the plain attack. */
    action = BattleAction_Get(action_id);
    plan->range_index = action->damage_class;
    plan->presentation_flags = 0;
    plan->action_id = action_id;

    switch (action->effect) {
    case 41:
    case 42:
    case 43:
    case 44:
    case 65:
    case 68:
        if (action->effect == 65 || action->effect == 68) {
            random_threshold = 153;
        } else if (action->effect == 41 || action->effect == 43) {
            random_threshold = 32;
        } else {
            random_threshold = 64;
        }

        if (action->effect == 65 || action->effect == 41 ||
            action->effect == 42) {
            adjustment_step = 1;
        } else {
            adjustment_step = 2;
        }

        if ((BattleRandom_Next() & 255) < random_threshold) {
            for (index = 0; index < plan->target_count; index++) {
                plan->target_adjustments[index] =
                    (u8)(plan->target_adjustments[index] + adjustment_step);
            }
        }
        break;

    default:
        if ((u8)(action->effect - 36) <= 4) {
            switch (action->effect - 36) {
            case 0: random_threshold = 63; break;
            case 1: random_threshold = 31; break;
            case 2: random_threshold = 15; break;
            case 3: random_threshold = 7; break;
            default: random_threshold = 3; break;
            }
            if ((BattleRandom_Next() & random_threshold) == 0) {
                for (index = 0; index < plan->target_count; index++) {
                    plan->target_modifiers[index] = 2;
                }
            }
        } else if (action_id == 178) {
            for (index = 0; index < plan->target_count; index++) {
                plan->target_results[index] =
                    Func_08077178(request->actor_id, plan->target_ids[index],
                                  action->damage_class, action->effect, 100);
            }
        }
        break;
    }

    /* 080bf0f8 */
    if (action_id <= 0x206) {
        plan->presentation_flags = BATTLE_ACTION_FLAGS[action_id];
        if ((s8)plan->target_adjustments[0] > 1) {
            plan->presentation_flags = BATTLE_ACTION_FLAGS[action_id] +
                ((s8)plan->target_adjustments[0] << 12) - 0x1000;
        }
    }

    if (action_id <= 0x205 && BATTLE_ACTION_STATUS[action_id] != 0) {
        plan->outcome = BATTLE_ACTION_STATUS[action_id];
    } else if (Func_080bd3c8(action_id) != 0) {
        plan->outcome = 3;
    } else if (plan->presentation_flags != 0) {
        unit = context[0];
        plan->outcome = (unit->class_bonus_disabled == 0) ? 8 : 3;
    } else {
        plan->outcome = 1;
    }

    if (BattleEffect_Classify(action->effect) != 0) {
        plan->presentation_flags |= 0x10000;
    }

    if (action_id == 178 && (s8)plan->target_results[0] != 0) {
        plan->presentation_flags |= 0x1000;
    }

finish:
    /* 080bf1a8 */
    if (request->command == 2) {
        if (plan->outcome != 5 && plan->outcome != 9) {
            plan->outcome = 4;
        }
    }

    plan->command = (u16)request->command;
    return 0;
}
