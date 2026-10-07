#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "DMA.H"
#include "BTLWORK.H"
#include "TYPES.H"
#include "BATTLE_COMMAND.H"
#include "BTLSYS.H"
#include "BTLTYPE.H"
#include "CHARACTER.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_STATUS_ICON.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "VRAM_BLOCK.H"
#include "BTLEV.H"
#include "ANIMSPR.H"
#include "MOTOBJ.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE.H"
#include "UI.H"
#include "MENU_LIST.H"

#define Battle_FindTaggedSlotByValue ((s32 (*)(s32))Battle_FindTaggedSlotByValue)

struct BattleAutomaticState {
    unsigned initialized : 1;
    unsigned selection : 3;
    unsigned remaining : 28;
};

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
    struct BtlAct *item_action;
    struct BtlAct *action;
    struct BtlUnit *actor;
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

        value = BtlRndFar() & 0xff;
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

    actor = UnitGetFar(request->unit_id);
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
            auto_state->selection = BtlRndFar();
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

    item_available = (*availability >> selected) & 1;
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
        if (action->pp_cost > actor->ep && profile->target_strategy != 0)
            goto next_candidate;
        if (actor->fuuji != 0 && profile->target_strategy == 2)
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
            if (request->unit_id <= 7U)
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
            if (request->unit_id <= 7U)
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

void Dma_StopAllThenRunHook(void)
{
    Dma_Set(0, 0, 0x84000000, REG_DMA3);
    Dma_Set(0, 0, 0x84000000, REG_DMA3);
    Dma_Set(0, 0, 0x84000000, REG_DMA3);
    _call_via_r0(*(u32 *)0x030000c4);
}

void BattleEventRuntime_BeginPhase(s32 cue)
{
    struct BattleEventState *events;

    events = &BtlWk->events;
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

    events = &BtlWk->events;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = frames;
    events->phase = BATTLE_PLAYBACK_DISPATCH;
    BtlWk->plan.target_count = 0;
    return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

extern volatile u32 gFrameCount;
extern volatile s32 gFrameTick;
extern volatile s32 gKeysPressedLatch;

void Object_SetMode(void *object, s32 mode);

void BattleEvent_Playback(void)
{
    u16 selection[8];
    struct AnimationObject *records[4];
    struct BtlWork *work = BtlWk;
    struct BattleEventState *state = &work->events;

    void BattleEvent_ClearRecordTiles(struct AnimationObject *record, s32 value)
    {
        Iwram_ClearWords((void *)(0x06010000 + gVramBlockCache[record->slot].offset),
            record->width * record->height);
    }

    if (work->events.phase == 0)
        return;
    for (;;) {
        if (state->phase == 4)
            return;
        if (state->phase == 1) {
            if (state->queue.target_index < work->plan.target_count) {
                state->queue.count = 0;
                state->event_index = 0;
                state->timer = 0;
                BtlTgtExec(&work->plan, state->queue.target_index);
                state->queue.target_index++;
                state->phase = 2;
            } else {
                state->phase = 4;
            }
        } else if (state->phase == 2) {
            while (state->event_index < state->queue.count) {
                {
                    s32 i = state->event_index;

                    if (state->phase != 2)
                        break;
                    if (state->timer != 0) {
                        state->timer--;
                        return;
                    }
                    switch (state->queue.opcodes[i]) {
                    case BEV_SE:
                        AudioCommand_PlayFar(state->queue.operands[i]);
                        break;
                    case BEV_SCR:
                        Battle_SetRuntimeFlagBit0(state, state->queue.operands[i]);
                        break;
                    case BEV_UNIT:
                        UiText_DrawQuantity(state->queue.operands[i], 1);
                        break;
                    case BEV_NUM:
                        UiText_DrawQuantity(state->queue.operands[i], 5);
                        break;
                    case BEV_ITEM:
                        UiText_DrawQuantity(state->queue.operands[i] & 0x1ff, 2);
                        break;
                    case BEV_ACT:
                        UiText_DrawQuantity(state->queue.operands[i] & 0x3fff, 4);
                        break;
                    case BEV_MARK:
                        gBattleDisplayWork->marked = 1;
                        break;
                    case BEV_MES:
                        if ((s32)state->queue.operands[i] >= 0)
                            UiText_PrepareMessageWorkFar(state->queue.operands[i]);
                        state->phase = 3;
                        gKeysPressedLatch = 0;
                        break;
                    case BEV_MES_ADD:
                        if ((s32)state->queue.operands[i] >= 0)
                            UiText_PrepareMessageWorkFar(state->queue.operands[i]);
                        state->phase = 13;
                        break;
                    case BEV_CLR:
                        UiWork_ClearValueNameTablesFar();
                        break;
                    case BEV_OBJ_EFX:
                        BattleActor_DestroyTemporaryObject(state->queue.operands[i]);
                        break;
                    case BEV_OBJ_BGN:
                        if (state->pending_cue > 0)
                            AudioCommand_PlayFar(state->pending_cue);
                        state->actor_id = state->queue.operands[i];
                        Object_SetMode(BtlObjGet(state->actor_id)->object, 5);
                        state->phase = 10;
                        state->timer = 0;
                        break;
                    case BEV_OBJ_RES:
                        {
                            struct BtlUnit *unit;
                            struct AnimationObject *record;
                            s32 n;

                            state->actor_id = state->queue.operands[i];
                            BattleEnemy_RecordDefeat(state->actor_id, state->flags);
                            BattleActor_ResetRuntimeFields(state->actor_id);
                            unit = UnitGetFar(state->actor_id);
                            for (n = 0; (record = GetMotionRecord(BtlObjGet(state->actor_id)->object, n)) != 0; n++) {
                                if (unit->side != 1)
                                    Object_InitializeMode(record, 4);
                                else
                                    Object_InitializeMode(record, 5);
                            }
                            if (unit->side == 1) {
                                state->phase = 11;
                                state->timer = 0;
                            }
                        }
                        break;
                    case BEV_REDRAW:
                        PtyStDrawFar(BtlWk->party_status_mode);
                        break;
                    case BEV_OBJ_END:
                        {
                            struct BtlObj *slot;

                            BattleUnit_BuildStatusFlags(state->queue.operands[i], BtlObjGet(state->queue.operands[i]));
                            slot = BtlObjGet(state->queue.operands[i]);
                            BattleMotion_SetRecordChildValues(slot->object, BattleMotion_GetSlotPalette(state->queue.operands[i]));
                            BattlePres_SetActorModeAndAction(state->queue.operands[i]);
                        }
                        break;
                    }
                    state->event_index++;
                }
            }
            if (state->phase == 2)
                state->phase = 1;
        } else if (state->phase == 3 || state->phase == 13) {
            if (UiWork_IsCompleteFar() == 0)
                return;
            if (state->phase == 13) {
                state->phase = 2;
                state->timer = 0;
            } else {
                state->phase = 5;
                state->display_source = -1;
                state->timer = gFrameTick;
            }
        } else if (state->phase == 5) {
            struct BattlePromptSprite *entry = &state->display_entry;
            u8 *arrow = BattlePres_AdvanceArrowTiles;
            s32 tiles = (s32)arrow + ((gFrameCount >> 2) & 7) * 128;
            struct BattleDisplayWork *display = gBattleDisplayWork;
            struct UiWindow *context = display->window;
            struct BattleDisplayOffset *viewport = display->offset;
            s32 x = 0;

            if (state->display_source == -1)
                state->display_source = work->prompt_slot;
            UiWork_ClearValueNameTablesFar();
            QueueIoWriteDelay10(0x0400004a, 4);
            QueueIoWriteDelay6(0x0400004a, 16);
            ((u32 *)entry)[1] = 0xa000;
            ((u32 *)entry)[2] = x;
            entry->tile = Resource_GetBuffer(state->display_source, tiles);
            x = context->x * 8 + (viewport->x >> 8) + 4;
            entry->x = x;
            entry->y = Trig_Sin(gFrameCount << 12) / 0x8000 + context->y * 8 + (viewport->y >> 8) + 6;
            if ((BATTLE_OPTIONS & 2) || (gKeysPressedLatch & 0x303)
                || ((u32)(gFrameTick - state->timer) > 10 && (BATTLE_OPTIONS & 0x303))) {
                AudioCommand_PlayFar(111);
                state->phase = 2;
                state->timer = 0;
            } else {
                Runtime_PushSlotEntry((s32 *)entry, 240);
                return;
            }
        } else if (state->phase == 10) {
            if (state->timer & 1) {
                if (state->timer & 2) {
                    struct BtlObj *slot;

                    selection[0] = 0xff;
                    slot = BtlObjGet(state->actor_id);
                    BattleMotion_SetRecordChildValues(slot->object, BattleMotion_GetSlotPalette(state->actor_id));
                } else {
                    selection[0] = state->actor_id;
                    selection[1] = 0xff;
                    BattleMotion_SetRecordChildValues(BtlObjGet(state->actor_id)->object, 7);
                }
                BattleLayout_HighlightPartyPanelsFar(selection);
            }
            state->timer++;
            if (state->timer <= 8)
                return;
            state->phase = 2;
            state->timer = 0;
        } else if (state->phase == 11) {
            if (state->timer == 0 || state->timer >= 0x400) {
                s32 frame = 6;

                if (state->timer == 0 && state->flags != 0) {
                    s32 kind = Summon_GetLayoutKind(UnitGetFar(state->actor_id)->class_id);

                    if (kind >= 0) {
                        kind--;
                        if (kind < 0)
                            kind = 0;
                        AudioCommand_PlayFar(kind + 146);
                    }
                    state->timer = 0x400;
                }
                if (state->timer > 0x41d)
                    state->timer = 0;
                if (state->timer == 0) {
                    s32 kind = Summon_GetLayoutKind(UnitGetFar(state->actor_id)->class_id);

                    if (kind >= 0)
                        AudioCommand_PlayFar(kind + 146);
                }
                if (state->timer >= 0x400)
                    frame = (state->timer - 0x400) / 8 % 5 + 1;
                if (frame == 6 || (state->timer & 7) == 0) {
                    struct AnimationObject *record;
                    s32 n;
                    struct AnimationObject **slot;
                    s32 none;

                    for (n = 0, slot = records, none = 0xff; (record = GetMotionRecord(BtlObjGet(state->actor_id)->object, n)) != 0; n++) {
                        struct AnimationEntry *child;
                        u8 old;

                        *slot++ = record;
                        child = record->entries[0];
                        old = child->frame;
                        child->param = frame;
                        child->frame = old | none;
                    }
                }
                state->timer++;
                return;
            }
            if (state->timer == 4) {
                BattleActor_RemoveFromLists(state->actor_id);
                state->timer++;
                return;
            }
            if (state->timer > 4) {
                struct BtlObj *slot;
                struct AnimationObject *record;
                s32 count;
                s32 frame;
                s32 base;

                slot = BtlObjGet(state->actor_id);
                slot->fading = 1;
                for (count = 0; (record = GetMotionRecord(slot->object, count)) != 0; count++)
                    records[count] = record;
                base = state->timer * 4;
                frame = base - 20;
                if (frame > 127) {
                    s32 k;

                    for (k = 0; k < count; k++)
                        BattleEvent_ClearRecordTiles(records[k], 0);
                    ActivateBattleObjectSlot(state->actor_id);
                    state->phase = 2;
                    state->timer = 0;
                    return;
                }
                {
                    s32 k;

                for (k = 0; k < count; k++) {
                    render_animated_tile_frameFar((u8 *)records[k], frame);
                    render_animated_tile_frameFar((u8 *)records[k], base - 19);
                    render_animated_tile_frameFar((u8 *)records[k], base - 18);
                    render_animated_tile_frameFar((u8 *)records[k], base - 17);
                }
                }
                state->timer++;
                return;
            }
            state->timer++;
            return;
        }
    }
}
