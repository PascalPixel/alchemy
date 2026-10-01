/* Draft, not exact: 108 differing halfwords / 75 aligned edits, 540-byte
   candidate for the 540-byte owner, freshly scored as a unit binding
   __divsi3 0x080022ec and
   __modsi3 0x080022fc. Residual: register allocation only; the reference
   spills the item code to sp+8 and keeps the layer mask in fp, this
   candidate keeps the code in fp and spills the item pointer and overlay
   flag. Declaration order does not change it.
   Score as --unit candidate-item-icon-compose: compiler division/modulo
   helpers are bound to the two calls proved by the owner's listing.
   Explicit __modsi3/__divsi3 calls and a cached quotient give 536 bytes /
   81 aligned edits; count spills and code still occupies fp. A volatile
   code parameter adds an address register and grows the frame from 12 to
   16 bytes (544 bytes / 108 edits), not the reference's ordinary spill.
   Keep the operator model; neither experiment establishes the missing
   lifetime relationship. No exact bytes claimed. */

#include "ITEM.H"

/* A 16x16 icon composed from layered compressed glyphs. */
struct UiGlyph {
    u8 pixels[0x600];
    u16 width;
    u16 height;
    const void *source;
};

void UiGlyph_DecodeWithHeapRoutines(struct UiGlyph *glyph, s32 overlay);

extern struct UiGlyph *gGlyphWork;
extern const void *const UiIcon_FramePointerTable[];
extern const void *const Data_08029acc[];
extern const void *const Data_08029b68[];
extern const void *const UiIcon_ItemIconPointers[];

#define ICON_CURSED 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers)
{
    struct ItemDefinition *item;
    struct UiGlyph *glyph;
    s32 overlay;
    s32 count;

    overlay = 0;
    count = 0;
    item = Item_Get(code & 0x1ff);
    glyph = gGlyphWork;
    if (glyph == NULL)
        return -1;
    if (layers & ICON_CURSED) {
        glyph->source = UiIcon_FramePointerTable[2];
        glyph->width = 2;
        glyph->height = 2;
        UiGlyph_DecodeWithHeapRoutines(glyph, 0);
        overlay = 1;
    }
    glyph->source = UiIcon_ItemIconPointers[item->icon];
    glyph->width = 2;
    glyph->height = 2;
    UiGlyph_DecodeWithHeapRoutines(glyph, overlay);
    if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
        glyph->source = Data_08029acc[1];
        glyph->width = 2;
        glyph->height = 2;
        UiGlyph_DecodeWithHeapRoutines(glyph, 1);
    }
    if ((layers & ICON_BROKEN) && (code & 0x200)) {
        glyph->source = Data_08029acc[0];
        glyph->width = 2;
        glyph->height = 2;
        UiGlyph_DecodeWithHeapRoutines(glyph, 1);
    }
    if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2)) {
        glyph->source = Data_08029acc[2];
        glyph->width = 2;
        glyph->height = 2;
        UiGlyph_DecodeWithHeapRoutines(glyph, 1);
    }
    if (layers & ICON_COUNT_ABOVE_ONE) {
        count = ((code & 0xf800) >> 11) + 1;
        if (count <= 1)
            count = 0;
    }
    if (layers & ICON_COUNT)
        count = ((code & 0xf800) >> 11) + 1;
    if (count != 0 && count <= 30) {
        glyph->source = Data_08029b68[count % 10];
        glyph->width = 2;
        glyph->height = 2;
        UiGlyph_DecodeWithHeapRoutines(glyph, 1);
        if (count / 10 != 0) {
            glyph->source = Data_08029b68[count / 10 + 9];
            glyph->width = 2;
            glyph->height = 2;
            UiGlyph_DecodeWithHeapRoutines(glyph, 1);
        }
    }
    return 256;
}
