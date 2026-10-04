#include "DMA.H"
#include "BATTLE_WORK.H"
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "CHARACTER.H"

struct EnemyDefinition *Owner_GetRecordFar(s32 class_id);
struct BattleAction *Ability_GetData(s32 action_id);
s32 Battle_FindTaggedSlotByValue(s32 actor_id);
s32 BattleTarget_SelectForAction(s32 actor_id, struct BattleAction *action);
s32 BattleTarget_SelectRandomPosition(s32 side);
extern u8 BattleCommand_SelectionWeights[3][8];

/* Low bits of the combatant word copied from its enemy definition. */
struct BattleAutomaticState {
    unsigned initialized : 1;
    unsigned selection : 3;
    unsigned remaining : 28;
};

/*
 * Choose a legal automatic command for one combatant.  The class profile at
 * Owner_GetRecordFar supplies a strategy byte at +0x36, availability bits at
 * +0x37, and the candidate action IDs beginning at +0x38.  The weighted
 * selector is a GCC nested function: each call passes the frame in r9.
 */
s32 BattleCommand_SelectAutomatic(
    struct BattleActionRecord *request,
    s32 retry)
{
    register u32 selected;
    struct BattleAutomaticState *auto_state;
    struct EnemyDefinition *profile;
    s32 allow_item;
    s32 attempt;
    s32 keep_going;
    s8 *selection_mode;
    u8 *availability;
    s16 command;
    s32 target;
    s32 alternate_target;
    s32 side;
    s32 alternate_side;
    struct ItemDefinition *item;
    s32 item_available;
    s8 mode;
    struct BattleAction *item_action;
    struct BattleAction *action;
    struct BattleUnit *actor;
    s32 action_id;
    s32 target_flags;
    u8 effect;
    u8 target_mode;
    s32 Battle_SelectWeightedIndex(u8 *weights)
    {
        s32 value;
        s32 total;
        s32 result;
        s32 index;

        value = BattleRandom16Far() & 0xff;
        total = weights[0];
        result = 0;
        index = 0;
        if (value >= total) {
        loop:
            index++;
            if (index <= 7) {
                total += weights[index];
                if (value < total)
                    result = index;
                else
                    goto loop;
            }
        }
        return result;
    }

    actor = Owner_GetStateFar(request->unit_id);
    keep_going = allow_item = 1;
    attempt = 0;
    selected = -1U;
    if (actor->class_index != 0)
        return;
    if (retry != 0 && request->command != 4)
        return;

    profile = Owner_GetRecordFar(actor->class_id);
    auto_state = (struct BattleAutomaticState *)&actor->rewards;
    selection_mode = &profile->action_strategy;
    availability = &profile->item_actions;

select_again:
    mode = *selection_mode;
    switch (mode) {
    case 0:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[0]);
        break;
    case 1:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[1]);
        break;
    case 2:
        selected = Battle_SelectWeightedIndex(BattleCommand_SelectionWeights[2]);
        break;
    case 3:
        if (auto_state->initialized == 0) {
            auto_state->selection = BattleRandom16Far();
            auto_state->initialized = 1;
        }
read_saved_selection:
        selected = auto_state->selection;
        if (retry != 0) {
            auto_state->selection = selected + 1;
        }
        break;
    case 4:
        goto read_saved_selection;
    case 5:
        selected++;
        break;
    case 6:
        break;
    }

    item_available = ((s32)*availability >> selected) & 1;
    item_available &= allow_item;
    action_id = profile->actions[selected];
    request->command = 4;
    if (item_available != 0 && retry != 0) {
        if (!(0x1ff & actor->inventory[0])) {
            item_available = 0;
            if (profile->target_strategy == 0) {
                request->command = 2;
                request->parameter = 0x1fd;
                return;
            }
        }
        if (item_available != 0) {
            item = Item_Get(actor->inventory[0]);
            if (item->use_type == 1) {
                item_action = Ability_GetData(item->action_id);
                action_id = item->action_id;
                request->command = 2;
                target_flags = item_action->target_flags;
                request->parameter = 0;
                switch (target_flags) {
                case 1:
                case 2:
                    goto item_checked;
                }
            }
            item_available = 0;
item_checked:
            if (item_available == 0)
                goto disable_item;
        } else {
disable_item:
            allow_item = 0;
        }
    }

    if (keep_going == 0)
        goto finish_target;

    action = Ability_GetData(action_id);
    effect = action->effect;
    switch (effect) {
    case 46:
        request->command = 3;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    case 47:
        request->command = 7;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    case 49:
        request->command = 99;
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    }

    if (retry == 0) {
        switch (request->command) {
        case 3:
        case 7:
            break;
        default:
            goto done;
        }
    }

    if (request->command != 2 && Ability_CheckStatusOrSpecialId(action_id) != 0) {
        request->command = 1;
        request->parameter = action_id;
        if (action->pp_cost > actor->pp && profile->target_strategy != 0)
            goto next_candidate;
        if (actor->psy_seal != 0 && profile->target_strategy == 2)
            goto next_candidate;
    }
    command = request->command;
    if (command == 99 && (u8)actor->battle_end_state != 0)
        goto next_candidate;
    if (retry != 0 && (command == 3 || command == 7))
        goto next_candidate;

    if (command == 4) {
        request->parameter = action_id;
        if (action_id == 1)
            request->command = 0;
    }

    request->range = action->range;
    target_mode = action->target_mode;
    switch (target_mode) {
    case 2:
    case 4:
        target = BattleTarget_SelectForAction(request->unit_id, action);
        if (target == -2) {
            side = 0;
            if ((u32)(u16)request->unit_id <= 7U)
                side = 1;
            target = BattleTarget_SelectRandomPosition(side);
        }
        if (target != -1) {
            request->target = target;
            keep_going = 0;
        }
        break;
    case 1:
        alternate_target =
            BattleTarget_SelectForAction(request->unit_id, action);
        if (alternate_target == -2) {
            alternate_side = 0;
            if ((u32)(u16)request->unit_id <= 7U)
                alternate_side = 1;
            alternate_target = BattleTarget_SelectRandomPosition(alternate_side);
        }
        if (alternate_target != -1) {
            request->target = alternate_target;
            keep_going = 0;
        }
        break;
    case 3:
        request->target = Battle_FindTaggedSlotByValue(request->unit_id);
        break;
    default:
        alternate_target = Battle_FindTaggedSlotByValue(request->unit_id);
        request->target = alternate_target;
        keep_going = 0;
        break;
    }

finish_target:
    if (retry == 0)
        keep_going = 0;
next_candidate:
    if (keep_going != 0 && attempt > 16) {
        request->command = 3;
        keep_going = 0;
    }
    attempt++;
    if (keep_going != 0)
        goto select_again;
done:
    return;
}

/* The container-built _call_via_r0 veneer: it calls the routine in r0. */
void _call_via_r0(u32 routine);

void BattleEvent_Playback(void);

/* Writes three empty transfers to the DMA3 registers, then runs the hook
   stored at 0x030000c4. */
void Dma_StopAllThenRunHook(void)
{
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    _call_via_r0(*(u32 *)0x030000c4);
}

void BattleEventRuntime_BeginPhase(s32 cue)
{
    struct BattleEventState *events;

    events = &gBattleWork->events;
    if (events->phase == BATTLE_PLAYBACK_IDLE) {
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
        if (cue != 0) {
            events->pending_cue = cue;
        }
    }
}

s32 BattleEventRuntime_SchedulePhase(s32 frames)
{
    struct BattleEventState *events;

    events = &gBattleWork->events;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = frames;
    events->phase = BATTLE_PLAYBACK_DISPATCH;
    gBattleWork->plan.target_count = 0;
    return Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
}
