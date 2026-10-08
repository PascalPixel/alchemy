#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "iwram_call.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"

extern u8 gMapCellBuffer[];

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

static __inline__ void CopyWords(void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: needed to match: a direct call changes Tilemap_ConvertBuffer from mov r6, r8 to mov r6, sl (52/56 assembly lines). */
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
            if (no != idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}

void Tilemap_ConvertBuffer(void)
{
    u8 *saved;
    ConvertFn routine;
    u32 size;

    size = 0x8000;
    /* FAKEMATCH: needed to match: the two do-while blocks keep each allocation next to the transfer that uses it */
    do {
        saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
        CopyWords(saved, Ram_MapCellBuffer, size);
    } while (0);
    {
        u32 code_size = (u32)Tile_ConvertMapCodeSize;

        do {
            routine = (ConvertFn)MemAlloc(code_size);
            Dma_Set((void *)Tile_ConvertMap, routine, 0x84000000 | (code_size >> 2),
                    REG_DMA3);
        } while (0);
    }
    routine((gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
    MemFree(routine);
    MemFree(saved);
}
