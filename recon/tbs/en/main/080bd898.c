/* DRAFT: BattleEvent_Playback with its nested tile clearer (listings 080bd898
 * and 080bd850). Same size as the ROM, 62 of 830 instructions differ:
 * 1. the advance-arrow preamble picks other scratch registers (the address of
 *    gFrameCount lives in r4 across the block here, in the ROM it is reloaded),
 *    which also reorders the entry address and the zeroing of x;
 * 2. `redraw` forces the 0xff out of the record loop as the ROM has it; written
 *    as a constant the loop pass leaves it in (22 instructions, lifetime 1) and
 *    combine folds the OR away. The OR then takes its operands the other way;
 * 3. the actor store in the ACTOR_RESOLVE case is scheduled one instruction early.
 * The reload registers go round-robin through the whole function, so 1 may only
 * be the trace of one reload more or fewer somewhere before it; 3 is the one
 * earlier place where the code differs. Tried without effect: statement and
 * declaration order, the display work read directly, five spellings of the tile
 * address, run-once blocks around each statement group.
 * alchemy drafts cannot parse a nested function; compare by compiling and diffing. */
#include "TYPES.H"
#include "BATTLE_STATUS_ICON.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "VRAM_BLOCK.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "MOTION_OBJECT.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE.H"
#include "UI.H"
#include "MENU_LIST.H"

struct BattleMotionPart {
    u8 unknown_00[5];
    s8 frame;
    u8 unknown_06[0x10];
    u8 flags;
};

struct BattleMotionRecord {
    u8 unknown_00[0x1c];
    u8 block;
    u8 unknown_1d[3];
    u8 width;
    u8 height;
    u8 unknown_22[6];
    struct BattleMotionPart *part;
};

extern volatile u32 gFrameCount;
extern volatile s32 gFrameTick;
extern volatile s32 gKeysPressedLatch;
extern u8 BattlePres_AdvanceArrowTiles[];

void Battle_ResolveTargetAction(struct BattlePlan *plan, s32 target);
void AudioCommand_PlayFar(s32 cue);
void Battle_SetRuntimeFlagBit0(struct BattleEventState *state, s32 value);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_PrepareMessageWorkFar(s32 message);
void UiWork_ClearValueNameTablesFar(void);
void BattleActor_DestroyTemporaryObject(s32 unit_id);
void Object_SetMode(struct MotionObject *object, s32 mode);
void BattleEnemy_RecordDefeat(s32 unit_id, s32 flags);
void BattleActor_ResetRuntimeFields(s32 unit_id);
void Object_InitializeMode(void *record, s32 mode);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
s32 BattleMotion_GetSlotField14(s32 unit_id);
void BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value);
void BattlePres_SetActorModeAndAction(s32 unit_id);
void QueueIoWriteDelay6(u32 address, u32 value);
void BattleLayout_HighlightPartyPanelsFar(u16 *selection);
s32 Summon_GetEntryByte3Kind(s32 class_id);
void BattleActor_RemoveFromLists(s32 unit_id);
void render_animated_tile_frameFar(void *record, s32 frame);
s32 __modsi3(s32, s32);

/* Plays the queued battle events one frame at a time: the callback the event
 * runtime schedules while its phase is not 0 or 4. */
void BattleEvent_Playback(void)
{
    u8 reserved[12]; /* the frame keeps twelve bytes nothing reads */
    u16 selection[2];
    void *records[4];
    struct BattleSession *work = gBattleWork;
    struct BattleEventState *state = &work->events;

    /* Blanks the tiles a motion record's sprite occupies. */
    void BattleEvent_ClearRecordTiles(struct BattleMotionRecord *record, s32 value)
    {
        Iwram_ClearWords((void *)(0x06010000 + gVramBlockCache[record->block].offset),
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
                            void *record;
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
                    s32 kind = Summon_GetEntryByte3Kind(Owner_GetStateFar(state->actor_id)->class_id);

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
                    s32 kind = Summon_GetEntryByte3Kind(Owner_GetStateFar(state->actor_id)->class_id);

                    if (kind >= 0)
                        AudioCommand_PlayFar(kind + 146);
                }
                if (state->timer >= 0x400)
                    frame = (state->timer - 0x400) / 8 % 5 + 1;
                if (frame == 6 || (state->timer & 7) == 0) {
                    struct BattleMotionRecord *record;
                    /* FAKEMATCH: the variable takes the 0xff out of the loop, where
                     * the ROM has it; as a constant it stays in and the OR folds away. */
                    s32 redraw = 0xff;
                    s32 n;

                    for (n = 0; (record = GetMotionRecord(GetBattleObjectSlot(state->actor_id)->object, n)) != 0; n++) {
                        records[n] = record;
                        record->part->frame = frame;
                        record->part->flags |= redraw;
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
                void *record;
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
                    render_animated_tile_frameFar(records[k], frame);
                    render_animated_tile_frameFar(records[k], base - 19);
                    render_animated_tile_frameFar(records[k], base - 18);
                    render_animated_tile_frameFar(records[k], base - 17);
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
