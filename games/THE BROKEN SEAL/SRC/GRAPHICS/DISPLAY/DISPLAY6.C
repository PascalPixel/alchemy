#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"

extern u16 Data_02004c00;
extern s16 Flash_Handler0;
extern u32 *gFlashNumRemainingBytes;
extern u32 gFrameTick;

extern s16 Flash_Layout;
extern const void *DisplayScroll_LineTable[];

void DisplayScroll_UpdateObjects(void);
void DisplayScroll_RenderEnteringLine(void);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
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
    u32 frame = Data_02004c00;
    u32 fine = frame & 7;
    s32 tile = (((s16)frame / 8) & 0x1f) * 3 * 8;
    u32 *entry = gFlashNumRemainingBytes + 48;
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
    volatile u32 fill;
    u32 *entry;
    u32 i;
    u32 j;

    *(void **)((u8 *)&gFlashNumRemainingBytes) = Runtime_BumpAllocateAlternatePool(0x400);
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

static __inline__ void DisplayScroll_CopyWords(void *destination, void *source, s32 size)
{
    /* FAKEMATCH: a direct call keeps the size in r5 for the fill that follows
       and puts the row total in r6; through this the size is built in r2. */
    Iwram_CopyWords(destination, source, size);
}

/* Draw one line of the scrolling text into 24 object tiles starting at
   `slot`: left aligned, centred (1) or right aligned (2) in 192 pixels.

   The line is drawn 8bpp into a 256-wide work buffer, each lit pixel in
   colour 15 with a shadow in colour 1 one pixel down and to the right, then
   packed to 4bpp in place and copied to object VRAM a tile column at a time.
   The buffer has nine pixel rows: the ninth holds the shadow that falls below
   the line, and becomes the top row of the next line drawn. */
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align)
{
    u8 *buf = Runtime_BumpAllocateAlternatePool(0x900);
    /* FAKEMATCH: the frame holds 32 bytes that nothing reads or writes;
       without them every stack offset is 32 lower. */
    u32 unused[8];
    s32 x = 0;
    s32 width = 192;
    const u8 *font = DisplayScroll_Font;
    s32 tiles;
    s32 pos;
    const u8 *p;
    u8 *dst;
    u32 c;
    s32 glyph;
    s32 w;
    s32 row;
    s32 bit;
    u32 bits;
    u32 mask;

    if (text == NULL)
        return -1;
    if (!GameFlag_TestFar(0x200)) {
        Iwram_FillWords(buf, 0x900, 0);
        GameFlag_SetBitFar(0x200);
    } else {
        DisplayScroll_CopyWords(buf, buf + 0x800, 0x100);
        Iwram_FillWords(buf + 0x100, 0x800, 0);
    }

    p = text;
    pos = 0;
    while ((c = *p++) != 0) {
        if (c > 31)
            pos += DisplayScroll_GlyphWidths[c - 32];
    }
    if (align == 2)
        x = width - pos;
    else if (align == 1)
        x = (width - pos) / 2;

    /* FAKEMATCH: nothing reads this pointer before the glyph row sets it;
       without the assignment the first character is fetched through r2 and
       the text pointer stepped with a constant in r3. */
    p = text;
    pos = 0;
    while ((c = *text++) != 0) {
        if (c > 31) {
            glyph = c - 32;
            p = font + glyph * 8;
            dst = buf + x + pos;
            for (row = 0; row < 8; row++) {
                bits = *p++;
                mask = 0x80;
                for (bit = 7; bit >= 0; bit--) {
                    if (bits & mask) {
                        dst[0x101] = 1;
                        dst[0] = 15;
                    }
                    dst++;
                    mask >>= 1;
                }
                dst += 248;
            }
            w = 1;
            if (c > 31)
                w = DisplayScroll_GlyphWidths[glyph];
            pos += w;
        }
    }

    tiles = width / 8;
    p = buf;
    dst = buf;
    for (row = 0; row < 8; row++) {
        for (bit = 0; bit < tiles * 4; bit++) {
            c = *p++;
            c |= *p++ << 4;
            *dst++ = c;
        }
        dst += 256 - tiles * 4;
        p += 256 - tiles * 8;
    }

    for (row = 0; row < tiles; row++) {
        s32 tile = slot + row;
        u32 *words = (u32 *)buf + row;

        *(u32 *)(0x06010000 + tile * 32) = words[0x000];
        *(u32 *)(0x06010004 + tile * 32) = words[0x040];
        *(u32 *)(0x06010008 + tile * 32) = words[0x080];
        *(u32 *)(0x0601000c + tile * 32) = words[0x0c0];
        *(u32 *)(0x06010010 + tile * 32) = words[0x100];
        *(u32 *)(0x06010014 + tile * 32) = words[0x140];
        *(u32 *)(0x06010018 + tile * 32) = words[0x180];
        *(u32 *)(0x0601001c + tile * 32) = words[0x1c0];
    }
    Runtime_BumpFree(buf);
    return 0;
}
