#include "types.h"
#include "battle_command.h"
#include "battle_runtime.h"
#include "battle_types.h"

void *Func_08077198(s32 class_id);
void *Func_08077018(u16 item_id);
u32 Func_080bd3c8(s32 action_id);
s32 Func_080b9a70(u32 actor_id);
s32 Func_080bae40(s32 actor_id, struct BattleAction *action);
s32 Func_080bad7c(s32 side);
s32 Func_080bd3e4(s32 table);

#define OWNER_FIELD(base, type, offset) \
    (*(type *)((u8 *)(base) + (offset)))

#ifndef BATTLE_COMMAND_SELECT_OWNER
#define BATTLE_COMMAND_SELECT_OWNER Func_080b4424
#endif

/*
 * Choose a legal automatic command for one combatant.  The class profile at
 * Func_08077198 supplies a strategy byte at +0x36, availability bits at
 * +0x37, and the candidate action IDs beginning at +0x38.
 */
void BATTLE_COMMAND_SELECT_OWNER(
    struct BattleCommandRequest *request,
    s32 retry)
{
    register u32 selected;
    register u32 *auto_state;
    u8 *availability;
    s8 *selection_mode;
    s32 keep_going;
    s32 attempt;
    s32 allow_item;
    void *profile;
    s32 retry_mode;
    s16 command;
    s16 manual_command;
    s16 target;
    s16 alternate_target;
    s32 side;
    s32 alternate_side;
    s32 weights;
    s32 item_available;
    s8 mode;
    struct BattleAction *item_action;
    struct BattleAction *action;
    struct BattleUnit *actor;
    u16 current_command;
    u16 action_id;
    u8 target_flags;
    u8 effect;
    u8 target_mode;
    void *item;
    selected = -1U;
    retry_mode = retry;
    actor = Func_08077008(request->actor_id);
    attempt = 0;
    allow_item = 1;
    keep_going = 1;
    if (actor->class_bonus_disabled != 0)
        return;
    if (retry_mode != 0 && request->command != 4)
        return;

    profile = Func_08077198(actor->class_id);
    selection_mode = profile + 0x36;
    availability = profile + 0x37;
    auto_state = (u32 *)((u8 *)actor + 0x120);

select_again:
    mode = *selection_mode;
    switch (mode) {
    case 0:
        weights = 0x080c2b80;
select_weighted:
        selected = Func_080bd3e4(weights);
        break;
    case 1:
        weights = 0x080c2b88;
        goto select_weighted;
    case 2:
        weights = 0x080c2b90;
        goto select_weighted;
    case 3:
        if ((*(s32 *)auto_state << 31) == 0) {
            actor->unknown_0f6[0x2a] =
                (-15 & actor->unknown_0f6[0x2a])
                | ((Func_080771a0() & 7) * 2) | 1;
        }
read_saved_selection:
        selected =
            (u32)(*(s32 *)auto_state << 28) >> 29;
        if (retry_mode != 0) {
            actor->unknown_0f6[0x2a] =
                (-15 & actor->unknown_0f6[0x2a])
                | (((selected + 1) & 7) * 2);
        }
        break;
    case 4:
        goto read_saved_selection;
    case 5:
        selected++;
        break;
    }

    item_available = ((s32)*availability >> selected) & 1 & allow_item;
    action_id = OWNER_FIELD(profile, u16, selected * 2 + 0x38);
    request->command = 4;
    if (item_available != 0 && retry_mode != 0) {
        if (!(0x1ff & actor->inventory[0])) {
            item_available = 0;
            if ((s8)OWNER_FIELD(profile, u8, 0x35) == 0) {
                request->command = 2;
                request->parameter = 0x1fd;
                return;
            }
        }
        if (item_available != 0) {
            item = Func_08077018(actor->inventory[0]);
            if (OWNER_FIELD(item, u8, 0x0c) != 1
                || (item_action =
                        Func_08077080(OWNER_FIELD(item, u16, 0x28)),
                    request->command = 2,
                    target_flags = item_action->target_flags,
                    action_id = OWNER_FIELD(item, u16, 0x28),
                    request->parameter = 0,
                    target_flags > 2)
                || target_flags < 1) {
                item_available = 0;
            }
            if (item_available == 0)
                goto disable_item;
        } else {
disable_item:
            allow_item = 0;
        }
    }

    if (keep_going == 0)
        goto finish_target;

    action = Func_08077080(action_id);
    effect = action->effect;
    switch (effect) {
    case 46:
        request->command = 3;
set_actor_target:
        request->unknown_0a = Func_080b9a70(request->actor_id);
        break;
    case 47:
        request->command = 7;
        goto set_actor_target;
    case 49:
        request->command = 99;
        goto set_actor_target;
    }

    if (retry_mode == 0) {
        manual_command = request->command;
        current_command = request->command;
        if (manual_command != 3 && manual_command != 7)
            return;
    } else {
        current_command = request->command;
    }

    if ((current_command << 16) != 0x20000) {
        if (Func_080bd3c8(action_id) != 0) {
            request->command = 1;
            request->parameter = action_id;
            if (action->pp_cost > actor->pp
                && (s8)OWNER_FIELD(profile, u8, 0x35) != 0) {
                /* This candidate action is rejected below. */
            } else if (actor->psy_seal != 0) {
                if ((s8)OWNER_FIELD(profile, u8, 0x35) == 2) {
                    /* This candidate action is rejected below. */
                } else {
                    goto command_ready;
                }
            } else {
command_ready:
                current_command = 1;
                goto validate_command;
            }
        } else {
            current_command = request->command;
            goto validate_command;
        }
    } else {
validate_command:
        command = current_command;
        if (command != 99 || (u8)actor->battle_end_state == 0) {
            if (retry_mode != 0) {
                if (command != 3 && command != 7)
                    goto prepare_target;
            } else {
prepare_target:
                if (command == 4) {
                    request->parameter = action_id;
                    if (action_id == 1)
                        request->command = 0;
                }

                OWNER_FIELD(request, s16, 0x0c) = action->range;
                target_mode = action->target_mode;
                switch (target_mode) {
                case 2:
                case 4:
                    target = Func_080bae40(request->actor_id, action);
                    if (target == -2) {
                        side = 0;
                        if ((u32)(u16)request->actor_id <= 7U)
                            side = 1;
                        target = Func_080bad7c(side);
                    }
                    if (target != -1) {
                        request->unknown_0a = target;
                        keep_going = 0;
                    }
                    break;
                case 1:
                    alternate_target =
                        Func_080bae40(request->actor_id, action);
                    if (alternate_target == -2) {
                        alternate_side = 0;
                        if ((u32)(u16)request->actor_id <= 7U)
                            alternate_side = 1;
                        alternate_target = Func_080bad7c(alternate_side);
                    }
                    if (alternate_target != -1) {
set_target:
                        request->unknown_0a = alternate_target;
                        keep_going = 0;
                    }
                    break;
                case 3:
                    request->unknown_0a = Func_080b9a70(request->actor_id);
                    break;
                default:
                    alternate_target = Func_080b9a70(request->actor_id);
                    goto set_target;
                }
finish_target:
                if (retry_mode == 0)
                    keep_going = 0;
            }
        }
    }

    if (keep_going != 0 && attempt > 16) {
        request->command = 3;
        keep_going = 0;
    }
    attempt++;
    if (keep_going != 0)
        goto select_again;
}
