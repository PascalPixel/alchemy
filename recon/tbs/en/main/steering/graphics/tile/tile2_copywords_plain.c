/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords direct-call attempt.
 * Remaining difference: a direct call changes Tilemap_ConvertBuffer from mov r6, r8 to mov r6, sl (52/56 assembly lines).
 * Measured with TBS's existing agscc option set; no option changes.
 * Production retains the measured boundary with its reason inside the helper.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];

extern const u8 Func_0800a37c[];
typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void *Runtime_BumpAllocate(u32 size);
void Runtime_BumpFree(void *allocation);
extern u8 Tile_ConvertMapCodeSize[];


void Graphics_RenumberFillerEntries(void)
;

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
        Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
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
