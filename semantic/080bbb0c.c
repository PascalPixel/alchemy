#include "battle_command.h"
#include "motion_object.h"

/* exact/080d40ec.c and exact/080e0524.c witness this IWRAM copy ABI. */
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_080022f4(s32, s32);
void Func_08002df0(void *);
void *Func_08004938(s32);
void Func_08015130(s32);
void Func_08077120(s32, s32);
void Func_08077140(s32, s32, s32);
s32 Func_08077178(s32, s32, s32, s32, s32);
s32 Func_08077180(u16, u32, u16, s32);
s32 Func_08077188(s32, s32, s32);
s32 Func_08077190(s32, s32, s32);
s32 Func_080b6ae0(u16 *);
void Func_080b6c90(void);
s32 Func_080b6cdc(s32);
void Func_080b6f44(void *, s32, s32, s32);
s32 Func_080b7514(void);
void Func_080b7548(void);
struct BattleObjectSlot *Func_080b7dd0(s32);
void Func_080b8000(s32);
s32 Func_080bbae8(s32);
s32 Func_080c1df4(s32, s32);
s32 Func_080c1f50(s32);
u32 Func_080c1fa8(s32);

/*
 * Resolve one target from a queued battle action.  The plan identifies the
 * acting combatant, action, target list, range slot, and per-target modifiers.
 * The routine applies the action's HP/PP/stat/status effect to the live target,
 * queues the corresponding battle messages, and then performs common cleanup.
 *
 * The original owner also contained two internal branch-with-link
 * continuations at 0x080bd236 and 0x080bd2c0.  They are represented below as
 * direct message emission followed by `goto finalize`; they are not separate
 * ABI-callable functions.
 */
s32 Func_080bbb0c(struct BattlePlan *input_plan, s32 target_slot) {
    u16 queued_ids[7];
    struct BattleAction *action;
    u8 *actor_state;
    s32 actor_id;
    s32 action_id;
    s32 stat_delta;
    u8 *battle_state;
    s32 halve_defense;
    s32 target_adjustment;
    u32 effect_amount;
    s32 leave_one_hp;
    s32 action_allowed;
    s32 target_modifier;
    s32 skip_primary_effect;
    s32 action_family;
    s32 range_relation;
    struct BattleUnit *target_snapshot;
    s32 source_stat;
    s32 power_bonus_percent;
    s16 *command;
    struct BattlePlan *plan;
    u8 *target_ids;
    u16 *queued_object_cursor;
    s16 range_value;
    s16 actor_pp_before_drain;
    s32 turn_order_entry;
    s16 target_max_pp;
    s16 actor_max_hp;
    s16 actor_max_pp;
    s16 target_pp_available;
    s16 target_max_hp;
    s16 actor_pp;
    s16 target_hp;
    s16 actor_hp;
    s16 target_hp_before_heal;
    s16 target_pp_before_damage;
    s16 target_hp_before_ranged_damage;
    s16 target_pp_before_recovery;
    s16 target_hp_before_effect;
    u8 *target_state;
    struct BattleUnit *target_unit;
    s32 object_effect_config;
    s32 queued_object_count;
    s32 queued_id;
    s32 hp_minus_one;
    s32 range_check_score;
    s32 healing_power;
    s32 effect_object_id;
    s32 range_index;
    s32 turn_order_tail_offset;
    s32 turn_order_entry_offset;
    s32 turn_order_offset;
    s32 turn_order_next_offset;
    s32 result;
    s32 base_damage;
    s32 modifier_numerator;
    s32 recovery_percent_product;
    s32 range_lower_count;
    s32 recovery_source_stat;
    s32 range_upper_count;
    s32 actor_adjustment_offset;
    s32 target_adjustment_offset;
    s32 battle_record_offset;
    s32 queue_head;
    s32 range_scale;
    s32 prior_health;
    s32 restored_pp;
    s32 healing_source_stat;
    s32 object_anchor_x;
    s32 object_anchor_z;
    s32 range_scale_table;
    s32 scratch_value;
    s32 range_scale_offset;
    u8 status_mask;
    s32 scaled_ranged_damage;
    s32 effect_damage;
    s32 recovered_hp;
    s32 actor_recovered_pp;
    s32 drained_pp;
    s32 turn_order_index;
    s32 queued_object_remaining;
    s32 adjusted_damage;
    s32 modified_damage;
    s32 resulting_hp;
    s32 resulting_pp;
    s32 secondary_resulting_pp;
    s32 actor_recovered_hp;
    s32 damage_pass;
    s32 ranged_damage_pass;
    u8 *status_flags;
    s8 *status_level_one;
    s8 *status_level_two;
    s8 effect_guard_level;
    s8 guard_level;
    s8 pp_guard_level;
    s8 secondary_pp_guard_level;
    s32 range_distance;
    s32 cached_action_result;
    u8 *allowed_table;
    s32 *recovery_table;
    s32 cached_result_offset;
    s32 result_slot_offset;
    u32 target_defense;
    u32 max_hp_for_half_revive;
    s32 action_power;
    u32 target_max_hp_unsigned;
    u32 target_hp_before_percent_heal;
    s32 effective_defense;
    s32 scaled_action_power;
    u32 range_index_unsigned;
    u32 modifier_product;
    u32 ranged_damage_text;
    u32 effect_damage_text;
    u32 recovered_hp_amount;
    u32 defeat_effect_text;
    u32 effect_text;
    u32 continuation_text;
    u32 damage_text;
    u32 defeat_text;
    u32 pp_damage_text;
    u32 secondary_pp_damage_text;
    u32 battle_record_index;
    u32 ranged_damage;
    u32 pp_recovery;
    u32 actor_hp_recovery;
    u32 damage;
    u32 pp_damage;
    u32 healed_hp;
    u32 secondary_pp_damage;
    u32 actor_pp_recovery;
    u8 recovery_roll;
    s32 attack_down_one;
    s32 attack_down_two;
    s32 attack_up_one;
    s32 attack_up_two;
    s32 defense_down_one;
    s32 defense_down_two;
    s32 defense_up_one;
    s32 defense_up_two;
    s32 resistance_down_one;
    s32 resistance_down_two;
    s32 resistance_up_one;
    s32 resistance_up_two;
    u8 status_141_value;
    u8 status_13c_value;
    u8 target_id;
    s8 applied_resistance_down_two;
    u8 applied_resistance_up_one;
    u8 applied_resistance_up_two;
    s32 class_id;
    struct BattleObjectSlot *object_slot;
    u8 *turn_order_base;
    u8 *turn_order_scan_base;
    s32 turn_order_tail_slot;
    u8 *range_table;
    u8 *lower_threshold;
    s16 *upper_threshold;
    u32 status_140_address;

    plan = input_plan;
    stat_delta = 0;
    battle_state = *(void **)0x03001E74;
    halve_defense = 0;
    effect_amount = 0;
    leave_one_hp = 0;
    skip_primary_effect = 0;
    range_relation = 0;
    target_snapshot = Func_08004938(BATTLE_UNIT_SIZE);
    actor_id = (s32) plan->actor_id;
    target_ids = plan->target_ids;
    target_id = *(target_ids + target_slot);
    action_id = plan->action_id;
    range_index = plan->range_index;
    target_adjustment = plan->target_adjustments[target_slot];
    target_modifier = plan->target_modifiers[target_slot];
    action = BattleAction_Get(action_id);
    actor_state = (u8 *)BattleUnit_Get(actor_id);
    target_state = (u8 *)BattleUnit_Get((s32) target_id);
    target_unit = (struct BattleUnit *)target_state;
    ((WordCopy)0x03001388)(target_snapshot, target_state, BATTLE_UNIT_SIZE);
    if (action->range != 0xFF) {
        range_distance = plan->target_offsets[target_slot];
        if (range_distance < 0) {
            range_distance = -range_distance;
        }
    } else {
        range_distance = 0;
    }
    if (range_index != 4) {
        range_value = M2C_FIELD(((range_index * 4) + target_state), s16, 0x26);
        range_lower_count = 0;
        range_table = target_state + 0x24;
        if ((s32) range_value >= (s32) M2C_FIELD(range_table, s16, 2)) {
            lower_threshold = range_table;
loop_7:
            range_lower_count += 1;
            lower_threshold += 4;
            if (range_lower_count <= 3) {
                if ((s32) range_value >= (s32) M2C_FIELD(lower_threshold, s16, 2)) {
                    goto loop_7;
                }
            }
        }
        if (range_lower_count == 4) {
            range_relation = -1;
        }
        range_upper_count = 0;
        lower_threshold = target_state;
        lower_threshold += 0x24;
        if ((s32) range_value <= (s32) M2C_FIELD(lower_threshold, s16, 2)) {
            for (;;) {
                range_upper_count += 1;
                if (range_upper_count > 3) {
                    break;
                }
                upper_threshold = (s16 *)(target_state + (range_upper_count * 4) + 0x24);
                lower_threshold = (u8 *)upper_threshold;
                if ((s32) range_value > (s32) M2C_FIELD(upper_threshold, s16, 2)) {
                    break;
                }
            }
        }
        if (range_upper_count == 4) {
            range_relation = 1;
        }
    }
    range_index_unsigned = (u32) plan->range_index;
    if (range_index_unsigned <= 3U) {
        command = &plan->command;
        if (plan->command != 2) {
            actor_adjustment_offset = (range_index_unsigned * 4) + 0x48;
            source_stat = M2C_FIELD(actor_state, s16, actor_adjustment_offset);
        } else {
            goto block_21;
        }
    } else {
        command = &plan->command;
block_21:
        source_stat = 0x64;
    }
    if ((*command == 5) && (range_index_unsigned <= 3U) && (range_relation > 0)) {
        target_adjustment_offset = (range_index_unsigned * 4) + 0x48;
        range_check_score = source_stat - M2C_FIELD(
            target_state + target_adjustment_offset,
            s16,
            2
        );
        range_check_score += 0x1E;
        range_check_score *= 0x28F;
        if (range_check_score > (BattleRandom_Next() & 0xFFFF)) {
            BattleEvent_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5U);
        }
    }
    action_family = 0xF & action->target_flags;
    /* Load plan->target_results[target_slot] through the live ID base. */
    result_slot_offset = target_slot + 0x38;
    cached_action_result = ((s8 *)target_ids)[result_slot_offset];
    if (cached_action_result == -1) {
        allowed_table = (u8 *)0x080C2AB8;
        cached_action_result = Func_08077178(
            actor_id,
            target_id,
            range_index,
            action->effect,
            allowed_table[range_distance]
        );
    }
    action_allowed = cached_action_result;
    cached_action_result = 0;
    if ((u32) ((action->effect + 0xCE) << 0x18) > 0x01000000U) {

    } else {
        class_id = M2C_FIELD(actor_state, u8, 0x128);
        effect_object_id = Func_080b7514();
        if (action->effect == 0x33) {
            class_id = Func_080c1fa8(M2C_FIELD(battle_state, s32, 0));
        }
        if ((action_allowed != 0) && (Func_080b6cdc(class_id) != 0) && (effect_object_id >= 0)) {
            object_effect_config = Func_080c1df4(class_id, 1);
            if (0x8000 & object_effect_config) {
                Func_080c1f50(class_id);
            }
            Func_08077140(effect_object_id, class_id, 0x7FFF & object_effect_config);
            turn_order_base = battle_state + 2;
            turn_order_entry_offset = 0x64;
            turn_order_index = 0;
            turn_order_tail_offset = 0;
            turn_order_next_offset = 0;
            queue_head = M2C_FIELD(turn_order_base, s16, turn_order_entry_offset);
            if (queue_head == 0xFE) {
                M2C_FIELD(turn_order_base, s16, turn_order_entry_offset) = (s16) effect_object_id;
            } else {
                turn_order_offset = 0x64;
                for (;;) {
                    turn_order_scan_base = battle_state + 2;
                    turn_order_entry = *(s16 *)(turn_order_offset + (s32) turn_order_scan_base);
                    if (turn_order_entry == 0xFF) {
                        turn_order_tail_slot = turn_order_tail_offset + 0x66;
                        M2C_FIELD(turn_order_base, s16, turn_order_offset) = (s16) effect_object_id;
                        M2C_FIELD(turn_order_base, s16, turn_order_tail_slot) = turn_order_entry;
                        break;
                    }
                    turn_order_index += 1;
                    turn_order_offset += 2;
                    turn_order_next_offset += 2;
                    if (turn_order_index > 5) {
                        break;
                    }
                    turn_order_tail_offset = turn_order_next_offset;
                    if (*(s16 *)(turn_order_offset + (s32) turn_order_base) != 0xFE) {
                        continue;
                    }
                    *(s16 *)(turn_order_offset + (s32) turn_order_base) = (s16) effect_object_id;
                    break;
                }
            }
            Func_080b7548();
            object_slot = Func_080b7dd0(effect_object_id);
            object_anchor_x = object_slot->anchor_x / 65536;
            object_anchor_z = object_slot->anchor_z / 65536;
            Func_080b6f44(object_slot, effect_object_id, object_anchor_x, object_anchor_z);
            Func_080b6c90();
            queued_object_count = Func_080b6ae0(queued_ids);
            if (queued_object_count > 0) {
                queued_object_cursor = queued_ids;
                queued_object_remaining = queued_object_count;
                do {
                    queued_id = *queued_object_cursor;
                    queued_object_remaining -= 1;
                    queued_object_cursor++;
                    Func_080b8000(queued_id);
                } while (queued_object_remaining != 0);
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) effect_object_id);
            if (action_id != 0x1F7) {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8F5U);
            } else {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8F3U);
            }
        } else if (action_id == 0x1F7) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8F4U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8F6U);
        }
    }
    if (action_allowed != 0) {
        if (action->effect == 0x35) {
            action_allowed = 0;
            battle_record_offset = 0x2EC;
            battle_record_index = 0;
            if (M2C_FIELD(battle_state, s16, battle_record_offset) == target_id) {
                action_allowed = 1;
            } else {
loop_63:
                battle_record_index += 1;
                if (battle_record_index <= 0x13U) {
                    battle_record_offset = (battle_record_index * 0x10) + 0x2EC;
                    if (M2C_FIELD(
                        battle_state,
                        s16,
                        battle_record_offset
                    ) == target_id) {
                        action_allowed = 1;
                    } else {
                        goto loop_63;
                    }
                }
            }
        } else {
            if (action->effect == 0x23) {
                halve_defense = 1;
            } else if (action->effect == 0x22) {
                leave_one_hp = 1;
            } else if (action->effect == 0x1B) {
                skip_primary_effect = 1;
            } else if (action->effect == 0x37) {
                if (M2C_FIELD(actor_state, s16, 0x38) != 0) {
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, (u32) actor_id);
                }
            } else if (action->effect == 0x20) {
                if (M2C_FIELD(target_state, s16, 0x3A) != 0) {
                    action_family = 0xA;
                } else {
                    action_allowed = 0;
                }
            }
        }
    }
    if (skip_primary_effect != 0) {

    } else if ((M2C_FIELD(target_state, s16, 0x38) == 0) && (BattleEffect_Classify(action->effect) == 0)) {

    } else if ((u32) (action_family + 1) > 0xCU) {

    } else {
        switch (action_family) {
        case 3:
        case 4:
            target_defense = M2C_FIELD(target_state, u16, 0x3E);
            target_hp = M2C_FIELD(target_state, s16, 0x38);
            effective_defense = target_defense;
            if (halve_defense != 0) {
                effective_defense = (u16) (target_defense >> 1);
            }
            damage_pass = 1;
loop_90:
            if (range_index != 4) {
                target_adjustment_offset = range_index * 4;
                target_adjustment_offset += 0x48;
                stat_delta = source_stat - M2C_FIELD(
                    target_state + target_adjustment_offset,
                    s16,
                    2
                );
            }
            if (damage_pass == 0) {
                stat_delta = 0;
            }
            action_power = action->power;
            if (action_family == 4) {
                base_damage = Func_080022ec(Func_08077180(M2C_FIELD(actor_state, u16, 0x3C), (u32) effective_defense, 0U, stat_delta) * action_power, 0xA);
            } else {
                base_damage = Func_08077180(M2C_FIELD(actor_state, u16, 0x3C), (u32) effective_defense, action_power, stat_delta);
            }
            adjusted_damage = target_adjustment * base_damage;
            if (target_modifier != 0) {
                if (target_modifier == 1) {
                    modifier_numerator = adjusted_damage * 5;
                    if (modifier_numerator < 0) {
                        modifier_numerator += 3;
                    }
                    modified_damage = modifier_numerator >> 2;
                } else {
                    modifier_product = adjusted_damage * 3;
                    modified_damage = (s32) (modifier_product + (modifier_product >> 0x1F)) >> 1;
                }
                adjusted_damage = modified_damage +
                    (u8) Func_080022f4(M2C_FIELD(target_state, u8, 0xF), 5) +
                    6;
                if (damage_pass == 0) {
                    BattleEvent_Push(BATTLE_EVENT_MARK, 0U);
                    continuation_text = 0x822;
                    if ((u32) target_id <= 7U) {
                        continuation_text = 0x823;
                    }
                    BattleEvent_Push(BATTLE_EVENT_TEXT_CONTINUE, continuation_text);
                }
            }
            damage = adjusted_damage + (3 & BattleRandom_Next());
            guard_level = (s8) M2C_FIELD(target_state, u8, 0x12B);
            if (guard_level != 0) {
                if (guard_level == 1) {
                    damage = (u32) ((s32) (damage + (damage >> 0x1F)) >> 1);
                } else {
                    damage = (u32) Func_080022ec((s32) damage, 0xA);
                }
            }
            if ((s32) damage <= 0) {
                damage = 1;
            }
            if (leave_one_hp != 0) {
                hp_minus_one = target_hp - 1;
                if ((s32) damage < hp_minus_one) {
                    damage = (u32) hp_minus_one;
                    if ((s32) damage <= 0) {
                        damage = 1;
                    }
                }
            }
            if ((BattleFlag_Test(0x16E) != 0) && (*command == 5) && ((s32) target_hp <= (s32) damage)) {
                damage = target_hp - 1;
            }
            damage_pass += 1;
            if (damage_pass <= 1) {
                goto loop_90;
            }
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            resulting_hp = target_hp - damage;
            BattleEvent_Push(BATTLE_EVENT_VALUE, damage);
            if (!((u32) target_id <= 7U)) {
                ranged_damage_text = range_relation + 0x831;
            } else {
                ranged_damage_text = range_relation + 0x834;
            }
            BattleEvent_Push(BATTLE_EVENT_TEXT, ranged_damage_text);
            if (resulting_hp > 0) {
                goto block_actor_finish;
            } else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                resulting_hp = 0;
                if ((u32) target_id <= 7U) {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x825U);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x824U);
                }
            }
            goto block_246;
        case 10:
            if (action->power == 0) {

            } else {
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        target_state + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                action_power = action->power;
                pp_damage = Func_08077188(action_power, stat_delta, 0x100);
                recovery_table = (s32 *)0x080C2AC0;
                cached_result_offset = range_distance * 4;
                pp_damage = Func_080022ec(
                    pp_damage * M2C_FIELD(recovery_table, s32, cached_result_offset),
                    0x64
                );
                pp_damage *= target_adjustment;
                pp_guard_level = (s8) M2C_FIELD(target_state, u8, 0x12B);
                if (pp_guard_level != 0) {
                    if (pp_guard_level == 1) {
                        pp_damage = (u32) ((s32) (pp_damage + (pp_damage >> 0x1F)) >> 1);
                    } else {
                        pp_damage = (u32) Func_080022ec((s32) pp_damage, 0xA);
                    }
                }
                if ((action->effect == 0x20) && ((s32) pp_damage > (s32) (M2C_FIELD(target_state, s16, 0x3A)))) {
                    pp_damage = (u32) (M2C_FIELD(target_state, s16, 0x3A));
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, pp_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (!((u32) target_id <= 7U)) {
                    pp_damage_text = 0x829;
                } else {
                    pp_damage_text = 0x82A;
                }
                resulting_pp = (M2C_FIELD(target_state, s16, 0x3A)) - pp_damage;
                BattleEvent_Push(BATTLE_EVENT_TEXT, pp_damage_text);
                if (resulting_pp <= 0) {
                    resulting_pp = 0;
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                effect_amount = M2C_FIELD(target_state, s16, 0x3A) - resulting_pp;
                M2C_FIELD(target_state, s16, 0x3A) = (s16) resulting_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 1:
            if (action->power == 0) {

            } else {
                target_hp_before_heal = M2C_FIELD(target_state, s16, 0x38);
                healing_source_stat = source_stat;
                healing_power = action->power;
                if (range_index == 4) {
                    healing_source_stat = 0x64;
                }
                healing_power = Func_08077190(healing_power, healing_source_stat, 0x100);
                recovery_table = (s32 *)0x080C2AD8;
                healing_power = Func_080022ec(healing_power * recovery_table[range_distance], 0x64);
                healing_power *= target_adjustment;
                healed_hp = healing_power + (3 & BattleRandom_Next());
                target_max_hp = M2C_FIELD(target_state, s16, 0x34);
                resulting_hp = target_hp_before_heal + healed_hp;
                if (resulting_hp > (s32) target_max_hp) {
                    resulting_hp = (s32) target_max_hp;
                    healed_hp = resulting_hp - M2C_FIELD(target_state, s16, 0x38);
                }
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (!(resulting_hp == M2C_FIELD(target_state, s16, 0x34))) {
                    BattleEvent_Push(BATTLE_EVENT_VALUE, healed_hp);
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
                }
                prior_health = M2C_FIELD(target_state, s16, 0x38);
                goto block_247;
            }
            break;
        case -1:
            if (action->power == 0) {

            } else {
                target_pp_before_damage = M2C_FIELD(target_state, s16, 0x3A);
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        target_state + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                secondary_pp_damage = Func_08077188(action->power, stat_delta, 0x100);
                secondary_pp_damage *= M2C_FIELD(
                    (u8 *)0x080C2AF0,
                    s32,
                    range_distance * 4
                );
                secondary_pp_damage = Func_080022ec(secondary_pp_damage, 0x64);
                secondary_pp_damage *= target_adjustment;
                secondary_pp_guard_level = (s8) M2C_FIELD(target_state, u8, 0x12B);
                if (secondary_pp_guard_level != 0) {
                    if (secondary_pp_guard_level == 1) {
                        secondary_pp_damage = (u32) ((s32) (secondary_pp_damage + (secondary_pp_damage >> 0x1F)) >> 1);
                    } else {
                        secondary_pp_damage = (u32) Func_080022ec((s32) secondary_pp_damage, 0xA);
                    }
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, secondary_pp_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (!((u32) target_id <= 7U)) {
                    secondary_pp_damage_text = 0x826;
                } else {
                    secondary_pp_damage_text = 0x827;
                }
                secondary_resulting_pp = target_pp_before_damage - secondary_pp_damage;
                BattleEvent_Push(BATTLE_EVENT_TEXT, secondary_pp_damage_text);
                if (secondary_resulting_pp <= 0) {
                    secondary_resulting_pp = 0;
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
block_226:
                M2C_FIELD(target_state, s16, 0x3A) = (s16) secondary_resulting_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 5:
        case 6:
        case 8:
            if (action->power == 0) {

            } else {
                ranged_damage_pass = 1;
                target_hp_before_ranged_damage = M2C_FIELD(target_state, s16, 0x38);
loop_175:
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        target_state + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                if (ranged_damage_pass == 0) {
                    stat_delta = 0;
                }
                scaled_action_power = action->power;
                if (*command == 6) {
                    switch (action_id) {
                    case 380:
                    case 386:
                    case 392:
                    case 398:
                        power_bonus_percent = 3;
                        break;
                    case 381:
                    case 387:
                    case 393:
                    case 399:
                        power_bonus_percent = 6;
                        break;
                    case 382:
                    case 388:
                    case 394:
                    case 400:
                        power_bonus_percent = 9;
                        break;
                    case 383:
                    case 389:
                    case 395:
                    case 401:
                        power_bonus_percent = 0xC;
                        break;
                    }
                    scaled_action_power += Func_080022ec(power_bonus_percent * M2C_FIELD(target_state, s16, 0x34), 0x64);
                }
                scaled_ranged_damage = Func_08077188(scaled_action_power, stat_delta, 0x100);
                scaled_ranged_damage *= target_adjustment;
                switch (action_family) {
                case 5:
                    range_scale_table = 0x080C2B08;
                    range_scale_offset = range_distance * 4;
                    goto block_196;
                case 8:
                    range_scale_table = 0x080C2B20;
                    range_scale_offset = range_distance * 4;
                    goto block_196;
                case 6:
                    range_scale_table = 0x080C2B38;
                    range_scale_offset = range_distance * 4;
block_196:
                    range_scale = M2C_FIELD((u8 *)range_scale_table, s32, range_scale_offset);
                    scaled_ranged_damage = Func_080022ec(
                        range_scale * scaled_ranged_damage,
                        0x64
                    );
                    break;
                }
                ranged_damage = scaled_ranged_damage + (3 & BattleRandom_Next());
                if (((s8) M2C_FIELD(target_state, u8, 0x12B)) != 0) {
                    if (((s8) M2C_FIELD(target_state, u8, 0x12B)) == 1) {
                        ranged_damage = (u32) ((s32) (ranged_damage + (ranged_damage >> 0x1F)) >> 1);
                    } else {
                        ranged_damage = (u32) Func_080022ec((s32) ranged_damage, 0xA);
                    }
                }
                if ((BattleFlag_Test(0x16E) != 0) && (*command == 6) && ((s32) target_hp_before_ranged_damage > (s32) ranged_damage)) {
                    ranged_damage = (u32) target_hp_before_ranged_damage;
                }
                ranged_damage_pass += 1;
                if (ranged_damage_pass <= 1) {
                    goto loop_175;
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, ranged_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    ranged_damage_text = range_relation + 0x834;
                } else {
                    ranged_damage_text = range_relation + 0x831;
                }
                resulting_hp = target_hp_before_ranged_damage - ranged_damage;
                BattleEvent_Push(BATTLE_EVENT_TEXT, ranged_damage_text);
                if (resulting_hp <= 0) {
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, (u32) target_id);
                    BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                    resulting_hp = 0;
                    if ((u32) target_id <= 7U) {
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x825U);
                    } else {
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x824U);
                    }
                } else {
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                }
                prior_health = M2C_FIELD(target_state, s16, 0x38);
                goto block_247;
            }
            break;
        case 11:
            if (action->power == 0) {

            } else {
                target_pp_before_recovery = M2C_FIELD(target_state, s16, 0x3A);
                recovery_source_stat = source_stat;
                pp_recovery = action->power;
                if (range_index == 4) {
                    recovery_source_stat = 0x64;
                }
                pp_recovery = Func_08077190(pp_recovery, recovery_source_stat, 0x100);
                recovery_table = (s32 *)0x080C2B50;
                cached_result_offset = range_distance * 4;
                pp_recovery = Func_080022ec(
                    pp_recovery * M2C_FIELD(recovery_table, s32, cached_result_offset),
                    0x64
                );
                pp_recovery *= target_adjustment;
                target_max_pp = M2C_FIELD(target_state, s16, 0x36);
                restored_pp = target_pp_before_recovery + pp_recovery;
                if (restored_pp > (s32) target_max_pp) {
                    restored_pp = (s32) target_max_pp;
                    pp_recovery = restored_pp - M2C_FIELD(target_state, s16, 0x3A);
                }
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (restored_pp == M2C_FIELD(target_state, s16, 0x36)) {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821U);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_VALUE, pp_recovery);
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81EU);
                }
                M2C_FIELD(target_state, s16, 0x3A) = (s16) restored_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 2:
            if (action_allowed == 0) {
                goto block_case2_failure;
            } else if (action->power == 0) {

            } else {
                target_hp_before_effect = M2C_FIELD(target_state, s16, 0x38);
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        target_state + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                effect_damage = Func_08077188(action->power, stat_delta, 0x100);
                effect_damage *= target_adjustment;
                effect_damage *= M2C_FIELD(
                    (u8 *)0x080C2B68,
                    s32,
                    range_distance * 4
                );
                effect_damage = Func_080022ec(effect_damage, 0x64);
                effect_guard_level = (s8) M2C_FIELD(target_state, u8, 0x12B);
                if (effect_guard_level != 0) {
                    if (!(effect_guard_level == 1)) {
                        effect_damage = Func_080022ec(effect_damage, 0xA);
                    } else {
                        effect_damage = (s32) (effect_damage + ((u32) effect_damage >> 0x1F)) >> 1;
                    }
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) effect_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if ((u32) target_id <= 7U) {
                    effect_damage_text = 0x827;
                } else {
                    effect_damage_text = 0x826;
                }
                resulting_hp = target_hp_before_effect - effect_damage;
                BattleEvent_Push(BATTLE_EVENT_TEXT, effect_damage_text);
                if (resulting_hp <= 0) {
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, (u32) target_id);
                    BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                    resulting_hp = 0;
                    if ((u32) target_id <= 7U) {
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x825U);
                    } else {
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x824U);
                    }
                } else {
block_actor_finish:
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                }
block_246:
                prior_health = M2C_FIELD(target_state, s16, 0x38);
block_247:
                effect_amount = prior_health - resulting_hp;
                M2C_FIELD(target_state, s16, 0x38) = (s16) resulting_hp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
block_case2_failure:
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x854U);
            break;
        }
    }
    BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
    if ((BattleEffect_Classify(action->effect) == 0) && (M2C_FIELD(target_state, s16, 0x38) == 0) && (Func_080bbae8((s32) action->effect) == 0)) {
        goto finalize;
    }
    if (action_allowed == 0) {
        goto finalize;
    }
    if ((u32) (action->effect - 3) > 0x42U) {
        goto finalize;
    }
    switch ((s32) action->effect - 3) {
    case 0x3D:
        if (M2C_FIELD(target_state, u8, 0x138) != 0) {
            M2C_FIELD(target_state, u8, 0x138) = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88BU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x13B) != 0) {
            M2C_FIELD(target_state, u8, 0x13B) = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88DU);
        }
        M2C_FIELD(target_state, u8, 0x13C) = 0U;
        if (M2C_FIELD(target_state, u8, 0x13D) != 0) {
            M2C_FIELD(target_state, u8, 0x13D) = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88CU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x141) != 0) {
            M2C_FIELD(target_state, u8, 0x141) = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (M2C_FIELD(target_state, u8, 0x140) != 0) {
            M2C_FIELD(target_state, u8, 0x140) = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88FU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884U);
            M2C_FIELD(target_state, s8, 0x131) = 0;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        goto finalize;

    case 0x1:
        if (M2C_FIELD(target_state, u8, 0x138) != 0) {
            M2C_FIELD(target_state, u8, 0x138) = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88BU);
        }
        if (M2C_FIELD(target_state, u8, 0x13B) != 0) {
            M2C_FIELD(target_state, u8, 0x13B) = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88DU);
        }
        M2C_FIELD(target_state, u8, 0x13C) = 0U;
        if (M2C_FIELD(target_state, u8, 0x13D) != 0) {
            M2C_FIELD(target_state, u8, 0x13D) = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88CU);
        }
        if (M2C_FIELD(target_state, u8, 0x141) == 0) {
            goto finalize;
        }
        M2C_FIELD(target_state, u8, 0x141) = 0;
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894U);
        goto finalize;

    case 0x3A:
    case 0x3B:
        target_hp_before_percent_heal = *(volatile u16 *)&target_unit->hp;
        scratch_value = *(volatile u8 *)&action->effect;
        recovered_hp = target_unit->hp;
        target_max_hp_unsigned = *(u16 *)&target_unit->max_hp;
        if (scratch_value == 0x3D) {
            recovery_percent_product = (*(volatile s16 *)&target_unit->max_hp) * 0x3C;
        } else {
            recovery_percent_product = (*(volatile s16 *)&target_unit->max_hp) * 0x1E;
        }
        recovered_hp += Func_080022ec(recovery_percent_product, 0x64);
        if (recovered_hp > (s16) target_max_hp_unsigned) {
            recovered_hp = (s16) target_max_hp_unsigned;
        }
        recovered_hp_amount = recovered_hp - (s16) target_hp_before_percent_heal;
        if ((recovered_hp_amount == 0) && (action_family != 1)) {
            goto finalize;
        }
        if (recovered_hp == (s16) target_max_hp_unsigned) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, recovered_hp_amount);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
        }
        M2C_FIELD(target_state, s16, 0x38) = (s16) recovered_hp;
        goto block_402;
    case 0x3C: {
        s16 current_pp;
        s16 max_pp;
        s32 recovered_pp;
        u32 pp_delta;

        max_pp = M2C_FIELD(target_state, s16, 0x36);
        recovered_pp = M2C_FIELD(target_state, s16, 0x3A);
        current_pp = recovered_pp;
        recovered_pp += Func_080022ec(max_pp * 7, 0x64);
        if (recovered_pp > (s32) max_pp) {
            recovered_pp = (s32) max_pp;
        }
        pp_delta = recovered_pp - current_pp;
        if ((pp_delta == 0) && (action_family != 0xB)) {
            goto finalize;
        }
        if (!(recovered_pp == max_pp)) {
            BattleEvent_Push(BATTLE_EVENT_VALUE, pp_delta);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81EU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821U);
        }
        M2C_FIELD(target_state, s16, 0x3A) = (s16) recovered_pp;
        goto block_402;
    }
    case 0x38:
        M2C_FIELD(target_state, s8, 0x147) = 8;
        M2C_FIELD(target_state, s8, 0x146) = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_state, u16, 0x40) - M2C_FIELD(target_snapshot, u16, 0x40));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x877U);
        goto finalize;

    case 0x37:
        M2C_FIELD(target_state, u8, 0x147) = 0xFCU;
        M2C_FIELD(target_state, s8, 0x146) = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_snapshot, u16, 0x40) - M2C_FIELD(target_state, u16, 0x40));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x878U);
        goto finalize;

    /*
     * Modifier bytes are semantically signed, but these unsigned byte-view
     * loads are intentional: replacing them with direct signed members makes
     * this GCC route emit sign-extending loads and perturbs the whole owner.
     */
    case 0x6:
        attack_down_one = M2C_FIELD(target_state, u8, 0x133) - 1;
        M2C_FIELD(target_state, u8, 0x133) = attack_down_one;
        if ((s32) (s8) attack_down_one < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_snapshot, u16, 0x3C) - M2C_FIELD(target_state, u16, 0x3C));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x860U);
        M2C_FIELD(target_state, u8, 0x132) = 7;
        break;
    case 0x5:
        attack_down_two = M2C_FIELD(target_state, u8, 0x133) - 2;
        M2C_FIELD(target_state, u8, 0x133) = attack_down_two;
        if ((s32) (s8) attack_down_two < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_snapshot, u16, 0x3C) - M2C_FIELD(target_state, u16, 0x3C));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x860U);
        M2C_FIELD(target_state, u8, 0x132) = 7;
        break;
    case 0x4:
        attack_up_one = M2C_FIELD(target_state, u8, 0x133) + 1;
        M2C_FIELD(target_state, u8, 0x133) = attack_up_one;
        if ((s32) (s8) attack_up_one < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_state, u16, 0x3C) - M2C_FIELD(target_snapshot, u16, 0x3C));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x861U);
        M2C_FIELD(target_state, u8, 0x132) = 7;
        break;
    case 0x3:
        attack_up_two = M2C_FIELD(target_state, u8, 0x133) + 2;
        M2C_FIELD(target_state, u8, 0x133) = attack_up_two;
        if ((s32) (s8) attack_up_two < -4) {
            M2C_FIELD(target_state, u8, 0x133) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 4) {
            M2C_FIELD(target_state, u8, 0x133) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_state, u16, 0x3C) - M2C_FIELD(target_snapshot, u16, 0x3C));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x861U);
        M2C_FIELD(target_state, u8, 0x132) = 7;
        break;
    case 0xA:
        defense_down_one = M2C_FIELD(target_state, u8, 0x135) - 1;
        M2C_FIELD(target_state, u8, 0x135) = defense_down_one;
        if ((s32) (s8) defense_down_one < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_snapshot, u16, 0x3E) - M2C_FIELD(target_state, u16, 0x3E));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x862U);
        M2C_FIELD(target_state, u8, 0x134) = 7;
        break;
    case 0x9:
        defense_down_two = M2C_FIELD(target_state, s8, 0x135) - 2;
        M2C_FIELD(target_state, u8, 0x135) = defense_down_two;
        if ((s32) (s8) defense_down_two < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_snapshot, u16, 0x3E) - M2C_FIELD(target_state, u16, 0x3E));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x862U);
        M2C_FIELD(target_state, u8, 0x134) = 7;
        break;
    case 0x8:
        defense_up_one = M2C_FIELD(target_state, u8, 0x135) + 1;
        M2C_FIELD(target_state, u8, 0x135) = defense_up_one;
        if ((s32) (s8) defense_up_one < -4) {
            M2C_FIELD(target_state, u8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_state, u16, 0x3E) - M2C_FIELD(target_snapshot, u16, 0x3E));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x863U);
        M2C_FIELD(target_state, u8, 0x134) = 7;
        break;
    case 0x7:
        defense_up_two = M2C_FIELD(target_state, u8, 0x135) + 2;
        M2C_FIELD(target_state, u8, 0x135) = defense_up_two;
        if ((s32) (s8) defense_up_two < -4) {
            M2C_FIELD(target_state, s8, 0x135) = 0xFCU;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 4) {
            M2C_FIELD(target_state, u8, 0x135) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, M2C_FIELD(target_state, u16, 0x3E) - M2C_FIELD(target_snapshot, u16, 0x3E));
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x863U);
        M2C_FIELD(target_state, u8, 0x134) = 7;
        break;
    case 0x2:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            M2C_FIELD(target_state, s16, 0x38) = (s16) (u16) M2C_FIELD(target_state, s16, 0x34);
            BattleUnit_UpdateRatios(target_id);
        }
        break;
    case 0x35:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            max_hp_for_half_revive = M2C_FIELD(target_state, u16, 0x34);
            M2C_FIELD(target_state, s16, 0x38) = (s16) ((s32) ((s16) max_hp_for_half_revive + ((u32) (max_hp_for_half_revive << 0x10) >> 0x1F)) >> 1);
            goto block_402;
        }
        break;
    case 0x36:
        if (M2C_FIELD(target_state, s16, 0x38) != 0) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            M2C_FIELD(target_state, s16, 0x38) = (s16) Func_080022ec(M2C_FIELD(target_state, s16, 0x34) * 8, 0xA);
            goto block_402;
        }
        break;
    case 0x0:
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884U);
        }
        M2C_FIELD(target_state, s8, 0x131) = 0;
        break;
    case 0xE:
        resistance_down_one = M2C_FIELD(target_state, u8, 0x137) - 1;
        M2C_FIELD(target_state, u8, 0x137) = resistance_down_one;
        if ((s32) (s8) resistance_down_one < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        resistance_down_one = M2C_FIELD(target_state, s8, 0x137);
        if (resistance_down_one > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, (M2C_FIELD(target_snapshot, s8, 0x137) - M2C_FIELD(target_state, s8, 0x137)) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x865U);
        M2C_FIELD(target_state, u8, 0x136) = 7;
        break;
    case 0xD:
        resistance_down_two = M2C_FIELD(target_state, s8, 0x137) - 2;
        M2C_FIELD(target_state, u8, 0x137) = resistance_down_two;
        if ((s32) (s8) resistance_down_two < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        resistance_down_two = M2C_FIELD(target_state, s8, 0x137);
        applied_resistance_down_two = M2C_FIELD(target_state, u8, 0x137);
        if (resistance_down_two > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            applied_resistance_down_two = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, (M2C_FIELD(target_snapshot, s8, 0x137) - (s8) applied_resistance_down_two) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x865U);
        M2C_FIELD(target_state, u8, 0x136) = 7;
        break;
    case 0xC:
        resistance_up_one = M2C_FIELD(target_state, u8, 0x137) + 1;
        M2C_FIELD(target_state, u8, 0x137) = resistance_up_one;
        if ((s32) (s8) resistance_up_one < -4) {
            M2C_FIELD(target_state, s8, 0x137) = 0xFCU;
        }
        resistance_up_one = M2C_FIELD(target_state, s8, 0x137);
        applied_resistance_up_one = M2C_FIELD(target_state, u8, 0x137);
        if (resistance_up_one > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            applied_resistance_up_one = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, ((s8) applied_resistance_up_one - (s8) (u8) M2C_FIELD(target_snapshot, s8, 0x137)) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x866U);
        M2C_FIELD(target_state, u8, 0x136) = 7;
        break;
    case 0xB:
        resistance_up_two = M2C_FIELD(target_state, u8, 0x137) + 2;
        M2C_FIELD(target_state, u8, 0x137) = resistance_up_two;
        if ((s32) (s8) resistance_up_two < -4) {
            M2C_FIELD(target_state, u8, 0x137) = 0xFCU;
        }
        resistance_up_two = M2C_FIELD(target_state, s8, 0x137);
        applied_resistance_up_two = M2C_FIELD(target_state, u8, 0x137);
        if (resistance_up_two > 4) {
            M2C_FIELD(target_state, u8, 0x137) = 4U;
            applied_resistance_up_two = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, ((s8) applied_resistance_up_two - (s8) (u8) M2C_FIELD(target_snapshot, s8, 0x137)) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x866U);
        M2C_FIELD(target_state, u8, 0x136) = 7;
        break;
    case 0xF:
        if (M2C_FIELD(target_state, s8, 0x131) != 0) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x867U);
            M2C_FIELD(target_state, s8, 0x131) = 1;
        }
        break;
    case 0x10:
        if ((s32) M2C_FIELD(target_state, s8, 0x131) > 1) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x874U);
            M2C_FIELD(target_state, s8, 0x131) = 2;
        }
        break;
    case 0x11:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x868U);
        M2C_FIELD(target_state, u8, 0x138) = 7;
        break;
    case 0x12:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x869U);
        M2C_FIELD(target_state, s8, 0x139) = 7;
        break;
    case 0x13:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86AU);
        M2C_FIELD(target_state, u8, 0x13A) = 7;
        break;
    case 0x14:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86BU);
        M2C_FIELD(target_state, u8, 0x13B) = 7;
        break;
    case 0x15:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86CU);
        M2C_FIELD(target_state, u8, 0x13C) = 7;
        break;
    case 0x16:
        if (!((u32) target_id <= 7U)) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x876U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86DU);
        }
        status_flags = target_state + 0x13D;
        status_mask = 7;
        goto block_394;
    case 0x40:
        if ((u32) target_id <= 7U) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86DU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x876U);
        }
        status_flags = target_state + 0x13D;
        status_mask = 0x10;
block_394:
        *status_flags = status_mask | *status_flags;
        break;
    case 0x18:
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, (u32) target_id);
        if (M2C_FIELD(target_state, u8, 0x12A) == 2) {
            defeat_effect_text = 0x84F;
            goto block_399;
        }
        if (action_id == 0xDB) {
            defeat_effect_text = 0x850;
block_399:
            BattleEvent_Push(BATTLE_EVENT_TEXT, defeat_effect_text);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x84CU);
        }
        scratch_value = 0;
        M2C_FIELD(target_state, s16, 0x38) = (s16) scratch_value;
block_402:
        BattleUnit_UpdateRatios(target_id);
        break;
    case 0x1A:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86FU);
        M2C_FIELD(target_state, u8, 0x13E) = 7;
        break;
    case 0x1B:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x870U);
        M2C_FIELD(target_state, s8, 0x13F) = 7;
        break;
    case 0x1C:
    case 0x39:
        actor_hp = M2C_FIELD(actor_state, s16, 0x38);
        actor_hp_recovery = effect_amount;
        if (action->effect == 0x3C) {
            actor_hp_recovery = (u32) ((s32) (actor_hp_recovery + (actor_hp_recovery >> 0x1F)) >> 1);
        }
        actor_max_hp = M2C_FIELD(actor_state, s16, 0x34);
        actor_recovered_hp = actor_hp + actor_hp_recovery;
        if (actor_recovered_hp > (s32) actor_max_hp) {
            actor_recovered_hp = (s32) actor_max_hp;
            actor_hp_recovery = actor_recovered_hp - actor_hp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) actor_id);
        if (actor_recovered_hp == M2C_FIELD(actor_state, s16, 0x34)) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, actor_hp_recovery);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
        }
        M2C_FIELD(actor_state, s16, 0x38) = (s16) actor_recovered_hp;
        goto block_421;
    case 0x1D:
        actor_pp = M2C_FIELD(actor_state, s16, 0x3A);
        actor_pp_recovery = effect_amount;
        actor_max_pp = M2C_FIELD(actor_state, s16, 0x36);
        actor_recovered_pp = actor_pp + actor_pp_recovery;
        if (actor_recovered_pp > (s32) actor_max_pp) {
            actor_recovered_pp = (s32) actor_max_pp;
            actor_pp_recovery = actor_recovered_pp - actor_pp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) actor_id);
        if (!(actor_recovered_pp == M2C_FIELD(actor_state, s16, 0x36))) {
            BattleEvent_Push(BATTLE_EVENT_VALUE, actor_pp_recovery);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81EU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821U);
        }
        M2C_FIELD(actor_state, s16, 0x3A) = (s16) actor_recovered_pp;
block_421:
        BattleUnit_UpdateRatios((u8) actor_id);
        break;
    case 0x42:
        drained_pp = Func_080022ec((s32) effect_amount, 0xA);
        target_pp_available = M2C_FIELD(target_state, s16, 0x3A);
        if ((s32) target_pp_available < drained_pp) {
            drained_pp = (s32) target_pp_available;
        }
        actor_pp_before_drain = M2C_FIELD(actor_state, s16, 0x3A);
        if ((s32) (actor_pp_before_drain + drained_pp) > (s32) (M2C_FIELD(actor_state, s16, 0x36))) {
            drained_pp = (M2C_FIELD(actor_state, s16, 0x36)) - actor_pp_before_drain;
        }
        if (drained_pp == 0) {

        } else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) drained_pp);
            if ((u32) target_id <= 7U) {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x85FU);
            } else {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x85EU);
            }
            Func_08077120(actor_id, drained_pp);
        }
        break;
    case 0x1E:
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x133) > 0) {
            M2C_FIELD(target_state, u8, 0x133) = 0U;
            M2C_FIELD(target_state, s8, 0x132) = 0;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x135) > 0) {
            M2C_FIELD(target_state, u8, 0x135) = 0U;
            M2C_FIELD(target_state, s8, 0x134) = 0;
        }
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x137) > 0) {
            M2C_FIELD(target_state, u8, 0x137) = 0U;
            M2C_FIELD(target_state, s8, 0x136) = 0;
        }
        if ((s32) M2C_FIELD(target_state, s8, 0x147) > 0) {
            M2C_FIELD(target_state, s8, 0x147) = 0;
        }
        M2C_FIELD(target_state, s8, 0x12C) = 0;
        M2C_FIELD(target_state, s8, 0x12D) = 0;
        M2C_FIELD(target_state, s8, 0x12E) = 0;
        M2C_FIELD(target_state, s8, 0x12F) = 0;
        effect_text = 0x896;
block_446:
        BattleEvent_Push(BATTLE_EVENT_TEXT, effect_text);
        break;
    case 0x17:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x872U);
        status_level_one = target_state + 0x140;
        goto block_452;
    case 0x19:
        status_141_value = M2C_FIELD(target_state, u8, 0x141);
        if (status_141_value == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x873U);
            M2C_FIELD(target_state, u8, 0x141) = 7U;
        } else if ((u32) status_141_value > 1U) {
            M2C_FIELD(target_state, u8, 0x141) = (u8) (status_141_value + 0xFF);
            BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) M2C_FIELD(target_state, u8, 0x141));
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x875U);
        }
        break;
    case 0x3F:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87DU);
        status_level_two = target_state + 0x144;
        goto block_454;
    case 0x33:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87EU);
        M2C_FIELD(target_state, s8, 0x148) = 1;
        if ((u32) target_id <= 7U) {
            M2C_FIELD(battle_state, u8, 0x43) = (u8) (M2C_FIELD(battle_state, u8, 0x43) | 2);
        }
        break;
    case 0x32:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87FU);
        status_level_one = target_state + 0x145;
        goto block_452;
    case 0x2B:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x881U);
        status_level_one = target_state + 0x12B;
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x12B) > 0) {
            break;
        }
block_452:
        *status_level_one = 1;
        break;
    case 0x2C:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x882U);
        status_level_two = target_state + 0x12B;
        if ((s32) (s8) M2C_FIELD(target_state, u8, 0x12B) > 1) {
            break;
        }
block_454:
        *status_level_two = 2;
        break;
    case 0x2A:
        BattleEvent_Push(BATTLE_EVENT_TEXT, -1U);
        break;
    }
finalize:
    BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
    if (M2C_FIELD(target_state, s16, 0x38) != 0) {
        status_13c_value = M2C_FIELD(target_state, u8, 0x13C);
        if (status_13c_value != 0) {
            if ((u32) status_13c_value <= 6U) {
                if ((s32) effect_amount > 0) {
                    recovery_roll = BattleRandom_Next() & 3;
                    if (recovery_roll == 0) {
                        M2C_FIELD(target_state, u8, 0x13C) = recovery_roll;
                        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x883U);
                    }
                }
            }
        }
    }
    Func_08002df0((s16 *) target_snapshot);
    BattleUnit_Recalculate(target_id);
    Func_08015130(M2C_FIELD(*(void **)0x03001E74, u8, 0x41));
    if (M2C_FIELD(target_state, s16, 0x38) != 0) {
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
    }
    status_140_address = (u32)actor_state;
    result = 0x140;
    status_140_address += result;
    if (*(volatile u8 *)status_140_address != 0) {
        result = BattleRandom_Next() & 3;
        if ((result == 0) && ((s32) effect_amount > 0)) {
            result = (s32) effect_amount >> 2;
            if (result == 0) {
                result = 1;
            }
            plan->pending_amount_60 += result;
        }
    }
    return result;
}
