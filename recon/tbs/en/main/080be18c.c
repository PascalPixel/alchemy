/*
 * Draft: BattleCommand_BuildPlan with its nested target selector.
 * Measured 2026-10-02 with the ordinary TBS compiler plan in all six editions.
 * Complete native/source bank: 4220 bytes, including literal pools; the
 * selector is 492 bytes and BuildPlan is 3728 bytes. Removing the unread
 * eight-byte local keeps those extents: selector 0 differences, BuildPlan
 * 14 differing bytes in every edition. The frame becomes 40 rather than 48
 * bytes; frame-size and static-chain stack offsets differ. All 283 symbolic
 * call, table and pool relocations per edition retain their native values.
 * No assembly, register pin, extra storage or compiler option is retained.
 * This is uncredited work, not an exact match or a sound return contract.
 *
 * C contract limitation: BattleCommand_SelectTargets returns -1 on failure
 * but falls off its s32 body on success. Its callers inspect the result;
 * that successful path has undefined behavior in C. Adding an explicit
 * return 0 was measured, not retained: selector 496, bank 4224, with 31
 * selector and 245 BuildPlan byte differences at each function's boundary.
 * There is no claim of executable equivalence for this draft.
 *
 * Eight ordinary forms after deletion, EN selector/main sizes and differences:
 * remove array:      492/3728, 0/14 (retained)
 * explicit success:  496/3728, 31/245; whole shifted bank 3179 differences
 * counts first:      492/3728, 0/14
 * later list scope:  492/3728, 0/14
 * used work record:  492/3728, 0/1371; every field had a real use
 * while requirement: 492/3728, 0/14
 * conditional side:  492/3728, 0/14
 * requirement view:  492/3704, 0/2021
 * The original source baseline matched all 4220 bytes but required the
 * unread eight-byte array; it is not proposed for retention or credit.
 */

#include "TYPES.H"
#include "SYSTEM.H"
#include "BATTLE_UNIT.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_CALC.H"
#include "BATTLE_MSG.H"
#include "MOTION_OBJECT.H"
#include "ITEM.H"

/* A Djinni in a command's parameter: element in bits 8-11, index below. */
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v)   ((v) & 255)

struct BattleAction *Ability_GetData(s32 action);
s32 Battle_GetTaggedSlotValue(s32 target);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowMessageAndWaitCoreFar(s32 message);
void UiWork_ClearValueNameTablesFar(void);
s32 GameFlag_IsSet(s32 flag);
s32 Owner_AdjustFirstValueFar(s32 unit_id, s32 amount);
u32 BattleEv_Push(u32 opcode, u32 operand);
void BattleEv_DispatchQueued(void);
u16 RollWeaponUnleashFar(struct BattleUnit *unit);
s32 Inventory_GetEquippedItemFar(struct BattleUnit *unit, s32 slot);
void BattleEv_SetRuntimeField8(void);
struct ItemDefinition *Item_Get(s32 item);
s32 Djinn_GetDefinitionHeaderFar(s32 element, s32 index);
s32 Djinn_IsActiveFar(s32 unit_id, s32 element, s32 index);
s32 Trade_CanOfferDjinnFar(s32 unit_id, s32 element, s32 index);
void BattlePres_SetActorModes(u16 *ids, s32 mode);
void Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
void Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
void Trade_AddOfferFar(s32 unit_id, s32 element, s32 index);
void Owner_RecalculateStatsFar(s32 unit_id);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void Object_SetMode(struct MotionObject *object, s32 mode);
void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object, s32 value);
void BattleFx_PlayUnitElementEffect(s32 unit_id, s32 element, s32 mode, s32 flag);
s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts);
/* The standby Djinn of one side, as BattlePlacement_CountValidEntries reads
   them: a state of -1 is ready, 254 is spent by a summon. */
struct PlacementEntry {
    u8 element;
    u8 unknown_01;
    u8 object_id;
    s8 state;
};

struct PlacementList {
    struct PlacementEntry entries[64];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};

struct PlacementTable *Trade_GetOfferStateFar(s32 side);
u32 BattleParty_IsUnitListed(u32 unit_id);
s32 Item_GetEquippedElementFar(s32 unit_id);
u32 Battle_GetEntryField2LowBits(u32 class_id);
s32 Equipment_GetUnleashRateBonusFar(struct BattleUnit *unit);
u32 Ability_CheckStatusOrSpecialId(s32 action);
void BattlePresentation_WaitForAdvance(void);

/* Resolve a queued command into the plan its presentation plays: who acts,
 * with what, on whom, and how the first rolls fall. Returns -1 when the
 * command comes to nothing after a message, -2 when it was played out here. */
s32 BattleCommand_BuildPlan(struct BattleActionRecord *command, struct BattlePlan *plan)
{
    struct BattleUnit *actor = Owner_GetStateFar(command->unit_id);
    struct BattleSession *battle = gBattleWork;
    s32 target;
    s32 action;

    /* Fill the plan's targets for an action: the chosen unit alone, or every
     * listed unit within the command's range of its position on the chosen
     * side. Defeated units count only for the reviving effects. Returns -1,
     * after telling the player, when nobody is in range. */
    s32 BattleCommand_SelectTargets(s32 action_id)
    {
        struct BattleAction *action = Ability_GetData(action_id);
        s32 mode = action->target_mode;
        s32 allow_defeated = 0;
        s32 count;
        s32 allies;
        s32 enemies;
        s32 center;
        s32 range;
        s32 index;
        s32 unit;

        switch (action->effect) {
        case 5:
        case 56:
        case 57:
            allow_defeated = 1;
            break;
        }
        switch (mode) {
        case 0:
            plan->target_offsets[0] = mode;
            plan->target_count = 1;
            plan->target_ids[0] = target;
            plan->target_adjustments[0] = 1;
            break;
        case 4:
            plan->target_offsets[0] = 0;
            plan->target_count = 1;
            plan->target_ids[0] = target;
            plan->target_adjustments[0] = 1;
            break;
        default:
            count = 0;
            for (index = 0; battle->party_units[index] != 255; index++)
                ;
            allies = index;
            for (index = 0; battle->enemy_units[index] != 255; index++)
                ;
            enemies = index;
            center = command->target & 15;
            range = command->range;
            {
                s32 first = center - range + 1;
                s32 last = center + range - 1;

                for (index = first; index <= last; index++) {
                    if (index < 0)
                        continue;
                    if (command->target & 128) {
                        if (index >= enemies)
                            continue;
                        unit = battle->enemy_units[index];
                        if (unit == 254)
                            continue;
                        if (!allow_defeated && Owner_GetStateFar(unit)->hp == 0)
                            continue;
                        plan->target_adjustments[count] = 1;
                        plan->target_offsets[count] = index - center;
                        plan->target_ids[count] = unit;
                        count++;
                    } else {
                        if (index >= allies)
                            continue;
                        unit = battle->party_units[index];
                        if (unit == 254)
                            continue;
                        if (!allow_defeated && Owner_GetStateFar(unit)->hp == 0)
                            continue;
                        plan->target_adjustments[count] = 1;
                        plan->target_offsets[count] = index - center;
                        plan->target_ids[count] = unit;
                        count++;
                    }
                }
            }
            plan->target_count = count;
            if (count <= 0) {
                UiText_DrawQuantity(command->unit_id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
                if (actor->guard_level == 0)
                    actor->guard_level = 1;
                return -1;
            }
        }
    }

    target = Battle_GetTaggedSlotValue(command->target);
    BattleEventRuntime_Reset();
    plan->pending_amount_60 = 0;
    plan->actor_id = command->unit_id;
    plan->target_count = 0;
    plan->presentation_flags = 0;
    plan->failure = 0;
    plan->range_index = 4;
    UiWork_ClearValueNameTablesFar();
    if (actor->hp == 0)
        return -2;
    if (BATTLE_AUTO_MODE && GameFlag_IsSet(0x16d) && (BATTLE_OPTIONS & 0x100)) {
        s32 enemy_side = 1;
        s32 i;
        s32 unit;

        if (BATTLE_OPTIONS & 4)
            enemy_side = 0;
        for (i = 0; ; i++) {
            if (enemy_side)
                unit = battle->enemy_units[i];
            else
                unit = battle->party_units[i];
            if (unit == 255)
                break;
            if (unit == 254)
                continue;
            if (Owner_AdjustFirstValueFar(unit, 0xc0000000) == 0) {
                BattleEv_Push(8, unit);
                BattleEv_Push(9, unit);
            }
        }
        BattleEv_DispatchQueued();
        return -2;
    }
    UiWork_ClearValueNameTablesFar();
    if (actor->cannot_move) {
        actor->cannot_move = 0;
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgUnableToMove);
        return -1;
    }
    if (actor->sleep) {
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgIsAsleep);
        return -1;
    }
    if (actor->stun) {
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgIsParalyzed);
        return -1;
    }
    if ((actor->restraint & 1) && command->command != 3 && (BattleRandom16Far() & 3) == 0) {
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgIsBound);
        return -1;
    }
    if (command->command == 8)
        return -2;
    action = 1;
    {
        s32 i;

        for (i = 0; i < 14; i++)
            plan->target_modifiers[i] = 0;
        for (i = 0; i < 14; i++)
            plan->target_results[i] = -1;
    }
    switch (command->command) {
    case 99:
        if ((u16)command->unit_id <= 7) {
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgPartyFlees);
        } else {
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorRuns);
        }
        BattlePresentation_WaitForAdvance();
        plan->outcome = 7;
        return 0;
    case 0:
        action = RollWeaponUnleashFar(actor);
        if (BattleCommand_SelectTargets(action) == -1)
            return -1;
        if (action != 1) {
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(Inventory_GetEquippedItemFar(actor, 1), 2);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgWeaponHowls);
            BattleEv_SetRuntimeField8();
            UiText_DrawQuantity(action, 4);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgWeaponHowls + 1);
        }
        break;
    case 1:
        {
            struct BattleAction *ability;
            s32 ok = 1;

            action = command->parameter;
            ability = Ability_GetData(action);
            if (BattleCommand_SelectTargets(action) == -1)
                return -1;
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(action, 4);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorCasts);
            if (actor->pp < ability->pp_cost) {
                plan->failure = 2;
                ok = 0;
            }
            if (actor->psy_seal) {
                plan->failure = 1;
                ok = 0;
            }
            if (ok) {
                plan->failure = 0;
                actor->pp -= ability->pp_cost;
                Owner_RecalculateRatiosFar(command->unit_id);
                if (actor->pp < 0)
                    actor->pp = 0;
                if (actor->pp > actor->max_pp)
                    actor->pp = actor->max_pp;
            }
        }
        break;
    case 2:
        {
            struct ItemDefinition *item;

            if (command->parameter < 0) {
                UiText_DrawQuantity(command->unit_id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgItemAlreadyUsed);
                return -1;
            }
            item = Item_Get(actor->inventory[command->parameter]);
            action = item->action_id;
            if (action == 0 || (actor->inventory[command->parameter] & 0x400)) {
                UiText_DrawQuantity(command->unit_id, 1);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
                if (actor->guard_level == 0)
                    actor->guard_level = 1;
                return -1;
            }
            if (BattleCommand_SelectTargets(action) == -1)
                return -1;
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(actor->inventory[command->parameter], 2);
            if (item->use_type == 2 || item->use_type == 0) {
                switch (item->type) {
                case 1:
                case 3:
                case 6:
                case 7:
                case 8:
                    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorRaisesItem);
                    goto resolve;
                }
            }
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorUsesBattleItem);
        }
        break;
    case 3:
    case 7:
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
        return -1;
    case 8:
        return -2;
    case 4:
        {
            s32 message;

            action = command->parameter;
            if (BattleCommand_SelectTargets(action) == -1)
                return -1;
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(action, 4);
            if ((Ability_GetData(action)->target_flags & 15) == 6)
                message = (s32)&MsgActorUnleashesAbility;
            else
                message = (s32)&MsgActorUsesAbility;
            switch (action) {
            case 435:
            case 436:
                message = (s32)&MsgEmitsUltrasonicWaves;
                break;
            case 224:
                message = (s32)&MsgActorCasts;
                break;
            case 500:
                message = (s32)&MsgGlowersFerociously;
                break;
            case 501:
                message = (s32)&MsgIsIntimidated;
                break;
            case 499:
                message = (s32)&MsgBalefulGaze;
                break;
            case 494:
                message = (s32)&MsgEatsWorms;
                break;
            case 437:
            case 438:
            case 439:
            case 440:
            case 441:
                message = (s32)&MsgLetsOutAbility;
                break;
            case 442:
            case 443:
            case 444:
                message = (s32)&MsgActorUsesAbility;
                break;
            case 472:
                message = (s32)&MsgEnemyUnleashesAbility;
                break;
            case 488:
                message = (s32)&MsgGlowersMiserably;
                break;
            case 492:
                message = (s32)&MsgSmellOfDecay;
                break;
            case 495:
                message = (s32)&MsgFuriousRage;
                break;
            case 503:
                message = (s32)&MsgAttemptsToDivide;
                break;
            case 504:
                message = (s32)&MsgLooksForAllies;
                break;
            case 508:
                message = (s32)&MsgLooksForHelp;
                break;
            }
            UiText_ShowMessageAndWaitCoreFar(message);
        }
        break;
    case 5:
        action = Djinn_GetDefinitionHeaderFar(DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter));
        if (!Djinn_IsActiveFar(command->unit_id, DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter))) {
            if (Trade_CanOfferDjinnFar(command->unit_id, DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter))) {
                Ability_GetData(action);
                BattlePres_SetActorModes(0, 0);
                Djinn_ActivateFar(command->unit_id, DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter));
                Trade_RemoveOfferFar(command->unit_id, DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter));
                Owner_RecalculateStatsFar(command->unit_id);
                BattleEventRuntime_Reset();
                BattleEventRuntime_SchedulePhase(30);
                BattleEv_Push(0, command->unit_id);
                BattleEv_Push(3, DJINN_ELEMENT(command->parameter) * 20 + DJINN_INDEX(command->parameter) + 300);
                BattleEv_Push(14, 175);
                BattleEv_Push(10, 0);
                BattleEv_Push(4, (s32)&MsgDjinnSet);
                BattleEv_Push(11, command->unit_id);
                AudioCommand_PlayFar(212);
                Object_SetMode(GetBattleObjectSlot(command->unit_id)->object, 3);
                ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(command->unit_id)->object, 32);
                BattleFx_PlayUnitElementEffect(command->unit_id, DJINN_ELEMENT(command->parameter), 3, 0);
                BattleEventRuntime_WaitForReady();
                return -2;
            }
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(action, 4);
            AudioCommand_PlayFar(114);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgDjinnInRecovery);
            WaitFrames(60);
            return -1;
        }
        {
            struct BattleAction *ability;

            if (BattleCommand_SelectTargets(action) == -1)
                return -1;
            Trade_AddOfferFar(command->unit_id, DJINN_ELEMENT(command->parameter), DJINN_INDEX(command->parameter));
            ability = Ability_GetData(action);
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(action, 4);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorUnleashesDjinn);
            plan->range_index = ability->damage_class;
        }
        break;
    case 6:
        {
            const struct SummonDefinition *summon;
            struct PlacementList *list;
            s32 side;
            u8 counts[4];
            s32 i;

            summon = SummonDefinition_Get(command->parameter);
            BattlePlacement_CountValidEntries(command->unit_id, counts);
            side = 0;
            if ((u16)command->unit_id > 7)
                side = 1;
            list = &Trade_GetOfferStateFar(side)->list;
            for (i = 0; i < 4 && counts[i] >= summon->djinn_required[i]; i++)
                counts[i] = summon->djinn_required[i];
            action = summon->name_message_id;
            if (BattleCommand_SelectTargets(action) == -1)
                return -1;
            if (i != 4) {
                UiText_DrawQuantity(command->unit_id, 1);
                UiText_DrawQuantity(action, 4);
                UiText_ShowMessageAndWaitCoreFar((s32)&MsgSummonLacksDjinn);
                return -1;
            }
            UiText_DrawQuantity(command->unit_id, 1);
            UiText_DrawQuantity(action, 4);
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorSummons);
            for (i = 0; i != list->count; i++) {
                struct PlacementEntry *entry = &list->entries[i];

                if (entry->state == -1 && BattleParty_IsUnitListed(entry->object_id) && counts[entry->element] != 0) {
                    entry->state = 254;
                    counts[entry->element]--;
                }
            }
        }
        break;
    }
resolve:
    if (action == 1) {
        struct BattleUnit *defender = Owner_GetStateFar(plan->target_ids[0]);

        plan->action_id = 1;
        plan->range_index = Item_GetEquippedElementFar(command->unit_id);
        plan->outcome = 2;
        if (actor->class_index == 0) {
            plan->presentation_flags = Battle_GetEntryField2LowBits(actor->class_id) | 0x4000;
        } else {
            plan->presentation_flags = 0;
            switch (actor->class_id) {
            case 0:
                plan->presentation_flags = 0x4001;
                break;
            case 1:
                plan->presentation_flags = 0x4001;
                break;
            case 2:
                plan->presentation_flags = 0x4004;
                break;
            case 3:
                plan->presentation_flags = 0x4004;
                break;
            case 5:
                plan->presentation_flags = 0x4001;
                break;
            }
        }
        UiText_DrawQuantity(command->unit_id, 1);
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorAttacks);
        do {
            if (defender->hp == 0 || defender->sleep != 0 || defender->stun != 0
                || defender->cannot_move != 0 || defender->charm != 0)
                break;
            if (actor->delusion != 0 && (BattleRandom16Far() & 255) <= 152)
                plan->target_adjustments[0] = 0;
            if ((BattleRandom16Far() & 31) == 0)
                plan->target_adjustments[0] = 0;
        } while (0);
        if (GameFlag_IsSet(366))
            plan->target_adjustments[0] = 0;
        if (defender->hp != 0) {
            if ((BattleRandom16Far() & 31) == 0)
                plan->target_modifiers[0] = 1;
            else if ((Equipment_GetUnleashRateBonusFar(actor) << 16) / 200 > (BattleRandom16Far() & 0xffff))
                plan->target_modifiers[0] = 1;
        }
    } else {
        struct BattleAction *ability = Ability_GetData(action);

        plan->range_index = ability->damage_class;
        plan->presentation_flags = 0;
        plan->action_id = action;
        if (ability->effect == 65 || ability->effect == 41 || ability->effect == 42
            || ability->effect == 43 || ability->effect == 44 || ability->effect == 68) {
            s32 chance;
            s32 amount;

            if (ability->effect == 65 || ability->effect == 68)
                chance = 153;
            else if (ability->effect == 41 || ability->effect == 43)
                chance = 32;
            else
                chance = 64;
            if (ability->effect == 65 || ability->effect == 41 || ability->effect == 42)
                amount = 1;
            else
                amount = 2;
            if ((BattleRandom16Far() & 255) < chance) {
                s32 i;

                for (i = 0; i < plan->target_count; i++)
                    plan->target_adjustments[i] += amount;
            }
        } else if (ability->effect >= 36 && ability->effect <= 40) {
            s32 mask;

            switch (ability->effect) {
            case 36:
                mask = 63;
                break;
            case 37:
                mask = 31;
                break;
            case 38:
                mask = 15;
                break;
            case 39:
                mask = 7;
                break;
            case 40:
            default:
                mask = 3;
                break;
            }
            if ((BattleRandom16Far() & mask) == 0) {
                s32 i;

                for (i = 0; i < plan->target_count; i++)
                    plan->target_modifiers[i] = 2;
            }
        } else if (action == 178) {
            s32 i;

            for (i = 0; i < plan->target_count; i++)
                plan->target_results[i] = Battle_HitCheck(command->unit_id, plan->target_ids[i],
                    ability->damage_class, ability->effect, 100);
        }
        if ((u32)action <= 0x206) {
            plan->presentation_flags = Battle_ActionFlags[action];
            if (plan->target_adjustments[0] > 1)
                plan->presentation_flags = Battle_ActionFlags[action] + (plan->target_adjustments[0] << 12) - 0x1000;
        }
        if ((u32)action <= 0x205 && Battle_ActionStatus[action] != 0)
            plan->outcome = Battle_ActionStatus[action];
        else if (Ability_CheckStatusOrSpecialId(action))
            plan->outcome = 3;
        else if (plan->presentation_flags != 0) {
            if (actor->class_index == 0)
                plan->outcome = 8;
            else
                plan->outcome = 3;
        } else
            plan->outcome = 1;
        if (BattleFx_IsReviveFar(ability->effect))
            plan->presentation_flags |= 0x10000;
        if (action == 178 && plan->target_results[0] != 0)
            plan->presentation_flags |= 0x1000;
    }
    if (command->command == 2 && plan->outcome != 5 && plan->outcome != 9)
        plan->outcome = 4;
    plan->command = command->command;
    return 0;
}
