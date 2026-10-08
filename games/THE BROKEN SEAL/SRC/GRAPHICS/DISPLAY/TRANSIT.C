#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
#include "DISPTRAN.H"
#include "IRQ.H"

void WaitFrames(s32 frames);

extern volatile u32 gFrameCount;

#define QUEUE_DISPLAY_CONTROL(value)                                        \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = q->entries[count];                            \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (u32)&REG_DISPCNT;                             \
            *destination = 0x20000;                                         \
        }                                                                   \
        do { *ime = saved; } while (0);                                     \
    } while (0)

void DisplayTransition_Start(s32 mode, s32 frames)
{
    void **work;
    struct DisplayTransitionWindow *display;
    s32 value;
    s32 kind;

    /* FAKEMATCH: needed to match: one-pass IME scopes and count stores preserve saved-IME and queue load order; the map-work word prefix preserves address order. */
    kind = (mode >> 8) & 0xff;
    work = Ram_MapWork;
    display = *(struct DisplayTransitionWindow **)work;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget16(0);
        Blend_SetDarkenTarget0(frames);
        WaitFrames(1);
        break;
    case 1:
        BattleFx_ApplyColorToSourceBuffer(0x8000, PLTT_BACKDROP);
        BattleFx_StartBufferInterpolation(frames);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(REG_DISPCNT | display->dispcnt);
        BattleFx_SetPrimaryBufferValue(0);
        return;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 0;
        state->mask = 63;
        state->active = 1;
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(REG_DISPCNT | display->dispcnt);
        state->start = 0;
        state->end = 32;
        state->duration = frames;
        state->step = 0;
        return;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        DisplayTransition_FillTilemapAndSolidTile(15);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        QUEUE_DISPLAY_CONTROL(REG_DISPCNT | display->dispcnt);
        state->start = 0;
        state->end = 32;
        state->duration = frames;
        state->step = 0;
        return;
    }
    case 4: {
        struct DisplayTransitionState *state;

        display = (struct DisplayTransitionWindow *)*work;
        state = DisplayTransition_AllocateAndClearState();
        display->first_line = 80;
        display->second_line = 80;
        WaitFrames(1);
        if (value == 0) {
            Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->duration = frames;
            state->step = 0;
        } else {
            Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->duration = frames;
            state->step = 0;
        }
        break;
    }
    }
    QUEUE_DISPLAY_CONTROL(REG_DISPCNT | display->dispcnt);
}

void DisplayTransition_Finish(s32 mode, s32 frames)
{
    s32 value;
    s32 kind;

    kind = (mode >> 8) & 0xff;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget0(0);
        Blend_SetDarkenTarget16(frames);
        break;
    case 1:
        BattleFx_ApplyColorToTargetBuffer(0x8000, 0);
        BattleFx_StartBufferInterpolation(frames);
        break;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        state->mask = 63;
        state->active = 1;
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateScanlineTable, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
        WaitFrames(1);
        state->start = 32;
        state->end = 64;
        state->duration = frames;
        state->step = 0;
        break;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->mode = value;
        state->value = 32;
        DisplayTransition_FillTilemapAndSolidTile(0);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        state->start = 32;
        state->end = 64;
        state->duration = frames;
        state->step = 0;
        break;
    }
    case 4: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        if (value == 0) {
            Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->duration = frames;
            state->step = 0;
        } else {
            Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->duration = frames;
            state->step = 0;
        }
        break;
    }
    }
}

void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
    struct DisplayTransitionWindow *state;

    state = gMapWork[0];
    if (state != NULL) {
        if (clear_0200 != 0) {
            state->dispcnt &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            state->dispcnt &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            state->dispcnt &= 0xF7FF;
        }
    }
}

void DisplayTransition_Update(void)
{
    struct DisplayTransitionFrame *state =
        *Ram_DisplayWork;
    struct DisplayTransitionWindow *display =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_Update);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->end - state->start;
            s32 value;

            (*step)++;
            value = Iwram_SignedDivide(delta * *step, *duration);
            state->value = state->start + value;
        }
    }

    display_value = state->value;
    if (display_value > 79) {
        display->first_line = 200;
        display->second_line = 250;
    } else {
        display->first_line = display_value;
        display->second_line = 159 - display_value;
    }
}

void DisplayTransition_UpdateFromCentre(void)
{
    struct DisplayTransitionFrame *state =
        *Ram_DisplayWork;
    struct DisplayTransitionWindow *window =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback(DisplayTransition_UpdateFromCentre);
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->end - state->start;
            s32 offset;

            (*step)++;
            offset = Iwram_SignedDivide(delta * *step, *duration);
            state->value = state->start + offset;
        }
    }

    value = state->value;
    if (value > 79) {
        window->first_line = 200;
        window->second_line = 250;
    } else if (value != 0 && (gFrameCount & 1)) {
        window->first_line = value + 80;
        window->second_line = 80 - value;
    } else {
        window->first_line = 0;
        window->second_line = 159;
    }
}
