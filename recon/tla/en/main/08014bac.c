/*
 * Draft: Ui_LoadWindowGraphics, ☀️'s MEMORY.C body: matches as it stands, but
 * sits between Bg0_ClearTilemap and Runtime_InitializeHeap, which do not,
 * ahead of ⚓️'s MEMORY.C. Needs ResourceId_WindowTiles named on directory row
 * 0x13 (Ui_WindowTiles). Data_08017af0 and Data_08017b10 are ☀️'s
 * Ui_WindowPalette and System_BasicColorPalette.
 */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

extern u16 *Data_030011c4;
extern const u8 Data_08017b10[];
extern const u16 Data_08017af0[];
extern u8 Data_03001300[];
extern u8 Data_02030000[];
extern u8 IwramSoundMixWorkspace[];
extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32);

/* Loads the shared window graphics: resource 0x13 into BG character block
   0, the window palette into background bank 15, the bank-15 frame colours
   and the object palettes. */
void Ui_LoadWindowGraphics(void)
{
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowTiles), (void *)0x06000000, 0x84000800,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)Data_08017af0, (void *)0x050001e0, 0x80000010,
            (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the two do-while blocks keep the value-before-address
       order of the palette stores */
    do {
        s32 black = 0;
        *(volatile u16 *)0x05000000 = black;
    } while (0);
    do {
        volatile u16 *pal;
        s32 color;

        color = 0x4180;
        pal = (volatile u16 *)0x050001e8;
        *pal = color;
        color = 0x3960;
        pal++;
        *pal = color;
        color = 0x3140;
        pal++;
        *pal = color;
        color = 0x2920;
        pal++;
        *pal = color;
        color = 0x49a0;
        pal++;
        *pal = color;
        color = 0x51c0;
        pal++;
        *pal = color;
        color = 0x59e0;
        pal++;
        *pal = color;
    } while (0);
    Dma_Set((const void *)Data_08017b10, (void *)0x05000200, 0x800000e0,
            (volatile u32 *)0x040000d4);
}
