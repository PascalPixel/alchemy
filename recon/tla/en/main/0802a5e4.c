#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
extern u8 gMapCellBuffer[];

extern const u8 Func_0800a37c[];

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

void *Runtime_BumpAllocateAlternatePool(s32 size);
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);

extern u8 Tile_ConvertMapCodeSize[];

void Tilemap_ConvertBuffer(void)
{
    u8 *saved;
    ConvertFn routine;
    u32 size;

    size = 0x8000;
    /* FAKEMATCH: the two do-while blocks keep each allocation next to the
       transfer that uses it */
    do {
        saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
        CopyWords(saved, Ram_MapCellBuffer, size);
    } while (0);
    {
        u32 code_size = (u32)Tile_ConvertMapCodeSize;

        do {
            routine = (ConvertFn)Runtime_BumpAllocate(code_size);
            Dma_Set((void *)Func_0800a37c, routine, 0x84000000 | (code_size >> 2),
                    (volatile u32 *)0x040000d4);
        } while (0);
    }
    routine((gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
    Runtime_BumpFree(routine);
    Runtime_BumpFree(saved);
}
