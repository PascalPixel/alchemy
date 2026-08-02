/*
 * The two relocated routines are ordinary calls through IWRAM function
 * pointers.  The original branch targets are GCC call-via-register veneers,
 * not embedded instructions.
 */
#include "types.h"

enum {
    TEXT_SURFACE_WIDTH_080f07f0 = 0xc0,
    GLYPH_HEIGHT_080f07f0 = 8,
    TILE_COUNT_080f07f0 = 0x18,
    PIXEL_BUFFER_SIZE_080f07f0 = 0x900
};

typedef void *(*WordCopy_080f07f0)(
    void *destination,
    const void *source,
    s32 size);
typedef void (*ArmFill_080f07f0)(
    void *destination,
    u32 size,
    u32 value);

void Func_08002df0(void *);
u8 *Func_08004970(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);

static const u8 *GlyphWidths_080f07f0(void)
{
    return (const u8 *)0x080f11bd;
}

static const u8 *GlyphRows_080f07f0(u8 character)
{
    return (const u8 *)0x080f1770 + (character - 0x20) * 8;
}

static s32 MeasureText_080f07f0(const u8 *text)
{
    const u8 *widths = GlyphWidths_080f07f0();
    s32 width = 0;

    while (*text != 0) {
        u8 character = *text++;

        if (character > 0x1f)
            width += widths[character - 0x20];
    }
    return width;
}

static void DrawGlyph_080f07f0(
    u8 *pixels,
    s32 x,
    u8 character)
{
    const u8 *rows = GlyphRows_080f07f0(character);
    s32 row;

    for (row = 0; row < GLYPH_HEIGHT_080f07f0; row++) {
        u8 bits = rows[row];
        s32 column;

        for (column = 0; column < 8; column++) {
            if ((bits & (0x80 >> column)) != 0) {
                pixels[x + row * 0x100 + column] = 0x0f;
                pixels[x + row * 0x100 + column + 0x101] = 1;
            }
        }
    }
}

static void PackPixels_080f07f0(u8 *pixels)
{
    s32 row;

    for (row = 0; row < GLYPH_HEIGHT_080f07f0; row++) {
        u8 *source = pixels + row * 0x100;
        u8 *destination = pixels + row * 0x100;
        s32 packed_x;

        for (packed_x = 0; packed_x < 0x60; packed_x++) {
            destination[packed_x] =
                source[packed_x * 2] |
                (source[packed_x * 2 + 1] << 4);
        }
    }
}

static void CopyTilesToVram_080f07f0(u8 *pixels, s32 first_tile)
{
    volatile u32 *vram =
        (volatile u32 *)(0x06010000 + (first_tile << 5));
    s32 tile;

    for (tile = 0; tile < TILE_COUNT_080f07f0; tile++) {
        s32 row;

        for (row = 0; row < GLYPH_HEIGHT_080f07f0; row++)
            vram[row] = *(u32 *)(pixels + row * 0x100);
        vram += 8;
        pixels += 4;
    }
}

/* Rasterize one line into 24 4bpp tiles and upload them to character VRAM. */
s32 Func_080f07f0(const u8 *text, s32 first_tile, s32 alignment)
{
    ArmFill_080f07f0 fill = (ArmFill_080f07f0)0x03000168;
    WordCopy_080f07f0 copy = (WordCopy_080f07f0)0x03001388;
    u8 *pixels = Func_08004970(PIXEL_BUFFER_SIZE_080f07f0);
    s32 x = 0;

    if (text == 0)
        return -1;

    if (Func_080770c0(0x200) == 0) {
        fill(pixels, PIXEL_BUFFER_SIZE_080f07f0, 0);
        Func_080770c8(0x200);
    } else {
        /* Preserve the shadow spill from the preceding surface segment. */
        copy(pixels, pixels + 0x800, 0x100);
        fill(pixels + 0x100, 0x800, 0);
    }

    {
        s32 remaining = TEXT_SURFACE_WIDTH_080f07f0 -
            MeasureText_080f07f0(text);

        if (alignment == 2)
            x = remaining;
        else if (alignment == 1)
            x = (s32)((u32)remaining + ((u32)remaining >> 31)) >> 1;
    }

    while (*text != 0) {
        u8 character = *text++;

        if (character > 0x1f) {
            DrawGlyph_080f07f0(pixels, x, character);
            x += GlyphWidths_080f07f0()[character - 0x20];
        }
    }

    PackPixels_080f07f0(pixels);
    CopyTilesToVram_080f07f0(pixels, first_tile);
    Func_08002df0(pixels);
    return 0;
}
