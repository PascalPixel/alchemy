#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];

extern const u8 Func_0800a37c[];
typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);
extern u8 Tile_ConvertMapCodeSize[];

static __inline__ void CopyWords(void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: a direct call changes Tilemap_ConvertBuffer from mov r6, r8 to mov r6, sl (52/56 assembly lines). */
    Iwram_CopyWords(dst, src, size);
}

void Graphics_RenumberFillerEntries(void)
{
    u32 *p = (u32 *)gMapCellBuffer;
    u32 cnt = 128 << 7;
    u32 mask = 0xfff;
    s32 no = -1;

    do {
        u32 value = *p++;
        u32 idx = value & mask;

        if (idx == mask) {
            if (no != (s32)idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}

/* Converts the map at 0x02010000 into 0x02018000 with the ARM routine at
   0x0800a37c, run from a heap copy, against a saved copy of the source. */
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
            routine = (ConvertFn)Runtime_BumpAllocate((s32)code_size);
            Dma_Set((void *)Func_0800a37c, routine, 0x84000000 | (code_size >> 2),
                    (volatile u32 *)0x040000d4);
        } while (0);
    }
    routine((gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
    Runtime_BumpFree(routine);
    Runtime_BumpFree(saved);
}
