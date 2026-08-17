#include "battle_command.h"
#include "motion_object.h"

/* exact/080d40ec.c and exact/080e0524.c witness this IWRAM copy ABI. */
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

/* Byte-offset view for regions battle_types.h does not name yet. */
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* asm/080022ec.s and asm/080022f4.s: the IWRAM divide thunks. */
#define DivideSigned   Func_080022ec
#define DivideUnsigned Func_080022f4

/* exact/08004938.c: the battle scratch bump allocator and its release. */
#define ScratchAlloc Func_08004938
#define ScratchFree  Func_08002df0

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
 * Resolve one target of one queued battle action.  Callers loop over
 * plan->target_ids[0 .. plan->target_count - 1] and call this once per slot.
 *
 * The routine snapshots the target's live record, resolves elemental affinity
 * and the caster's elemental power, rolls whether the effect lands, applies the
 * primary HP/PP damage or recovery for the action family, then applies the
 * secondary status/stat effect keyed on action->effect.  Every visible result
 * is queued as battle-message opcodes through BattleEvent_Push; the routine
 * draws nothing itself.  Neither caller uses the return value.
 *
 * Offsets that battle_types.h does not yet name keep offset-style access:
 * the per-element (power, resistance) pairs at unit+0x24 (base) and unit+0x48
 * (post-modifier), the runtime block behind 0x03001E74, and the six-entry
 * area-of-effect falloff tables in ROM at 0x080C2AB8 .. 0x080C2B68.
 */
s32 Func_080bbb0c(struct BattlePlan *plan, s32 target_slot) {
    u16 queued_ids[7];
    struct BattleAction *action;
    struct BattleUnit *actor;
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
    struct BattleUnit *snapshot;
    s32 source_stat;
    s32 power_bonus_percent;
    s16 element_resist;
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
    struct BattleUnit *target;
    s32 object_effect_config;
    s32 queued_count;
    s32 hp_minus_one;
    s32 stumble_score;
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
    s32 lower_count;
    s32 recovery_source_stat;
    s32 upper_count;
    s32 actor_adjustment_offset;
    s32 target_adjustment_offset;
    s32 decision_offset;
    s32 queue_head;
    s32 falloff_percent;
    s32 prior_health;
    s32 restored_pp;
    s32 healing_source_stat;
    s32 object_anchor_x;
    s32 object_anchor_z;
    s32 falloff_table;
    s32 scratch_value;
    s32 falloff_offset_ranged;
    s32 scaled_ranged_damage;
    s32 effect_damage;
    s32 recovered_hp;
    s32 actor_recovered_pp;
    s32 drained_pp;
    s32 turn_order_index;
    s32 queued_index;
    s32 adjusted_damage;
    s32 modified_damage;
    s32 resulting_hp;
    s32 resulting_pp;
    s32 secondary_resulting_pp;
    s32 actor_recovered_hp;
    s32 damage_pass;
    s32 ranged_damage_pass;
    s8 effect_guard_level;
    s8 guard_level;
    s8 pp_guard_level;
    s8 secondary_pp_guard_level;
    s32 range_distance;
    s32 hit_result;
    u8 *accuracy_table;
    s32 *falloff_percents;
    s32 falloff_offset;
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
    u32 continuation_text;
    u32 pp_damage_text;
    u32 secondary_pp_damage_text;
    u32 decision_index;
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
    u8 *element_table;
    u8 *element_cursor;
    s16 *element_slot;
    u32 status_140_address;

    stat_delta = 0;
    battle_state = *(void **)0x03001E74;
    halve_defense = 0;
    effect_amount = 0;
    leave_one_hp = 0;
    skip_primary_effect = 0;
    range_relation = 0;
    snapshot = ScratchAlloc(BATTLE_UNIT_SIZE);
    actor_id = (s32) plan->actor_id;
    target_id = plan->target_ids[target_slot];
    action_id = plan->action_id;
    range_index = plan->range_index;
    target_adjustment = plan->target_adjustments[target_slot];
    target_modifier = plan->target_modifiers[target_slot];
    action = BattleAction_Get(action_id);
    actor = BattleUnit_Get(actor_id);
    target = BattleUnit_Get((s32) target_id);
    ((WordCopy)0x03001388)(snapshot, target, BATTLE_UNIT_SIZE);
    if (action->range != 0xFF) {
        range_distance = plan->target_offsets[target_slot];
        if (range_distance < 0) {
            range_distance = -range_distance;
        }
    } else {
        range_distance = 0;
    }
    if (range_index != 4) {
        element_resist = *(s16 *)((u8 *)target + range_index * 4 + 0x26);
        lower_count = 0;
        element_table = (u8 *)target + 0x24;
        if ((s32) element_resist >= (s32) M2C_FIELD(element_table, s16, 2)) {
            element_cursor = element_table;
            do {
                lower_count += 1;
                element_cursor += 4;
            } while ((lower_count <= 3) &&
                     ((s32) element_resist >= (s32) M2C_FIELD(element_cursor, s16, 2)));
        }
        if (lower_count == 4) {
            range_relation = -1;
        }
        upper_count = 0;
        element_cursor = (u8 *)target + 0x24;
        if ((s32) element_resist <= (s32) M2C_FIELD(element_cursor, s16, 2)) {
            for (;;) {
                upper_count += 1;
                if (upper_count > 3) {
                    break;
                }
                element_slot = (s16 *)((u8 *)target + upper_count * 4 + 0x24);
                element_cursor = (u8 *)element_slot;
                if ((s32) element_resist > (s32) M2C_FIELD(element_slot, s16, 2)) {
                    break;
                }
            }
        }
        if (upper_count == 4) {
            range_relation = 1;
        }
    }
    range_index_unsigned = (u32) plan->range_index;
    if ((range_index_unsigned <= 3U) && (plan->command != 2)) {
        actor_adjustment_offset = (range_index_unsigned * 4) + 0x48;
        source_stat = M2C_FIELD(actor, s16, actor_adjustment_offset);
    } else {
        source_stat = 0x64;
    }
    if ((plan->command == 5) && (range_index_unsigned <= 3U) && (range_relation > 0)) {
        target_adjustment_offset = (range_index_unsigned * 4) + 0x48;
        stumble_score = source_stat - M2C_FIELD(
            (u8 *)target + target_adjustment_offset,
            s16,
            2
        );
        stumble_score += 0x1E;
        stumble_score *= 0x28F;
        if (stumble_score > (BattleRandom_Next() & 0xFFFF)) {
            BattleEvent_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5U);
        }
    }
    action_family = 0xF & action->target_flags;
    hit_result = plan->target_results[target_slot];
    if (hit_result == -1) {
        accuracy_table = (u8 *)0x080C2AB8;
        hit_result = Func_08077178(
            actor_id,
            target_id,
            range_index,
            action->effect,
            accuracy_table[range_distance]
        );
    }
    action_allowed = hit_result;
    hit_result = 0;
    if ((action->effect == 0x32) || (action->effect == 0x33)) {
        class_id = actor->class_id;
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
            queued_count = Func_080b6ae0(queued_ids);
            for (queued_index = 0;
                 queued_index < queued_count;
                 queued_index++) {
                Func_080b8000(queued_ids[queued_index]);
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
            decision_offset = 0x2EC;
            decision_index = 0;
            if (M2C_FIELD(battle_state, s16, decision_offset) == target_id) {
                action_allowed = 1;
            } else {
scan_decisions:
                decision_index += 1;
                if (decision_index <= 0x13U) {
                    decision_offset = (decision_index * 0x10) + 0x2EC;
                    if (M2C_FIELD(
                        battle_state,
                        s16,
                        decision_offset
                    ) == target_id) {
                        action_allowed = 1;
                    } else {
                        goto scan_decisions;
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
                if (actor->hp != 0) {
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, (u32) actor_id);
                }
            } else if (action->effect == 0x20) {
                if (target->pp != 0) {
                    action_family = 0xA;
                } else {
                    action_allowed = 0;
                }
            }
        }
    }
    if ((skip_primary_effect == 0) &&
        ((target->hp != 0) || (BattleEffect_Classify(action->effect) != 0))) {
        switch (action_family) {
        case 3:
        case 4:
            target_defense = target->defense;
            target_hp = target->hp;
            effective_defense = target_defense;
            if (halve_defense != 0) {
                effective_defense = (u16) (target_defense >> 1);
            }
            for (damage_pass = 1; damage_pass <= 1; damage_pass++) {
            if (range_index != 4) {
                target_adjustment_offset = range_index * 4;
                target_adjustment_offset += 0x48;
                stat_delta = source_stat - M2C_FIELD(
                    (u8 *)target + target_adjustment_offset,
                    s16,
                    2
                );
            }
            if (damage_pass == 0) {
                stat_delta = 0;
            }
            action_power = action->power;
            if (action_family == 4) {
                base_damage = DivideSigned(Func_08077180(actor->attack, (u32) effective_defense, 0U, stat_delta) * action_power, 0xA);
            } else {
                base_damage = Func_08077180(actor->attack, (u32) effective_defense, action_power, stat_delta);
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
                    (u8) DivideUnsigned(M2C_FIELD(target, u8, 0xF), 5) +
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
            guard_level = (s8) target->guard_level;
            if (guard_level != 0) {
                if (guard_level == 1) {
                    damage = (u32) ((s32) (damage + (damage >> 0x1F)) >> 1);
                } else {
                    damage = (u32) DivideSigned((s32) damage, 0xA);
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
            if ((BattleFlag_Test(0x16E) != 0) && (plan->command == 5) && ((s32) target_hp <= (s32) damage)) {
                damage = target_hp - 1;
            }
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
            /* Shares case 2's write-back tail; the reference does too. */
            if (resulting_hp > 0) {
                goto target_survives;
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
            goto commit_damage;
        case 10:
            if (action->power != 0) {
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        (u8 *)target + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                action_power = action->power;
                pp_damage = Func_08077188(action_power, stat_delta, 0x100);
                falloff_percents = (s32 *)0x080C2AC0;
                falloff_offset = range_distance * 4;
                pp_damage = DivideSigned(
                    pp_damage * M2C_FIELD(falloff_percents, s32, falloff_offset),
                    0x64
                );
                pp_damage *= target_adjustment;
                pp_guard_level = (s8) target->guard_level;
                if (pp_guard_level != 0) {
                    if (pp_guard_level == 1) {
                        pp_damage = (u32) ((s32) (pp_damage + (pp_damage >> 0x1F)) >> 1);
                    } else {
                        pp_damage = (u32) DivideSigned((s32) pp_damage, 0xA);
                    }
                }
                if ((action->effect == 0x20) && ((s32) pp_damage > (s32) (target->pp))) {
                    pp_damage = (u32) (target->pp);
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, pp_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if ((u32) target_id > 7U) {
                    pp_damage_text = 0x829;
                } else {
                    pp_damage_text = 0x82A;
                }
                resulting_pp = (target->pp) - pp_damage;
                BattleEvent_Push(BATTLE_EVENT_TEXT, pp_damage_text);
                if (resulting_pp <= 0) {
                    resulting_pp = 0;
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                effect_amount = target->pp - resulting_pp;
                target->pp = (s16) resulting_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 1:
            if (action->power != 0) {
                target_hp_before_heal = target->hp;
                healing_source_stat = source_stat;
                healing_power = action->power;
                if (range_index == 4) {
                    healing_source_stat = 0x64;
                }
                healing_power = Func_08077190(healing_power, healing_source_stat, 0x100);
                falloff_percents = (s32 *)0x080C2AD8;
                healing_power = DivideSigned(healing_power * falloff_percents[range_distance], 0x64);
                healing_power *= target_adjustment;
                healed_hp = healing_power + (3 & BattleRandom_Next());
                target_max_hp = target->max_hp;
                resulting_hp = target_hp_before_heal + healed_hp;
                if (resulting_hp > (s32) target_max_hp) {
                    resulting_hp = (s32) target_max_hp;
                    healed_hp = resulting_hp - target->hp;
                }
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (!(resulting_hp == target->max_hp)) {
                    BattleEvent_Push(BATTLE_EVENT_VALUE, healed_hp);
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
                }
                prior_health = target->hp;
                effect_amount = prior_health - resulting_hp;
                target->hp = (s16) resulting_hp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case -1:
            if (action->power != 0) {
                target_pp_before_damage = target->pp;
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        (u8 *)target + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                secondary_pp_damage = Func_08077188(action->power, stat_delta, 0x100);
                secondary_pp_damage *= ((s32 *)0x080C2AF0)[range_distance];
                secondary_pp_damage = DivideSigned(secondary_pp_damage, 0x64);
                secondary_pp_damage *= target_adjustment;
                secondary_pp_guard_level = (s8) target->guard_level;
                if (secondary_pp_guard_level != 0) {
                    if (secondary_pp_guard_level == 1) {
                        secondary_pp_damage = (u32) ((s32) (secondary_pp_damage + (secondary_pp_damage >> 0x1F)) >> 1);
                    } else {
                        secondary_pp_damage = (u32) DivideSigned((s32) secondary_pp_damage, 0xA);
                    }
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, secondary_pp_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if ((u32) target_id > 7U) {
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
                target->pp = (s16) secondary_resulting_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 5:
        case 6:
        case 8:
            if (action->power != 0) {
                target_hp_before_ranged_damage = target->hp;
                for (ranged_damage_pass = 1; ranged_damage_pass <= 1;
                     ranged_damage_pass++) {
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        (u8 *)target + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                if (ranged_damage_pass == 0) {
                    stat_delta = 0;
                }
                scaled_action_power = action->power;
                if (plan->command == 6) {
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
                    scaled_action_power += DivideSigned(power_bonus_percent * target->max_hp, 0x64);
                }
                scaled_ranged_damage = Func_08077188(scaled_action_power, stat_delta, 0x100);
                scaled_ranged_damage *= target_adjustment;
                switch (action_family) {
                case 5:
                    falloff_table = 0x080C2B08;
                    falloff_offset_ranged = range_distance * 4;
                    goto apply_falloff;
                case 8:
                    falloff_table = 0x080C2B20;
                    falloff_offset_ranged = range_distance * 4;
                    goto apply_falloff;
                case 6:
                    falloff_table = 0x080C2B38;
                    falloff_offset_ranged = range_distance * 4;
apply_falloff:
                    falloff_percent = M2C_FIELD((u8 *)falloff_table, s32, falloff_offset_ranged);
                    scaled_ranged_damage = DivideSigned(
                        falloff_percent * scaled_ranged_damage,
                        0x64
                    );
                    break;
                }
                ranged_damage = scaled_ranged_damage + (3 & BattleRandom_Next());
                if (((s8) target->guard_level) != 0) {
                    if (((s8) target->guard_level) == 1) {
                        ranged_damage = (u32) ((s32) (ranged_damage + (ranged_damage >> 0x1F)) >> 1);
                    } else {
                        ranged_damage = (u32) DivideSigned((s32) ranged_damage, 0xA);
                    }
                }
                if ((BattleFlag_Test(0x16E) != 0) && (plan->command == 6) && ((s32) target_hp_before_ranged_damage > (s32) ranged_damage)) {
                    ranged_damage = (u32) target_hp_before_ranged_damage;
                }
                }
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, ranged_damage);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (!((u32) target_id <= 7U)) {
                    ranged_damage_text = range_relation + 0x831;
                } else {
                    ranged_damage_text = range_relation + 0x834;
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
                prior_health = target->hp;
                effect_amount = prior_health - resulting_hp;
                target->hp = (s16) resulting_hp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 11:
            if (action->power != 0) {
                target_pp_before_recovery = target->pp;
                recovery_source_stat = source_stat;
                if (range_index == 4) {
                    recovery_source_stat = 0x64;
                }
                pp_recovery = action->power;
                pp_recovery = Func_08077190(pp_recovery, recovery_source_stat, 0x100);
                falloff_percents = (s32 *)0x080C2B50;
                falloff_offset = range_distance * 4;
                pp_recovery = DivideSigned(
                    pp_recovery * M2C_FIELD(falloff_percents, s32, falloff_offset),
                    0x64
                );
                target_max_pp = target->max_pp;
                pp_recovery *= target_adjustment;
                restored_pp = target_pp_before_recovery + pp_recovery;
                if (restored_pp > (s32) target_max_pp) {
                    restored_pp = (s32) target_max_pp;
                    pp_recovery = restored_pp - target->pp;
                }
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                if (restored_pp == target->max_pp) {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821U);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_VALUE, pp_recovery);
                    BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81EU);
                }
                target->pp = (s16) restored_pp;
                BattleUnit_UpdateRatios(target_id);
            }
            break;
        case 2:
            if (action_allowed != 0) {
                if (action->power != 0) {
                target_hp_before_effect = target->hp;
                if (range_index != 4) {
                    target_adjustment_offset = range_index * 4;
                    target_adjustment_offset += 0x48;
                    stat_delta = source_stat - M2C_FIELD(
                        (u8 *)target + target_adjustment_offset,
                        s16,
                        2
                    );
                }
                effect_damage = Func_08077188(action->power, stat_delta, 0x100);
                effect_damage *= target_adjustment;
                effect_damage *= ((s32 *)0x080C2B68)[range_distance];
                effect_damage = DivideSigned(effect_damage, 0x64);
                effect_guard_level = (s8) target->guard_level;
                if (effect_guard_level != 0) {
                    if (effect_guard_level == 1) {
                        effect_damage = (s32) (effect_damage + ((u32) effect_damage >> 0x1F)) >> 1;
                    } else {
                        effect_damage = DivideSigned(effect_damage, 0xA);
                    }
                }
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) effect_damage);
                if ((u32) target_id > 7U) {
                    effect_damage_text = 0x826;
                } else {
                    effect_damage_text = 0x827;
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
target_survives:
                    BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                }
commit_damage:
                prior_health = target->hp;
                effect_amount = prior_health - resulting_hp;
                target->hp = (s16) resulting_hp;
                BattleUnit_UpdateRatios(target_id);
                }
            } else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x854U);
            }
            break;
        }
    }
    BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
    if ((BattleEffect_Classify(action->effect) == 0) && (target->hp == 0) && (Func_080bbae8((s32) action->effect) == 0)) {
        goto finalize;
    }
    if (action_allowed == 0) {
        goto finalize;
    }
    switch ((s32) action->effect) {
    case 0x40:
        if (target->status_138 != 0) {
            target->status_138 = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88BU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (target->status_13b != 0) {
            target->status_13b = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88DU);
        }
        target->status_13c = 0U;
        if (target->status_13d != 0) {
            target->status_13d = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88CU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (target->status_141 != 0) {
            target->status_141 = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (target->status_140 != 0) {
            target->status_140 = 0U;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88FU);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        }
        if (target->status_131 != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884U);
            target->status_131 = 0;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        goto finalize;

    case 0x4:
        if (target->status_138 != 0) {
            target->status_138 = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88BU);
        }
        if (target->status_13b != 0) {
            target->status_13b = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88DU);
        }
        target->status_13c = 0U;
        if (target->status_13d != 0) {
            target->status_13d = 0U;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
            BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88CU);
        }
        if (target->status_141 == 0) {
            goto finalize;
        }
        target->status_141 = 0;
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894U);
        goto finalize;

    case 0x3D:
    case 0x3E:
        target_hp_before_percent_heal = *(volatile u16 *)&target->hp;
        scratch_value = *(volatile u8 *)&action->effect;
        recovered_hp = target->hp;
        target_max_hp_unsigned = *(u16 *)&target->max_hp;
        if (scratch_value == 0x3D) {
            recovery_percent_product = (*(volatile s16 *)&target->max_hp) * 0x3C;
        } else {
            recovery_percent_product = (*(volatile s16 *)&target->max_hp) * 0x1E;
        }
        recovered_hp += DivideSigned(recovery_percent_product, 0x64);
        if (recovered_hp > (s16) target_max_hp_unsigned) {
            recovered_hp = (s16) target_max_hp_unsigned;
        }
        recovered_hp_amount = recovered_hp - (s16) target_hp_before_percent_heal;
        if ((recovered_hp_amount == 0) && (action_family != 1)) {
            goto finalize;
        }
        if (!(recovered_hp != (s16) target_max_hp_unsigned)) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, recovered_hp_amount);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
        }
        target->hp = (s16) recovered_hp;
        BattleUnit_UpdateRatios(target_id);
        break;
    case 0x3F: {
        s16 current_pp;
        s16 max_pp;
        s32 recovered_pp;
        u32 pp_delta;

        recovered_pp = target->pp;
        max_pp = target->max_pp;
        current_pp = recovered_pp;
        recovered_pp += DivideSigned(max_pp * 7, 0x64);
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
        target->pp = (s16) recovered_pp;
        BattleUnit_UpdateRatios(target_id);
        break;
    }
    case 0x3B:
        target->agility_modifier = 8;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->agility - snapshot->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x877U);
        goto finalize;

    case 0x3A:
        (*(u8 *)&target->agility_modifier) = 0xFCU;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, snapshot->agility - target->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x878U);
        goto finalize;

    /*
     * Modifier bytes are semantically signed, but these unsigned byte-view
     * loads are intentional: replacing them with direct signed members makes
     * this GCC route emit sign-extending loads and perturbs the whole owner.
     */
    case 0x9:
        attack_down_one = (*(u8 *)&target->attack_modifier) - 1;
        (*(u8 *)&target->attack_modifier) = attack_down_one;
        if ((s32) (s8) attack_down_one < -4) {
            (*(u8 *)&target->attack_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->attack_modifier) > 4) {
            (*(u8 *)&target->attack_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, snapshot->attack - target->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x860U);
        target->attack_modifier_turns = 7;
        break;
    case 0x8:
        attack_down_two = (*(u8 *)&target->attack_modifier) - 2;
        (*(u8 *)&target->attack_modifier) = attack_down_two;
        if ((s32) (s8) attack_down_two < -4) {
            (*(u8 *)&target->attack_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->attack_modifier) > 4) {
            (*(u8 *)&target->attack_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, snapshot->attack - target->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x860U);
        target->attack_modifier_turns = 7;
        break;
    case 0x7:
        attack_up_one = (*(u8 *)&target->attack_modifier) + 1;
        (*(u8 *)&target->attack_modifier) = attack_up_one;
        if ((s32) (s8) attack_up_one < -4) {
            (*(u8 *)&target->attack_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->attack_modifier) > 4) {
            (*(u8 *)&target->attack_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->attack - snapshot->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x861U);
        target->attack_modifier_turns = 7;
        break;
    case 0x6:
        attack_up_two = (*(u8 *)&target->attack_modifier) + 2;
        (*(u8 *)&target->attack_modifier) = attack_up_two;
        if ((s32) (s8) attack_up_two < -4) {
            (*(u8 *)&target->attack_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->attack_modifier) > 4) {
            (*(u8 *)&target->attack_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->attack - snapshot->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x861U);
        target->attack_modifier_turns = 7;
        break;
    case 0xD:
        defense_down_one = (*(u8 *)&target->defense_modifier) - 1;
        (*(u8 *)&target->defense_modifier) = defense_down_one;
        if ((s32) (s8) defense_down_one < -4) {
            (*(u8 *)&target->defense_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->defense_modifier) > 4) {
            (*(u8 *)&target->defense_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, snapshot->defense - target->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x862U);
        target->defense_modifier_turns = 7;
        break;
    case 0xC:
        defense_down_two = target->defense_modifier - 2;
        (*(u8 *)&target->defense_modifier) = defense_down_two;
        if ((s32) (s8) defense_down_two < -4) {
            (*(u8 *)&target->defense_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->defense_modifier) > 4) {
            (*(u8 *)&target->defense_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, snapshot->defense - target->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x862U);
        target->defense_modifier_turns = 7;
        break;
    case 0xB:
        defense_up_one = (*(u8 *)&target->defense_modifier) + 1;
        (*(u8 *)&target->defense_modifier) = defense_up_one;
        if ((s32) (s8) defense_up_one < -4) {
            (*(u8 *)&target->defense_modifier) = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->defense_modifier) > 4) {
            (*(u8 *)&target->defense_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->defense - snapshot->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x863U);
        target->defense_modifier_turns = 7;
        break;
    case 0xA:
        defense_up_two = (*(u8 *)&target->defense_modifier) + 2;
        (*(u8 *)&target->defense_modifier) = defense_up_two;
        if ((s32) (s8) defense_up_two < -4) {
            target->defense_modifier = 0xFCU;
        }
        if ((s32) (s8) (*(u8 *)&target->defense_modifier) > 4) {
            (*(u8 *)&target->defense_modifier) = 4U;
        }
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->defense - snapshot->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x863U);
        target->defense_modifier_turns = 7;
        break;
    case 0x5:
        if (target->hp == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            target->hp = (s16) (u16) target->max_hp;
            BattleUnit_UpdateRatios(target_id);
        }
        break;
    case 0x38:
        if (target->hp == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            max_hp_for_half_revive = (*(u16 *)&target->max_hp);
            target->hp = (s16) ((s32) ((s16) max_hp_for_half_revive + ((u32) (max_hp_for_half_revive << 0x10) >> 0x1F)) >> 1);
            BattleUnit_UpdateRatios(target_id);
        }
        break;
    case 0x39:
        if (target->hp == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864U);
            target->hp = (s16) DivideSigned(target->max_hp * 8, 0xA);
            BattleUnit_UpdateRatios(target_id);
        }
        break;
    case 0x3:
        if (target->status_131 != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884U);
        }
        target->status_131 = 0;
        break;
    case 0x11:
        resistance_down_one = (*(u8 *)&target->status_137) - 1;
        (*(u8 *)&target->status_137) = resistance_down_one;
        if ((s32) (s8) resistance_down_one < -4) {
            (*(u8 *)&target->status_137) = 0xFCU;
        }
        resistance_down_one = target->status_137;
        if (resistance_down_one > 4) {
            (*(u8 *)&target->status_137) = 4U;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, (snapshot->status_137 - target->status_137) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x865U);
        target->status_136 = 7;
        break;
    case 0x10:
        resistance_down_two = target->status_137 - 2;
        (*(u8 *)&target->status_137) = resistance_down_two;
        if ((s32) (s8) resistance_down_two < -4) {
            (*(u8 *)&target->status_137) = 0xFCU;
        }
        resistance_down_two = target->status_137;
        applied_resistance_down_two = (*(u8 *)&target->status_137);
        if (resistance_down_two > 4) {
            (*(u8 *)&target->status_137) = 4U;
            applied_resistance_down_two = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, (snapshot->status_137 - (s8) applied_resistance_down_two) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x865U);
        target->status_136 = 7;
        break;
    case 0xF:
        resistance_up_one = (*(u8 *)&target->status_137) + 1;
        (*(u8 *)&target->status_137) = resistance_up_one;
        if ((s32) (s8) resistance_up_one < -4) {
            target->status_137 = 0xFCU;
        }
        resistance_up_one = target->status_137;
        applied_resistance_up_one = (*(u8 *)&target->status_137);
        if (resistance_up_one > 4) {
            (*(u8 *)&target->status_137) = 4U;
            applied_resistance_up_one = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, ((s8) applied_resistance_up_one - (s8) (u8) snapshot->status_137) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x866U);
        target->status_136 = 7;
        break;
    case 0xE:
        resistance_up_two = (*(u8 *)&target->status_137) + 2;
        (*(u8 *)&target->status_137) = resistance_up_two;
        if ((s32) (s8) resistance_up_two < -4) {
            (*(u8 *)&target->status_137) = 0xFCU;
        }
        resistance_up_two = target->status_137;
        applied_resistance_up_two = (*(u8 *)&target->status_137);
        if (resistance_up_two > 4) {
            (*(u8 *)&target->status_137) = 4U;
            applied_resistance_up_two = 4;
        }
        BattleEvent_Push(BATTLE_EVENT_VALUE, ((s8) applied_resistance_up_two - (s8) (u8) snapshot->status_137) * 0x14);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x866U);
        target->status_136 = 7;
        break;
    case 0x12:
        if (target->status_131 == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x867U);
            target->status_131 = 1;
        }
        break;
    case 0x13:
        if ((s32) target->status_131 <= 1) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x874U);
            target->status_131 = 2;
        }
        break;
    case 0x14:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x868U);
        target->status_138 = 7;
        break;
    case 0x15:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x869U);
        target->status_139 = 7;
        break;
    case 0x16:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86AU);
        target->status_13a = 7;
        break;
    case 0x17:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86BU);
        target->status_13b = 7;
        break;
    case 0x18:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86CU);
        target->status_13c = 7;
        break;
    case 0x19:
        if ((u32) target_id <= 7U) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86DU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x876U);
        }
        target->status_13d =
            (u8) (target->status_13d | 7);
        break;
    case 0x43:
        if ((u32) target_id <= 7U) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86DU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x876U);
        }
        target->status_13d =
            (u8) (target->status_13d | 0x10);
        break;
    case 0x1B:
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, (u32) target_id);
        if (target->status_12a == 2) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x84FU);
        } else if (action_id == 0xDB) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x850U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x84CU);
        }
        scratch_value = 0;
        target->hp = (s16) scratch_value;
        BattleUnit_UpdateRatios(target_id);
        break;
    case 0x1D:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x86FU);
        target->status_13e = 7;
        break;
    case 0x1E:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x870U);
        target->status_13f = 7;
        break;
    case 0x1F:
    case 0x3C:
        actor_hp = actor->hp;
        actor_hp_recovery = effect_amount;
        if (action->effect == 0x3C) {
            actor_hp_recovery = (u32) ((s32) (actor_hp_recovery + (actor_hp_recovery >> 0x1F)) >> 1);
        }
        actor_max_hp = actor->max_hp;
        actor_recovered_hp = actor_hp + actor_hp_recovery;
        if (actor_recovered_hp > (s32) actor_max_hp) {
            actor_recovered_hp = (s32) actor_max_hp;
            actor_hp_recovery = actor_recovered_hp - actor_hp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) actor_id);
        if (actor_recovered_hp == actor->max_hp) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820U);
        } else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, actor_hp_recovery);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81DU);
        }
        actor->hp = (s16) actor_recovered_hp;
        BattleUnit_UpdateRatios((u8) actor_id);
        break;
    case 0x20:
        actor_pp = actor->pp;
        actor_pp_recovery = effect_amount;
        actor_max_pp = actor->max_pp;
        actor_recovered_pp = actor_pp + actor_pp_recovery;
        if (actor_recovered_pp > (s32) actor_max_pp) {
            actor_recovered_pp = (s32) actor_max_pp;
            actor_pp_recovery = actor_recovered_pp - actor_pp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) actor_id);
        if (!(actor_recovered_pp == actor->max_pp)) {
            BattleEvent_Push(BATTLE_EVENT_VALUE, actor_pp_recovery);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81EU);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821U);
        }
        actor->pp = (s16) actor_recovered_pp;
        BattleUnit_UpdateRatios((u8) actor_id);
        break;
    case 0x45:
        drained_pp = DivideSigned((s32) effect_amount, 0xA);
        target_pp_available = target->pp;
        if ((s32) target_pp_available < drained_pp) {
            drained_pp = (s32) target_pp_available;
        }
        actor_pp_before_drain = actor->pp;
        if ((s32) (actor_pp_before_drain + drained_pp) > (s32) (actor->max_pp)) {
            drained_pp = (actor->max_pp) - actor_pp_before_drain;
        }
        if (drained_pp != 0) {
            BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) drained_pp);
            if (!((u32) target_id <= 7U)) {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x85EU);
            } else {
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x85FU);
            }
            Func_08077120(actor_id, drained_pp);
        }
        break;
    case 0x21:
        if ((s32) (s8) (*(u8 *)&target->attack_modifier) > 0) {
            (*(u8 *)&target->attack_modifier) = 0U;
            target->attack_modifier_turns = 0;
        }
        if ((s32) (s8) (*(u8 *)&target->defense_modifier) > 0) {
            (*(u8 *)&target->defense_modifier) = 0U;
            target->defense_modifier_turns = 0;
        }
        if ((s32) (s8) (*(u8 *)&target->status_137) > 0) {
            (*(u8 *)&target->status_137) = 0U;
            target->status_136 = 0;
        }
        if ((s32) target->agility_modifier > 0) {
            target->agility_modifier = 0;
        }
        target->status_12c = 0;
        target->status_12d = 0;
        target->status_12e = 0;
        target->status_12f = 0;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x896U);
        break;
    case 0x1A:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x872U);
        target->status_140 = 1;
        break;
    case 0x1C:
        status_141_value = target->status_141;
        if (status_141_value == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x873U);
            target->status_141 = 7U;
        } else if ((u32) status_141_value > 1U) {
            target->status_141 = (u8) (status_141_value + 0xFF);
            BattleEvent_Push(BATTLE_EVENT_VALUE, (u32) target->status_141);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x875U);
        }
        break;
    case 0x42:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87DU);
        M2C_FIELD(target, s8, 0x144) = 2;
        break;
    case 0x36:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87EU);
        target->battle_end_state = 1;
        if ((u32) target_id <= 7U) {
            M2C_FIELD(battle_state, u8, 0x43) = (u8) (M2C_FIELD(battle_state, u8, 0x43) | 2);
        }
        break;
    case 0x35:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87FU);
        target->forced_action = 1;
        break;
    case 0x2E:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x881U);
        if ((s32) (s8) target->guard_level <= 0) {
            target->guard_level = 1;
        }
        break;
    case 0x2F:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x882U);
        if ((s32) (s8) target->guard_level <= 1) {
            target->guard_level = 2;
        }
        break;
    case 0x2D:
        BattleEvent_Push(BATTLE_EVENT_TEXT, -1U);
        break;
    }
finalize:
    BattleEvent_Push(BATTLE_EVENT_RESET, 0U);
    if (target->hp != 0) {
        status_13c_value = target->status_13c;
        if (status_13c_value != 0) {
            if ((u32) status_13c_value <= 6U) {
                if ((s32) effect_amount > 0) {
                    recovery_roll = BattleRandom_Next() & 3;
                    if (recovery_roll == 0) {
                        target->status_13c = recovery_roll;
                        BattleEvent_Push(BATTLE_EVENT_UNIT, (u32) target_id);
                        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x883U);
                    }
                }
            }
        }
    }
    ScratchFree((s16 *) snapshot);
    BattleUnit_Recalculate(target_id);
    Func_08015130(M2C_FIELD(*(void **)0x03001E74, u8, 0x41));
    if (target->hp != 0) {
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, (u32) target_id);
    }
    status_140_address = (u32)actor;
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
