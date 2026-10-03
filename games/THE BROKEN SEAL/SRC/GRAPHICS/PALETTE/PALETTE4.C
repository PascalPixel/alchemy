#include "PALBUF.H"
#include "DMA.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

void *Runtime_AllocateBlock(s32, u32);
void Unnamed_080f3078(u32, void *, void *, s32);

s32 Runtime_ReleaseHeapBlock(s32);


extern struct TitlePaletteWork *Data_03001ed0;


void TitlePalette_InitializeBuffers(void)
{
    volatile u32 zero;
    struct TitlePaletteWork *buffer;
    s32 operation;

    buffer = Runtime_AllocateBlock(32, sizeof(*buffer));
    zero = 0;
    Dma_Set(&zero, buffer, 0x85000c01, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000000, buffer, 0x84000080, (volatile u32 *)0x040000d4);
    Dma_Set((void *)0x05000200, &buffer->palette[256], 0x84000080, (volatile u32 *)0x040000d4);
    Unnamed_080f3078(0x10000, buffer, buffer->target, 0);
    operation = 3200;
    Scheduler_AddOrUpdateCallback((s32)(TitlePalette_UpdateFade), operation);
}

void Runtime_ScheduleCallbackAndReleaseBlock32A(void)
{
    Scheduler_RemoveCallback((u32)((s32)&TitlePalette_UpdateFade));
    Runtime_ReleaseHeapBlock(0x20);
}

void Graphics_TransformLargePalette(s32 index, s32 transform)
{
    struct TitlePaletteWork *target = Data_03001ed0;

    if (target != NULL)
        Unnamed_080f3078(index, target, target->target, transform);
}

void Graphics_TransformSmallPalette(s32 index, s32 transform)
{
    struct TitlePaletteWork *target = Data_03001ed0;

    if (target != NULL)
        Unnamed_080f3078(index, target, target->current, transform);
}

void Graphics_SetPaletteTransformValue(s32 value)
{
    struct TitlePaletteWork *target = Data_03001ed0;

    if (target != NULL)
        target->palette[0] = value;
}

void Graphics_UpdatePaletteInterpolation(s32 value)
{
    struct TitlePaletteWork *state = Data_03001ed0;

    if (state != NULL) {
        state->duration = value;
        state->step = 0;
        Graphics_InterpolatePaletteBuffers((s16 *)state->current, (s16 *)state->target, (s16 *)state->delta, value);
    }
}
