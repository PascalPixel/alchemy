/*
 * Draft: UiText_DrawGlyph, complete English owner [08018cac, 08018efc).
 * The 2026-09-29 permuter trial reduced its score from 7965 to 6875 with
 * allocation and scheduling rewrites. The current approved compiler scored
 * that retained draft 6855 (152 differing rows) before this correction.
 *
 * 2026-10-03: replace its private window/work/output records with maintained
 * owners, use physical labels for the copied ARM routine and display work,
 * and call the allocated code buffer through its actual five-argument type.
 * The old permuter temporaries, register hints and operand shuffles are
 * removed. Whole-owner score: 8270 (45 register-only, 29 operand, 22
 * reordered, 25 inserted, 36 deleted rows). A shared width-result local
 * gives the same score as direct space-width returns. The initial six-byte
 * OAM view failed layout guards because this compiler rounds nested records
 * to words; the eight-byte packed/table union below passes without packing.
 * Remaining: register allocation, stack/branch layout and literal addressing
 * still differ, including the separately named display-work cell load.
 * The ARM routine remains uncredited disassembly; this draft is not adopted.
 */
#include "WINDOW.H"
#include "BATTLE_WORK.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "IO_REG.H"

extern const u8 Func_080155d0[];

/* Glyph outputs interpret the packed words as the three OAM attributes.
 * Cursor placement changes only Y, retaining the other byte of attr0. */
union GlyphSpriteAttributes {
    struct {
        u16 vertical;
        u16 horizontal;
        u16 tile;
        u16 high;
    } attributes;
    struct {
        u8 y;
        u8 flags;
    } bytes;
};

LAYOUT_SIZE_GUARD(GlyphSpriteAttributes_Size, union GlyphSpriteAttributes,
    sizeof(((struct RenderOutput *)0)->packed) +
    sizeof(((struct RenderOutput *)0)->table));
LAYOUT_OFFSET_GUARD(GlyphSpriteAttributes_Horizontal,
    union GlyphSpriteAttributes, attributes.horizontal, 2);
LAYOUT_OFFSET_GUARD(GlyphSpriteAttributes_Tile,
    union GlyphSpriteAttributes, attributes.tile,
    (u32)&((struct RenderOutput *)0)->table -
    (u32)&((struct RenderOutput *)0)->packed);

s32 UiText_DrawGlyph(struct UiWindow *window, s32 character,
    s32 x, s32 y, s32 mode)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 tiles[32];
    struct RenderOutput *output;
    union GlyphSpriteAttributes *sprite;
    u8 *resource;
    void *code;
    s32 (*draw)(struct UiWindow *, s32, s32, s32, u8 *);
    s32 size;
    s32 width;
    s32 index;
    s32 tile;
    u16 offset_x = work->unknown_before_count;
    u16 offset_y = work->line_spacing;

    width = 5;
    if (mode != 1 && (window->flags & 8) != 0) {
        if (gBattleDisplayWork->window == window) {
            Resource_GetTableEntry(0x14);
            Resource_GetTableEntry(0x13);
            width = 3;
            if (character == 32)
                return width;
        }
        resource = Resource_GetTableEntry(0x13);
        width = 4;
        if (character == 32)
            return width;
        size = 0x318;
        code = Runtime_BumpAllocate(size);
        Dma_Set(Func_080155d0, code, 0x84000000 | (size >> 2),
            REG_DMA3);
        draw = (s32 (*)(struct UiWindow *, s32, s32, s32, u8 *))code;
        width = draw(window, character, x, y, resource);
        Runtime_BumpFree(code);
        return width;
    }

    if (character == 32)
        return width;
    output = RenderOutput_AcquireFree();
    if (output == NULL)
        return 0;
    index = (output - work->outputs) * 4;
    output->active = 1;
    output->kind = 0;
    if (mode == 1) {
        width = 1;
        output->active = 2;
    } else {
        switch (work->outline) {
        case 3:
            output->active = 5;
            break;
        case 4:
            output->active = 6;
            output->unknown_0c = 8;
            break;
        case 5:
            output->active = 7;
            output->unknown_0c = 0;
            break;
        case 2:
            output->active = 4;
            output->unknown_0c = 0;
            break;
        }
        width = UiText_RenderGlyphPair(character, tiles);
        if (width == 0)
            width = 1;
    }

    sprite = (union GlyphSpriteAttributes *)&output->packed;
    if ((u8)output->active == 2) {
        if (work->glyph_resource == 99)
            work->glyph_resource = Resource_FindFreeEntry();
        sprite->attributes.horizontal = (sprite->attributes.horizontal & ~0x1ff) |
            ((((window->x + window->width + 0xfffe) << 3) + 4) & 0x1ff);
        sprite->bytes.y =
            (((u8)window->y + (u8)window->height + 254) << 3) - 1;
    } else {
        tile = work->glyph_tiles + index;
        Dma_Set(tiles, (void *)(0x06010000 + (tile << 5)), 0x84000020,
            REG_DMA3);
        sprite->attributes.vertical =
            y + (offset_y >> 1) + (window->y << 3) + 0xfffe;
        sprite->attributes.horizontal =
            ((window->x << 3) + x + (offset_x >> 1) + 2) | 0x4000;
        sprite->attributes.tile = tile;
    }
    output->sentinel = 254;
    output->x = sprite->attributes.horizontal & 0x1ff;
    output->y = sprite->bytes.y;
    output->index = index;
    output->next = NULL;
    RenderOutput_AppendToList(&window->output, output);
    return width;
}
