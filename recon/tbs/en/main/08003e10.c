#include "video_dma_family.h"
#include "DMA.H"

#define Runtime_CopyAndCallRoutine Func_08003e10

u32 Func_08004938(s32 size);
void Func_08002df0(void *allocation);

typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size: 0xe0. */
extern u8 LoadedRuntime_Size[];

void Runtime_CopyAndCallRoutine(void *argument)
{
    u32 size = (u32)LoadedRuntime_Size;
    LoadedRoutine routine = (LoadedRoutine)Func_08004938(size);

    Dma_Set((const void *)0x08001dc8, (void *)routine,
            (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    routine(argument);
    Func_08002df0((void *)routine);
}
