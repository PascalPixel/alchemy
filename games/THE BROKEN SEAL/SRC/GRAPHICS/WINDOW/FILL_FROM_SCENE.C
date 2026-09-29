#include "TYPES.H"
#include "DMA.H"

extern const u8 Func_080158e8[];
extern u8 Tile_BuildMetatilesCodeSize[];

extern void *Data_03001e8c;

void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);

typedef void (*RamRoutine)(void *dst, void *scene);

/* With no scene loaded, fills the 160-entry buffer with 0xe0e0; otherwise
   copies the metatile builder into RAM and runs it over the buffer and the
   scene record. */
void UiWindow_FillFromScene(void *dst)
{
    void *scene = Data_03001e8c;

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, (volatile u32 *)0x040000d4);
    } else {
        u32 size = (u32)Tile_BuildMetatilesCodeSize;
        RamRoutine code = (RamRoutine)Runtime_BumpAllocate(size);

        Dma_Set((void *)Func_080158e8, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        code(dst, scene);
        Runtime_BumpFree(code);
    }
}
