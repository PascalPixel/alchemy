#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "LAYOUT_GUARD.H"

/* The scrolling credits keep 128 hardware OAM entries, two words each. */
struct DisplayScrollObject {
    u32 attr01;
    u32 attr2;
};

LAYOUT_SIZE_GUARD(DisplayScrollObject_Size, struct DisplayScrollObject, 8);

extern u16 Data_02004c00;
extern s16 Flash_Handler0;
/* Flash and audio reuse these scratch cells in their own modes. */
extern struct DisplayScrollObject *gFlashNumRemainingBytes;
extern u32 gFrameTick;

extern s16 Flash_Layout;
extern const void *DisplayScroll_LineTable[];

void DisplayScroll_UpdateObjects(void);
void DisplayScroll_RenderEnteringLine(void);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

extern const u8 DisplayScroll_GlyphWidths[];
extern const u8 DisplayScroll_Font[];

/* Lay the 16 x 6 grid of wide text objects over the scroll position: each
   row is eight pixels lower less the position's fine offset, and the tiles
   continue from the position's line in the 0x300-tile ring. Copy the table
   to OAM, then advance the position every fourth frame unless a line is
   still being drawn. */
void DisplayScroll_UpdateObjects(void)
{
    /* FAKEMATCH: keep the existing two-word OAM walk over the real records;
       named field stores in f4f9d28 reverse stores and register allocation. */
    u32 frame = Data_02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = (u32 *)(gFlashNumRemainingBytes + 24);
    s32 row;

    for (row = 0; row <= 15; row++) {
        s32 col;

        for (col = 0; col < 6; col++) {
            u32 *q = entry;

            *q++ = (row * 8 + 16 - fine) | ((col * 32 + 24) << 16) | 0x40004000;
            *q = tile;
            tile += 4;
            entry += 2;
            if (tile == 0x300)
                tile = 0;
        }
    }
    Dma_Set(gFlashNumRemainingBytes, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
    if (Flash_Handler0 == 0 && (gFrameTick & 3) == 0)
        Data_02004c00++;
}

/* When no line is still being drawn and the scroll position has entered a
   new eight-pixel line, remember the position and draw that line's text
   into its slot of the 32-line ring (24 tiles per line, eight ahead). */
void DisplayScroll_RenderEnteringLine(void)
{
    u32 current;
    s32 line;

    if (Flash_Handler0 == 0) {
        current = Data_02004c00;
        if ((s16)Data_02004c00 / 8 != Flash_Layout / 8) {
            line = (s16)Data_02004c00 / 8;
            Flash_Layout = current;
            Flash_Handler0 = DisplayScroll_DrawLine(DisplayScroll_LineTable[line], ((line + 16) & 31) * 24, 1);
        }
    }
}

/* Allocate the scrolling display's object table, clear OBJ VRAM and fill its
   last tiles with colour 1, then lay out the entries: three strips of eight
   tall objects, a 16 x 6 grid of wide objects stepping 8 down and 32 across
   with 4 tiles each, and eight parked objects. Reset the scroll position and
   group, schedule the step and group callbacks, and load the 32 tile groups. */
void DisplayScroll_InitObjectTable(void)
{
    /* FAKEMATCH: retain the existing pointer-cell and postincrement word
       stores; direct typed stores in f4f9d28 change the 376-byte body to 368. */
    volatile u32 fill;
    u32 *entry;
    u32 i;
    u32 j;

    *(void **)((u8 *)&gFlashNumRemainingBytes) = Runtime_BumpAllocateAlternatePool(128 * sizeof(struct DisplayScrollObject));
    fill = 0;
    Dma_Set((void *)&fill, (void *)0x06010000, 0x85001800, (volatile u32 *)0x040000d4);
    fill = 0x11111111;
    Dma_Set((void *)&fill, (void *)0x06016000, 0x85000040, (volatile u32 *)0x040000d4);

    entry = *(u32 **)((u8 *)&gFlashNumRemainingBytes);
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x80004000;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x80004088;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x40004098;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 6; j++) {
            u32 *q = entry;
            u32 tile = (i * 3) * 8 + j * 4;

            *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
            *q = tile;
            entry += 2;
        }
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = 0x00c000c0;
        *q = 0x300;
        entry += 2;
    }
    Data_02004c00 = 0;
    Flash_Layout = 0;
    Flash_Handler0 = 0;
    Scheduler_AddOrUpdateCallback((s32)DisplayScroll_UpdateObjects, 0x480);
    Scheduler_AddOrUpdateCallback((s32)DisplayScroll_RenderEnteringLine, 0xc80);
    for (i = 0; i < 32; i++)
        DisplayScroll_DrawLine(DisplayScroll_LineTable[0], i * 24, 1);
}
