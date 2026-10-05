/*
 * Draft: Bg0_ClearTilemap, ☀️'s MEMORY.C body; opens ⚓️'s
 * SYSTEM/MEMORY/MEMORY.C. Score 2: the listing issues the fill constant's
 * pool load before sub sp. Data_030011c4 is ☀️'s gDebugTextCursor.
 */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

extern u16 *Data_030011c4;
extern const u8 Data_08017b10[];
extern const u16 Ui_WindowPalette[];
extern u8 gIwramHeap[];
extern u8 gEwramHeap[];
extern u8 IwramSoundMixWorkspace[];
extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32);

/* Fill the BG0 screen block at 0x06002000 with blank tiles and home the cursor. */
void Bg0_ClearTilemap(void)
{
    volatile u32 fill;

    fill = 0xf000f000;
    Dma_Set((const void *)&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    /* FAKEMATCH: the cursor store sits in a do-while(0) to hold the BG0CNT constant load order */
    do {
        Data_030011c4 = (u16 *)0x06002000;
    } while (0);
    {
        s32 control = 0x400;

        *(volatile u16 *)0x04000008 = control;
    }
}
