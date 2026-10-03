/* DRAFT: BattleEvent_Playback and its nested tile clearer, covering the
 * native 1948-byte bank: 72-byte clearer and 1876-byte playback, including
 * the jump table, literal pools and alignment. No production adoption.
 * Earlier combined attempt reported 1948 bytes and 62/830 instructions
 * different: advance-arrow scratch registers/order, record redraw operand
 * order and the ACTOR_RESOLVE store schedule. Statement/declaration order,
 * direct display reads, five tile-address forms and one-pass blocks did not
 * improve it. Its unread reserved[12] frame padding is removed.
 * The old redraw temporary kept 0xff outside the loop and prevented OR
 * elimination; that changes instruction presence, so the device is removed.
 * Consolidated the overlapping 080bd850.c attempt here: it used private
 * playback/display/record views, phase-local arrays with call_workspace[12],
 * pointer-cursor loops and a direct fixed-entry tile clear. It had no recorded
 * complete-bank measurement; its duplicate body is removed.
 * T0: canonical animation/cache owners, direct frame reset, no dummy storage.
 * Complete EN output is 1940 bytes: clearer 72, playback 1868, versus
 * native 72 + 1876. Local frames are 4/32 bytes versus native 4/44;
 * saved-register space remains 8/32 bytes. The unused helper argument stays.
 * The full 72-byte clearer and 15-entry jump table match. All three main
 * literal groups retain their values; the final group moves eight bytes.
 * All 88 relocations resolve for comparison: 59 external calls preserve
 * target/order and the nested call reaches the clearer at the bank start.
 * Full-bank comparison has 401 differing bytes including the absent
 * eight-byte tail; the first difference is the frame reserve at bank +0x5a.
 * Main score 2292: 61 register, 2 stack, 6 operand, 14 reordered,
 * 3 inserted and 7 deleted. One operand is only an unresolved scorer
 * nested compiler name; full-bank relocation proves that target correct.
 * Remaining changes include arrow setup/store ordering, register allocation,
 * the smaller frame/static-chain offset and removal of the redraw read/OR.
 * T0 already uses signed interpretation at the unsigned Summon sentinel.
 * T0a: completed the remaining local producer declarations; the complete
 * emitted ELF is identical to T0. No width/lifetime defect justified a body
 * follow-up. No edition credit.
 */
#include "CALLBACK_SCHEDULER.H"
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "VRAM_BLOCK.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "ANIMSPR.H"
#include "MOTION_OBJECT.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE.H"
#include "UI.H"
#include "MENU_LIST.H"

extern volatile u32 gFrameCount;
extern volatile s32 gFrameTick;
extern volatile s32 gKeysPressedLatch;
extern u8 BattlePres_AdvanceArrowTiles[];

s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 target);
void AudioCommand_PlayFar(s32 cue);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *state, u32 value);
void UiText_DrawQuantity(u32 value, u32 slot);
void UiText_PrepareMessageWorkFar(s32 message);
void UiWork_ClearValueNameTablesFar(void);
void Object_SetMode(void *object, s32 mode);
s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 flags);
s32 BattleActor_ResetRuntimeFields(s32 unit_id);
s32 Object_InitializeMode(struct AnimationObject *record, s32 mode);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
s32 BattleMotion_GetSlotField14(s32 unit_id);
s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value);
s32 BattlePres_SetActorModeAndAction(s32 unit_id);
void QueueIoWriteDelay6(u32 address, u32 value);
s32 BattleLayout_HighlightPartyPanelsFar(u16 *selection);
u32 Summon_GetEntryByte3Kind(s32 class_id);
void BattleActor_RemoveFromLists(s32 unit_id);
void render_animated_tile_frameFar(u8 *record, u32 frame);
s32 __modsi3(s32, s32);

/* Plays the queued battle events one frame at a time: the callback the event
 * runtime schedules while its phase is not 0 or 4. */
void BattleEvent_Playback(void)
{
    u16 selection[2];
    struct AnimationObject *records[4];
    struct BattleSession *work = gBattleWork;
    struct BattleEventState *state = &work->events;

    /* Blanks the tiles a motion record's sprite occupies. */
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
                Battle_ResolveTargetAction(&work->plan, state->queue.target_index);
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
                    case BATTLE_EVENT_SOUND:
                        AudioCommand_PlayFar(state->queue.operands[i]);
                        break;
                    case BATTLE_EVENT_SCRIPT_UPDATE:
                        Battle_SetRuntimeFlagBit0(state, state->queue.operands[i]);
                        break;
                    case BATTLE_EVENT_UNIT:
                        UiText_DrawQuantity(state->queue.operands[i], 1);
                        break;
                    case BATTLE_EVENT_VALUE:
                        UiText_DrawQuantity(state->queue.operands[i], 5);
                        break;
                    case BATTLE_EVENT_ITEM:
                        UiText_DrawQuantity(state->queue.operands[i] & 0x1ff, 2);
                        break;
                    case BATTLE_EVENT_ACTION:
                        UiText_DrawQuantity(state->queue.operands[i] & 0x3fff, 4);
                        break;
                    case BATTLE_EVENT_MARK:
                        gBattleDisplayWork->marked = 1;
                        break;
                    case BATTLE_EVENT_TEXT:
                        if ((s32)state->queue.operands[i] >= 0)
                            UiText_PrepareMessageWorkFar(state->queue.operands[i]);
                        state->phase = 3;
                        gKeysPressedLatch = 0;
                        break;
                    case BATTLE_EVENT_TEXT_CONTINUE:
                        if ((s32)state->queue.operands[i] >= 0)
                            UiText_PrepareMessageWorkFar(state->queue.operands[i]);
                        state->phase = 13;
                        break;
                    case BATTLE_EVENT_RESET:
                        UiWork_ClearValueNameTablesFar();
                        break;
                    case BATTLE_EVENT_ACTOR_EFFECT:
                        BattleActor_DestroyTemporaryObject(state->queue.operands[i]);
                        break;
                    case BATTLE_EVENT_ACTOR_BEGIN:
                        if (state->pending_cue > 0)
                            AudioCommand_PlayFar(state->pending_cue);
                        state->actor_id = state->queue.operands[i];
                        Object_SetMode(GetBattleObjectSlot(state->actor_id)->object, 5);
                        state->phase = 10;
                        state->timer = 0;
                        break;
                    case BATTLE_EVENT_ACTOR_RESOLVE:
                        {
                            struct BattleUnit *unit;
                            struct AnimationObject *record;
                            s32 n;

                            state->actor_id = state->queue.operands[i];
                            BattleEnemy_RecordDefeat(state->actor_id, state->flags);
                            BattleActor_ResetRuntimeFields(state->actor_id);
                            unit = Owner_GetStateFar(state->actor_id);
                            for (n = 0; (record = GetMotionRecord(GetBattleObjectSlot(state->actor_id)->object, n)) != 0; n++) {
                                if (unit->status_12a != 1)
                                    Object_InitializeMode(record, 4);
                                else
                                    Object_InitializeMode(record, 5);
                            }
                            if (unit->status_12a == 1) {
                                state->phase = 11;
                                state->timer = 0;
                            }
                        }
                        break;
                    case BATTLE_EVENT_REFRESH:
                        UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode);
                        break;
                    case BATTLE_EVENT_ACTOR_FINISH:
                        {
                            struct BattleObjectSlot *slot;

                            BattleUnit_BuildStatusFlags(state->queue.operands[i], GetBattleObjectSlot(state->queue.operands[i]));
                            slot = GetBattleObjectSlot(state->queue.operands[i]);
                            BattleMotion_SetRecordChildValues(slot->object, BattleMotion_GetSlotField14(state->queue.operands[i]));
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
            struct BattleDisplayWork *display;
            struct BattlePromptSprite *entry;
            struct UiWindow *context;
            struct BattleDisplayOffset *viewport;
            s32 tiles;
            s32 x;

            tiles = (s32)BattlePres_AdvanceArrowTiles + ((gFrameCount >> 2) & 7) * 128;
            display = gBattleDisplayWork;
            entry = &state->display_entry;
            context = display->window;
            viewport = display->offset;
            x = 0;
            if (state->display_source == -1)
                state->display_source = work->prompt_slot;
            UiWork_ClearValueNameTablesFar();
            QueueIoWriteDelay10(0x0400004a, 4);
            QueueIoWriteDelay6(0x0400004a, 16);
            entry->attributes.word = 0xa000;
            entry->tile.word = x;
            entry->tile.bits.tile = Resource_GetBuffer(state->display_source, tiles);
            x = context->x * 8 + (viewport->x >> 8) + 4;
            entry->attributes.bits.x = x;
            entry->attributes.bits.y = Trig_Sin(gFrameCount << 12) / 0x8000 + context->y * 8 + (viewport->y >> 8) + 6;
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
                    struct BattleObjectSlot *slot;

                    selection[0] = 0xff;
                    slot = GetBattleObjectSlot(state->actor_id);
                    BattleMotion_SetRecordChildValues(slot->object, BattleMotion_GetSlotField14(state->actor_id));
                } else {
                    selection[0] = state->actor_id;
                    selection[1] = 0xff;
                    BattleMotion_SetRecordChildValues(GetBattleObjectSlot(state->actor_id)->object, 7);
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
                    s32 kind = (s32)Summon_GetEntryByte3Kind(Owner_GetStateFar(state->actor_id)->class_id);

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
                    s32 kind = (s32)Summon_GetEntryByte3Kind(Owner_GetStateFar(state->actor_id)->class_id);

                    if (kind >= 0)
                        AudioCommand_PlayFar(kind + 146);
                }
                if (state->timer >= 0x400)
                    frame = (state->timer - 0x400) / 8 % 5 + 1;
                if (frame == 6 || (state->timer & 7) == 0) {
                    struct AnimationObject *record;
                    s32 n;

                    for (n = 0; (record = GetMotionRecord(GetBattleObjectSlot(state->actor_id)->object, n)) != 0; n++) {
                        records[n] = record;
                        record->entries[0]->param = frame;
                        record->entries[0]->frame = 0xff;
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
                struct BattleObjectSlot *slot;
                struct AnimationObject *record;
                s32 count;
                s32 frame;
                s32 base;

                slot = GetBattleObjectSlot(state->actor_id);
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
