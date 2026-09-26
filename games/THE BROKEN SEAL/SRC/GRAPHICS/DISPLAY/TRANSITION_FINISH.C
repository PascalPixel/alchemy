#include "TYPES.H"

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
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void WaitFrames(s32 frames);
void Runtime_SetIrqHandler(s32, s32, void (*)(void));
void Blend_SetDarkenTarget0(s32 duration);
void Blend_SetDarkenTarget16(s32 duration);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Func_0808f52c(void);
void BattleFx_StartWindowHBlankDma(void);
void DisplayTransition_UpdateFrame(void);
void DisplayTransition_Update(void);
void DisplayTransition_UpdateFromCentre(void);
void DisplayTransition_UpdateScanline(void);

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
        Scheduler_AddOrUpdateCallback(Func_0808f52c, 0xc80);
        Scheduler_AddOrUpdateCallback(BattleFx_StartWindowHBlankDma, 0x480);
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
        Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
        state->start = 32;
        state->end = 64;
        state->frames = frames;
        state->phase = 0;
        break;
    }
    case 4: {
        struct DisplayTransitionState *state = DisplayTransition_AllocateAndClearState();
        if (value == 0) {
            Scheduler_AddOrUpdateCallback(DisplayTransition_Update, 0xc80);
            Runtime_SetIrqHandler(1, 0, DisplayTransition_UpdateScanline);
            state->start = 0;
            state->end = 80;
            state->frames = frames;
            state->phase = 0;
        } else {
            Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFromCentre, 0xc80);
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
