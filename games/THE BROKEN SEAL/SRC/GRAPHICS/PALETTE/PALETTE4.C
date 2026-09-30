#include "DMA.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

void *Runtime_AllocateBlock(s32, u32);
void Unnamed_080f3078(u32, void *, void *, s32);
void Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void TitlePalette_UpdateFade(void);

s32 Runtime_ReleaseHeapBlock(s32);

/* runtime/memory/schedule_callback_and_release_block_32_a.c */
s32 Scheduler_RemoveCallback(s32);

extern u8 Data_03001ed0[];
void Graphics_InterpolatePaletteBuffers(s16 *, s16 *, s16 *, s32);

struct PaletteInterpolationState {
    u8 unknown_0000[0x400];
    s16 first[0x600];
    s16 second[0x600];
    s16 output[0xa00];
    u8 unknown_3000;
    s8 value;
    s8 zero;
};

void TitlePalette_InitializeBuffers(void)
{
    volatile u32 zero;
    u8 *buffer;
    s32 operation;

    buffer = Runtime_AllocateBlock(32, 0x3004);
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000c01, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000080, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, buffer + 512, 0x84000080, (volatile u32 *)0x040000d4);
    Unnamed_080f3078(0x10000, buffer, buffer + 4096, 0);
    operation = 3200;
    Scheduler_AddOrUpdateCallback(TitlePalette_UpdateFade, operation);
}

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    Scheduler_RemoveCallback((s32)&TitlePalette_UpdateFade);
    Runtime_ReleaseHeapBlock(0x20);
}

void Graphics_TransformLargePalette(s32 index, s32 transform)
{
    void *target = *(void **)((u32)&Data_03001ed0);

    if (target != NULL)
        Unnamed_080f3078(index, target, (u8 *)target + 0x1000, transform);
}

void Graphics_TransformSmallPalette(s32 index, s32 transform)
{
    void *target = *(void **)((u32)&Data_03001ed0);

    if (target != NULL)
        Unnamed_080f3078(index, target, (u8 *)target + 0x400, transform);
}

void Graphics_SetPaletteTransformValue(s32 value)
{
    u16 *target = *(u16 **)((u32)&Data_03001ed0);

    if (target != NULL)
        *target = value;
}

void Graphics_UpdatePaletteInterpolation(s32 value)
{
    struct PaletteInterpolationState *state =
        *(struct PaletteInterpolationState **)((u32)&Data_03001ed0);

    if (state != NULL) {
        state->value = value;
        state->zero = 0;
        Graphics_InterpolatePaletteBuffers(state->first, state->second, state->output, value);
    }
}
