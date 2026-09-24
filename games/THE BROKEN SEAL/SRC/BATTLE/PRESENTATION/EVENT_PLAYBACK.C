#include "VRAM_BLOCK.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "MOTION_OBJECT.H"
#include "FIXED_MATH.H"

/*
 * Battle-event playback.  The resolver immediately before this owner fills
 * the parallel opcode/operand arrays; this routine consumes them and advances
 * the actor presentation state one frame at a time.
 */

struct BattlePlaybackState {
    struct BattleEventQueue events;       /* 0x000 */
    s32 phase;                            /* 0x148 */
    s32 event_index;                      /* 0x14c */
    s32 timer;                            /* 0x150 */
    u8 display_entry[12];                 /* 0x154 */
    s32 display_source;                   /* 0x160 */
    s32 actor_id;                         /* 0x164 */
    s32 animation_timer;                  /* 0x168 */
    s32 actor_mode;                       /* 0x16c */
};

struct BattlePlaybackRuntime {
    u8 unknown_000[0x654];
    struct BattlePlan plan;               /* 0x654 */
    struct BattlePlaybackState playback;  /* 0x6b8 */
};

struct BattleDisplayContext {
    u16 unknown_00[6];
    u16 tile_x;                           /* 0x0c */
    u16 tile_y;                           /* 0x0e */
};

struct BattleDisplayViewport {
    u16 unknown_00[2];
    u16 scroll_x;                         /* 0x04 */
    u16 scroll_y;                         /* 0x06 */
};

struct BattleDisplayRuntime {
    struct BattleDisplayContext *context;
    struct BattleDisplayViewport *viewport;
};

struct BattleMotionPart {
    u8 unknown_00[5];
    s8 frame;                             /* 0x05 */
    u8 unknown_06[0x10];
    u8 flags;                             /* 0x16 */
};

struct BattleMotionRecord {
    u8 unknown_00[0x28];
    struct BattleMotionPart *part;        /* 0x28 */
};

struct BattlePlaybackPhaseLocals {
    void *records[4];
    u16 selection[2];
    u8 call_workspace[12];
};

typedef char BattlePlaybackState_Phase[
    (u32)&((struct BattlePlaybackState *)0)->phase == 0x148 ? 1 : -1];
typedef char BattlePlaybackState_Actor[
    (u32)&((struct BattlePlaybackState *)0)->actor_id == 0x164 ? 1 : -1];
typedef char BattlePlaybackRuntime_State[
    (u32)&((struct BattlePlaybackRuntime *)0)->playback == 0x6b8 ? 1 : -1];

void QueueIoWriteDelay10(s32 reg, s32 value);
void QueueIoWriteDelay6(s32 reg, s32 value);
void Runtime_PushSlotEntry(void *entry, s32 slot);
s32 Resource_GetBuffer(s32 index, s32 table);
void AnimationObjects_SelectAnimationFar(void *record, s32 animation);
void Object_SetMode(void *object, s32 animation);
void render_animated_tile_frameFar(void *record, s32 frame);
s32 UiWork_IsCompleteFar(void);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_PushValueSlotFar(s32 value, s32 style);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void UiText_PrepareMessageWorkFar(void);
void BattleLayout_HighlightPartyPanelsFar(u16 *selection);
void BattleUnit_BuildStatusFlags(s32 actor_id, void *slot);
s32 BattleMotion_GetSlotField14(s32 actor_id);
void BattlePres_SetActorModeAndAction(s32 actor_id);
s32 ActivateBattleObjectSlot(s32 actor_id);
void Unnamed_080ba918(void *object, s32 value);
void BattleActor_RemoveFromLists(s32 actor_id);
void BattleActor_ResetRuntimeFields(s32 actor_id);
void BattleActor_DestroyTemporaryObject(s32 actor_id);
void Battle_SetRuntimeFlagBit0(struct BattlePlaybackState *state, s32 value);
void BattleEnemy_RecordDefeat(s32 actor_id, s32 mode);
s32 Summon_GetEntryByte3Kind(s32 class_id);
void Audio_PlayCue(s32 sound_id);

#define REG_DISPCNT_SUB (*(volatile u32 *)0x03001e40)
#define FRAME_COUNTER   (*(volatile s32 *)0x03001800)
#define BATTLE_INPUT    (*(volatile s32 *)0x03001ae8)
#define BATTLE_INPUT_2  (*(volatile s32 *)0x03001af8)

void BattleEvent_Playback(void)
{
    struct BattleDisplayViewport *viewport;
    struct BattlePlaybackRuntime *runtime;
    struct BattlePlaybackPhaseLocals phase_locals;
    struct BattlePlaybackState *state;
    s32 display_x;

    void BattleEvent_ClearRecordTiles(void *record, s32 value)
    {
        u8 *part = record;
        void *destination = (void *)(0x06010000 + gVramBlockCache[part[28]].offset);
        ((void (*)(void *, u32))0x03000164)(destination, part[32] * part[33]);
    }

    runtime = *(struct BattlePlaybackRuntime **)0x03001e74;
    state = &runtime->playback;

    if (state->phase == 0)
        return;

    for (;;) {
        s32 phase;

        phase = state->phase;
        if (phase == 4)
            return;

        if (phase == 1) {
            if (state->events.target_index < runtime->plan.target_count) {
                state->events.count = 0;
                state->event_index = 0;
                state->timer = 0;
                Battle_ResolveTargetAction(
                    &runtime->plan,
                    state->events.target_index);
                state->events.target_index++;
                state->phase = 2;
            } else {
                state->phase = 4;
            }
            continue;
        }

        if (phase == 2) {
            s32 event_index;

            event_index = state->event_index;
            if (event_index < state->events.count) {
                do {
                    if (state->timer != 0) {
                        state->timer--;
                        return;
                    }

                    switch (state->events.opcodes[event_index]) {
                    case BATTLE_EVENT_SOUND:
                        Audio_PlayCue(state->events.operands[event_index]);
                        break;
                    case BATTLE_EVENT_SCRIPT_UPDATE:
                        Battle_SetRuntimeFlagBit0(
                            state,
                            state->events.operands[event_index]);
                        break;
                    case BATTLE_EVENT_UNIT:
                        UiWork_PushValueSlotFar(
                            state->events.operands[event_index],
                            1);
                        break;
                    case BATTLE_EVENT_VALUE:
                        UiWork_PushValueSlotFar(
                            state->events.operands[event_index],
                            5);
                        break;
                    case BATTLE_EVENT_ITEM:
                        UiWork_PushValueSlotFar(
                            state->events.operands[event_index] & 0x1ff,
                            2);
                        break;
                    case BATTLE_EVENT_ACTION:
                        UiWork_PushValueSlotFar(
                            state->events.operands[event_index] & 0x3fff,
                            4);
                        break;
                    case BATTLE_EVENT_MARK:
                        (*(s32 **)0x03001ee4)[2] = 1;
                        break;
                    case BATTLE_EVENT_TEXT:
                        if ((s32)state->events.operands[event_index] >= 0)
                            UiText_PrepareMessageWorkFar();
                        state->phase = 3;
                        BATTLE_INPUT_2 = 0;
                        break;
                    case BATTLE_EVENT_TEXT_CONTINUE:
                        if ((s32)state->events.operands[event_index] >= 0)
                            UiText_PrepareMessageWorkFar();
                        state->phase = 13;
                        break;
                    case BATTLE_EVENT_RESET:
                        UiWork_ClearValueNameTablesFar();
                        break;
                    case BATTLE_EVENT_ACTOR_EFFECT:
                        BattleActor_DestroyTemporaryObject(state->events.operands[event_index]);
                        break;
                    case BATTLE_EVENT_ACTOR_BEGIN:
                    {
                        struct BattleObjectSlot *slot;

                        if (state->animation_timer > 0)
                            Audio_PlayCue(state->animation_timer);
                        state->actor_id = state->events.operands[event_index];
                        slot = GetBattleObjectSlot(
                            state->events.operands[event_index]);
                        Object_SetMode(slot->object, 5);
                        state->phase = 10;
                        state->timer = 0;
                        break;
                    }
                    case BATTLE_EVENT_ACTOR_RESOLVE:
                    {
                        struct BattleUnit *unit;
                        s32 record_index;
                        void *record;

                        state->actor_id = state->events.operands[event_index];
                        BattleEnemy_RecordDefeat(
                            state->events.operands[event_index],
                            state->actor_mode);
                        BattleActor_ResetRuntimeFields(state->actor_id);
                        unit = BattleUnit_Get(state->actor_id);
                        record_index = 0;
                        while ((record = GetMotionRecord(
                                    GetBattleObjectSlot(state->actor_id)->object,
                                    record_index)) != NULL) {
                            if (unit->status_12a != 1)
                                AnimationObjects_SelectAnimationFar(record, 4);
                            else
                                AnimationObjects_SelectAnimationFar(record, 5);
                            record_index++;
                        }
                        if (unit->status_12a == 1) {
                            state->phase = 11;
                            state->timer = 0;
                        }
                        break;
                    }
                    case BATTLE_EVENT_REFRESH:
                        UiWindow_DrawPartyStatusContentsFar(
                            ((u8 *)*(void **)0x03001e74)[65]);
                        break;
                    case BATTLE_EVENT_ACTOR_FINISH:
                    {
                        struct BattleObjectSlot *slot;

                        BattleUnit_BuildStatusFlags(
                            state->events.operands[event_index],
                            GetBattleObjectSlot(
                                state->events.operands[event_index]));
                        slot = GetBattleObjectSlot(
                            state->events.operands[event_index]);
                        Unnamed_080ba918(
                            slot->object,
                            BattleMotion_GetSlotField14(
                                state->events.operands[event_index]));
                        BattlePres_SetActorModeAndAction(
                            state->events.operands[event_index]);
                        break;
                    }
                    }

                    event_index = ++state->event_index;
                } while (event_index < state->events.count
                    && state->phase == 2);
            }

            if (state->phase == 2)
                state->phase = 1;
            continue;
        }

        if (phase == 3 || phase == 13) {
            if (UiWork_IsCompleteFar() == 0)
                return;
            if (state->phase == 13) {
                state->phase = 2;
                state->timer = 0;
            } else {
                state->phase = 5;
                state->display_source = -1;
                state->timer = FRAME_COUNTER;
            }
            continue;
        }

        if (phase == 5) {
            struct BattleDisplayRuntime *display_runtime;
            struct BattleDisplayContext *context;
            u8 *entry;
            s32 table;
            s32 wave;

            table = ((((u32)REG_DISPCNT_SUB >> 2) & 7) << 7)
                + 0x080c3734;
            display_runtime = *(struct BattleDisplayRuntime **)0x03001ee4;
            context = display_runtime->context;
            viewport = display_runtime->viewport;
            entry = state->display_entry;
            display_x = 0;
            if (state->display_source == -1)
                state->display_source = runtime->plan.outcome;

            UiWork_ClearValueNameTablesFar();
            QueueIoWriteDelay10(0x0400004a, 4);
            QueueIoWriteDelay6(0x0400004a, 0x10);
            *(u32 *)(entry + 4) = 0xa000;
            *(u32 *)(entry + 8) = display_x;
            *(u16 *)(entry + 8) =
                (*(u16 *)(entry + 8) & 0xfc00)
                | (Resource_GetBuffer(state->display_source, table) & 0x3ff);
            display_x = context->tile_x * 8
                + (viewport->scroll_x >> 8) + 4;
            *(u16 *)(entry + 6) =
                (*(u16 *)(entry + 6) & 0xfe00)
                | (display_x & 0x1ff);

            wave = Trig_Sin(REG_DISPCNT_SUB << 12);
            if (wave < 0)
                wave += 0x7fff;
            entry[4] = (viewport->scroll_y >> 8)
                + (wave >> 15) + context->tile_y * 8 + 6;

            if ((BATTLE_INPUT & 2)
                || (BATTLE_INPUT_2 & 0x303)
                || ((u32)(FRAME_COUNTER - state->timer) > 10
                    && (BATTLE_INPUT & 0x303))) {
                Audio_PlayCue(0x6f);
                state->phase = 2;
                state->timer = 0;
                continue;
            }

            Runtime_PushSlotEntry(entry, 0xf0);
            return;
        }

        if (phase == 10) {
            if (state->timer & 1) {
                if (state->timer & 2) {
                    struct BattleObjectSlot *slot;

                    phase_locals.selection[0] = 0xff;
                    slot = GetBattleObjectSlot(state->actor_id);
                    Unnamed_080ba918(
                        slot->object,
                        BattleMotion_GetSlotField14(state->actor_id));
                } else {
                    struct BattleObjectSlot *slot;

                    phase_locals.selection[0] = state->actor_id;
                    phase_locals.selection[1] = 0xff;
                    slot = GetBattleObjectSlot(state->actor_id);
                    Unnamed_080ba918(slot->object, 7);
                }
                BattleLayout_HighlightPartyPanelsFar(phase_locals.selection);
            }

            state->timer++;
            if (state->timer <= 8)
                return;
            state->phase = 2;
            state->timer = 0;
            continue;
        }

        if (phase == 11) {
            s32 timer;

            timer = state->timer;
            if (timer == 0 || timer >= 0x400) {
                s32 frame;

                frame = 6;
                if (timer == 0 && state->actor_mode != 0) {
                    s32 sound;

                    sound = Summon_GetEntryByte3Kind(
                        BattleUnit_Get(state->actor_id)->class_id);
                    if (sound >= 0) {
                        sound--;
                        if (sound < 0)
                            sound = 0;
                        Audio_PlayCue(sound + 0x92);
                    }
                    state->timer = 0x400;
                }

                if (state->timer > 0x41d)
                    state->timer = 0;
                if (state->timer == 0) {
                    s32 sound;

                    sound = Summon_GetEntryByte3Kind(
                        BattleUnit_Get(state->actor_id)->class_id);
                    if (sound >= 0)
                        Audio_PlayCue(sound + 0x92);
                }
                if (state->timer >= 0x400) {
                    s32 step;

                    step = state->timer - 0x400;
                    if (step < 0)
                        step = state->timer - 0x3f9;
                    frame = Math_Mod(step >> 3, 5) + 1;
                }

                if (frame == 6 || (state->timer & 7) == 0) {
                    s32 part_flags;
                    struct BattleMotionRecord *record;
                    void **record_cursor;
                    s32 record_index;

                    part_flags = 0xff;
                    record_index = 0;
                    record_cursor = phase_locals.records;
                    while ((record = GetMotionRecord(
                                GetBattleObjectSlot(state->actor_id)->object,
                                record_index)) != NULL) {
                        *record_cursor++ = record;
                        record->part->frame = frame;
                        record->part->flags |= part_flags;
                        record_index++;
                    }
                }
            } else if (timer == 4) {
                BattleActor_RemoveFromLists(state->actor_id);
            } else if (timer > 4) {
                struct BattleObjectSlot *slot;
                void **record_cursor;
                s32 count;
                s32 frame;
                s32 scaled_timer;
                void *record;

                slot = GetBattleObjectSlot(state->actor_id);
                *(s16 *)((u8 *)slot + 0x2a) = 1;
                count = 0;
                record_cursor = phase_locals.records;
                record = GetMotionRecord(slot->object, count);
                while (record != NULL) {
                    *record_cursor++ = record;
                    count++;
                    record = GetMotionRecord(slot->object, count);
                }

                scaled_timer = state->timer * 4;
                frame = scaled_timer - 20;
                if (frame > 127) {
                    s32 remaining;

                    if (count > 0) {
                        record_cursor = phase_locals.records;
                        remaining = count;
                        do {
                            BattleEvent_ClearRecordTiles(*record_cursor++, 0);
                            remaining--;
                        } while (remaining != 0);
                    }
                    ActivateBattleObjectSlot(state->actor_id);
                    state->phase = 2;
                    state->timer = 0;
                    return;
                }

                if (count > 0) {
                    s32 remaining;

                    record_cursor = phase_locals.records;
                    remaining = count;
                    do {
                        render_animated_tile_frameFar(*record_cursor, frame);
                        render_animated_tile_frameFar(*record_cursor, scaled_timer - 19);
                        render_animated_tile_frameFar(*record_cursor, scaled_timer - 18);
                        remaining--;
                        render_animated_tile_frameFar(*record_cursor++, scaled_timer - 17);
                    } while (remaining != 0);
                }
            }

            state->timer++;
            return;
        }
    }
}
