#include "TYPES.H"
#include "DMA.H"

extern void *Data_03001e8c;
extern u8 Value_00000214;

void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);

typedef void (*RamRoutine)(void *dst, void *scene);

/* With no scene loaded, fills the 160-entry buffer with 0xe0e0; otherwise
   copies the 0x214-byte routine at 0x080158e8 into RAM and runs it over the
   buffer and the scene record. */
void UiWindow_FillFromScene(void *dst)
{
    void *scene = Data_03001e8c;

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, (volatile u32 *)0x040000d4);
    } else {
        u32 size = (u32)&Value_00000214;
        RamRoutine code = (RamRoutine)Runtime_BumpAllocate(size);

        Dma_Set((void *)0x080158e8, code, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        code(dst, scene);
        Runtime_BumpFree(code);
    }
}
