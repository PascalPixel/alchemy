#include "DMA.H"
extern u8 Data_000002c4[];
void *Func_08004938(s32 size);
void Func_08002df0(void *allocation);
typedef u32 (*LoadedRoutine)(const void *source, void *destination);
u32 Func_08005340(const void *source, void *destination)
{
    u32 size = (u32)Data_000002c4;
    LoadedRoutine routine = (LoadedRoutine)Func_08004938(size);
    u32 result;
    Dma_Set((const void *)0x08002544, (void *)routine, (size >> 2) | 0x84000000, (volatile u32 *)0x040000d4);
    result = routine(source, destination);
    Func_08002df0((void *)routine);
    return result;
}
