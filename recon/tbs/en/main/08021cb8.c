/* NONMATCHING: 208 of 208 bytes, 3 differing halfwords (2026-09-25).
 * Snapshotting the full palette count before the byte store and inlining
 * the remap branch reduced 17 halfword edits to 3. The initial slot load
 * still uses r3 instead of r2, reverses its copy, and advances src early.
 * A separate test argument and narrow helper types did not improve it.
 */

#include "DMA.H"

/* A text window's glyph palette: the slot each font colour was given, 0xff
   while unassigned, and the number of slots used. */
struct GlyphPalette {
    u8 slot[256];
    s32 count;
};

extern u8 Value_000000f1;
u8 *Runtime_AllocateHeapBlock(s32 slot, s32 size);
u8 *Resource_GetTableEntry(u32 index);
s32 Resource_DecodeByteLz(const void *source, void *destination);
u8 *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
void Runtime_ReleaseHeapBlock(s32 slot);

/* FAKEMATCH: this inline boundary preserves a separate input and return
 * value for the remap branch, as in the reference. */
static __inline__ u32 RemapColour(struct GlyphPalette *palette, u32 index, u32 colour)
{
    if (colour == 255) {
        s32 count = palette->count;
        palette->slot[index] = count;
        if (palette->count <= 63) {
            ((u16 *)0x05000000)[palette->count] = ((u16 *)0x05000200)[index];
            palette->count++;
            colour = palette->slot[index];
        } else {
            colour = count;
        }
    }
    return colour;
}

/* Decodes one glyph of the font resource, remaps its colours through the
   window's glyph palette (assigning and uploading new colours while slots
   remain) and copies the tile to character block 1. */
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile)
{
    u8 *decoded;
    u8 *font;
    u8 *remapped;
    u8 *src;
    u8 *dst;
    s32 i;
    u32 index;
    u32 colour;

    decoded = Runtime_AllocateHeapBlock(17, 0x608);
    font = Resource_GetTableEntry((u32)&Value_000000f1);
    *(u8 **)(decoded + 0x604) = font + ((u16 *)font)[glyph];
    Resource_DecodeByteLz(*(u8 **)(decoded + 0x604), decoded);
    remapped = Runtime_BumpAllocate(0x400);
    src = decoded;
    dst = remapped;
    i = 0;
loop:
    {
        index = *src++;
        colour = palette->slot[index];
        colour = RemapColour(palette, index, colour);
        *dst++ = colour;
    }
    if (++i < 0x400)
        goto loop;
    Dma_Set(remapped, (void *)(0x06004000 + tile * 64), 0x84000100, (volatile u32 *)0x040000d4);
    Runtime_BumpFree(remapped);
    Runtime_ReleaseHeapBlock(17);
}
