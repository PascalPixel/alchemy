/* Native DisplayScroll_DrawLine: complete 620 bytes including its literal pool.
 * Ordinary TBS flags, freshly linked in all six editions, 2026-10-02.
 * This plain source differs at exactly two byte positions (+0x12 and +0x226):
 * stack reservation/release are 12 bytes; native code uses 44. All working
 * stack accesses, calls and pool words match. No unread reservation is kept.
 * Incoming unread u32[8] matched 620 bytes, but was refused under S2: its
 * sole effect is the frame reserve/release extent; removing it changes no
 * working stack offsets. That attempt is recorded here, not kept as source.
 * Four ordinary EN trials: declarations-before-call 620/26, scoped glyph
 * locals 612/249, real glyph-state aggregate 712/613, tile packet 640/474
 * (emitted bytes/differing bytes, complete linked extents).
 * The measured CopyWords/register-rematerialization devices remain tagged.
 */
#include "TYPES.H"
#include "RUNTIME_MEM.H"
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
void Runtime_BumpFree(void *block);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_SetBitFar(s32 flag);
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);

extern const u8 DisplayScroll_GlyphWidths[];
extern const u8 DisplayScroll_Font[];

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
    u8 *buf = (u8 *)Runtime_BumpAllocateAlternatePool(0x900);
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
