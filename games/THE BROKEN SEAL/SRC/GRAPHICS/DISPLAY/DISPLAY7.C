#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_REG.H"
#include "IO_WRITE_QUEUE.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
    u8 unknown_52c[8];
    s16 level;
    s16 step;
    u8 unknown_538[2];
    u8 start;
    u8 end;
    u8 frames;
    u8 phase;
};

void *DisplayTransition_AllocateAndClearState(void);
void DisplayTransition_FillTilemapAndSolidTile(s32 color);
void WaitFrames(s32 frames);
void Runtime_SetIrqHandler(s32, s32, void (*)(void));
void Blend_SetDarkenTarget0(s32 duration);
void Blend_SetDarkenTarget16(s32 duration);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_SetPrimaryBufferValue(u32 value);
void BattleFx_StartBufferInterpolation(s32 frames);
void DisplayTransition_UpdateScanlineTable(void);
void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);

extern u8 gCam[];

struct DisplayTransitionState2 {
    u8 pad_000[0x52a];
    u16 transition_value;
    u8 pad_52c[14];
    s8 transition_start;
    s8 transition_end;
    s8 transition_duration;
    s8 transition_step;
};

struct DisplayTransitionRegisters {
    u8 pad_000[0x100];
    u16 primary_value;
    u16 secondary_value;
};


struct DisplayTransitionState3 {
    u8 reserved_000[0x52a];
    u16 value;
    u8 reserved_52c[14];
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

struct DisplayTransitionWindow {
    u8 reserved_000[0x100];
    u16 first_line;
    u16 second_line;
};

extern volatile u32 gFrameCount;

/* The display work the transitions reach through the map work pointer: the
   display control value the frame's register write is made from, and the
   two lines the scanline split is drawn between. */
struct DisplayWork {
    u32 unknown_00[5];
    u16 dispcnt;
    u8 unknown_16[0xea];
    u16 split_top;
    u16 split_bottom;
};


/* QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C) written out: the display
   control write for the next frame, its value read only once a queue entry
   is free.
   FAKEMATCH: that function's odd constructs, the one-pass loops around the
   IME read and restore and the count stored through a u16 pointer, are what
   give the ROM's scheduling here, as in BATTLE/EFFECT/EFFECT45.C. */
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
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = 0x04000000;                                    \
            *destination = 0x20000;                                         \
        }                                                                   \
        do { *ime = saved; } while (0);                                     \
    } while (0)

/* Starts a screen transition; DisplayTransition_Finish ends it. The high
   byte of mode picks the effect (darken blend, palette fade, window wipe,
   tile fill, scanline split); the low byte is kept in the transition state.
   Every effect but the darken blend also queues the display control write. */
void DisplayTransition_Start(s32 mode, s32 frames)
{
    void **work;
    struct DisplayWork *display;
    s32 value;
    s32 kind;

    kind = (mode >> 8) & 0xff;
    work = Ram_MapWork;
    display = *(struct DisplayWork **)work;
    value = mode & 0xff;
    switch (kind) {
    case 0:
        Blend_SetDarkenTarget16(0);
        Blend_SetDarkenTarget0(frames);
        WaitFrames(1);
        break;
    case 1:
        BattleFx_ApplyColorToSourceBuffer(0x8000, *(u16 *)0x05000000);
        BattleFx_StartBufferInterpolation(frames);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
        BattleFx_SetPrimaryBufferValue(0);
        return;
    case 2: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->value = value;
        state->timer = 0;
        state->level = 63;
        state->step = 1;
        Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateScanlineTable), 0xc80);
        Scheduler_AddOrUpdateCallback((s32)(BattleFx_StartWindowHBlankDma), 0x480);
        WaitFrames(1);
        QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
        state->start = 0;
        state->end = 32;
        state->frames = frames;
        state->phase = 0;
        return;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->value = value;
        state->timer = 32;
        DisplayTransition_FillTilemapAndSolidTile(15);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0xc80);
        QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
        state->start = 0;
        state->end = 32;
        state->frames = frames;
        state->phase = 0;
        return;
    }
    case 4: {
        struct DisplayTransitionState *state;

        /* The map work pointer is read again here, as a plain pointer. */
        display = (struct DisplayWork *)*work;
        state = DisplayTransition_AllocateAndClearState();
        display->split_top = 80;
        display->split_bottom = 80;
        WaitFrames(1);
        if (value == 0) {
            Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_Update), 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->frames = frames;
            state->phase = 0;
        } else {
            Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFromCentre), 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 80;
            state->end = 0;
            state->frames = frames;
            state->phase = 0;
        }
        break;
    }
    }
    QUEUE_DISPLAY_CONTROL(*(volatile u16 *)0x04000000 | display->dispcnt);
}

/* Ends a screen transition. The high byte of mode picks the effect (darken
   blend, palette fade, window wipe, tile fill, scanline split); the low byte
   is kept in the transition state. */
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
        state->value = value;
        state->timer = 32;
        state->level = 63;
        state->step = 1;
        Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateScanlineTable), 0xc80);
        Scheduler_AddOrUpdateCallback((s32)(BattleFx_StartWindowHBlankDma), 0x480);
        WaitFrames(1);
        state->start = 32;
        state->end = 64;
        state->frames = frames;
        state->phase = 0;
        break;
    }
    case 3: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        state->value = value;
        state->timer = 32;
        DisplayTransition_FillTilemapAndSolidTile(0);
        WaitFrames(1);
        Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0xc80);
        state->start = 32;
        state->end = 64;
        state->frames = frames;
        state->phase = 0;
        break;
    }
    case 4: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        if (value == 0) {
            Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_Update), 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->frames = frames;
            state->phase = 0;
        } else {
            Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFromCentre), 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->frames = frames;
            state->phase = 0;
        }
        break;
    }
    }
}

/* display/state/clear_flags.c */
void DisplayState_ClearFlags(s32 clear_0800, s32 clear_0400, s32 clear_0200)
{
    void *state;

    state = *(void **)((u32)&gCam);
    if (state != NULL) {
        if (clear_0200 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFDFF;
        }
        if (clear_0400 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xFBFF;
        }
        if (clear_0800 != 0) {
            FIELD_AT_OFFSET(state, u16 *, 0x14) &= 0xF7FF;
        }
    }
}

/* display/transition/update.c */
/* Signed division runs from IWRAM through the register-call veneer. */
void DisplayTransition_Update(void)
{
    struct DisplayTransitionState2 *state =
        *(struct DisplayTransitionState2 **)Ram_DisplayWork;
    struct DisplayTransitionRegisters *display =
        *(struct DisplayTransitionRegisters **)Ram_MapWork;
    s8 *duration = &state->transition_duration;
    u32 display_value;

    if (*duration != 0) {
        s8 *step = &state->transition_step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback((u32)(DisplayTransition_Update));
            Runtime_SetIrqHandler(1, 0, 0);
            return;
        } else {
            s32 delta = state->transition_end - state->transition_start;
            s32 value;

            (*step)++;
            value = Iwram_SignedDivide(delta * *step, *duration);
            state->transition_value = state->transition_start + value;
        }
    }

    display_value = state->transition_value;
    if (display_value > 79) {
        display->primary_value = 200;
        display->secondary_value = 250;
    } else {
        display->primary_value = display_value;
        display->secondary_value = 159 - display_value;
    }
}

/* Steps the transition value from start to end over duration frames; once
 * done it removes itself as a callback and clears the HBlank handler. Then
 * sets the window's two scanlines: 200 and 250 from a value of 80 up; on odd
 * frames with a nonzero value, 80 + value and 80 - value about the centre
 * row; otherwise 0 and 159, the whole screen. */
void DisplayTransition_UpdateFromCentre(void)
{
    struct DisplayTransitionState3 *state =
        *(struct DisplayTransitionState3 **)Ram_DisplayWork;
    struct DisplayTransitionWindow *window =
        *(struct DisplayTransitionWindow **)Ram_MapWork;
    s8 *duration = &state->duration;
    u16 value;

    if (*duration != 0) {
        s8 *step = &state->step;

        if (*step >= *duration) {
            *duration = 0;
            Scheduler_RemoveCallback((u32)(DisplayTransition_UpdateFromCentre));
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
