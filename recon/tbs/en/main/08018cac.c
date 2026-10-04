/* Draft, not exact: score 450, 13 differing rows (was 8065/151); two of
 * them are the link-time names below, not yet defined.
 * Remaining: case 3 of the outline switch stores width's register (mov r2,
 * r9, which CSE knows holds 5) where the listing loads #5; in the cursor
 * branch the 0x12b6 offset takes r3 not r2 and the window copy r1 not r3.
 * Found: one shared `return width` (space cases jump to it); u32 spacing
 * locals (ldrh, lsr); the glyph width through its own temporary; the cursor
 * position as (x << 3) + ((width - 2) << 3) into u16 bitfields, which keeps
 * the narrowed 0xfffe / 254 apart; plain u16 stores for the attributes
 * (a union lets the glyph_tiles load pass the attr1 store); a sprite
 * pointer at output+16 whose bitfields force its address into r7 (GCSE
 * places it at the end of the other branch); gWindowWork[22] for the
 * battle display. To link: row 20 of DIRECTORY.S (UiText_SmallFont) needs
 * its ResourceId name and Func_080155d0 a CodeSize symbol after its last
 * byte; Japanese is 0x18 bytes longer and needs its own branch. */
#include "WINDOW.H"
#include "BATTLE_WORK.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "RUNTIME_MEM.H"
#include "DMA.H"
#include "IO_REG.H"

extern const u8 Func_080155d0[];
extern u8 UiText_DrawGlyphCodeSize[];
extern char ResourceId_SmallFont;

typedef s32 (*GlyphRoutine)(struct UiWindow *, s32, s32, s32, u8 *);

/* The sprite a glyph output carries, from the output's sixteenth byte. */
struct GlyphSprite {
    u32 unknown_00;
    struct {
        u16 y : 8;
        u16 flags : 8;
        u16 x : 9;
        u16 unknown_attr1 : 7;
    } pos;
    u16 tile;
    u16 unknown_0a;
};

#define OAM(output) ((struct GlyphSprite *)(output)->unknown_10)

s32 UiText_DrawGlyph(struct UiWindow *window, s32 character,
    s32 x, s32 y, s32 mode)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
    u32 spacing_x = work->unknown_before_count;
    u32 spacing_y = work->line_spacing;
    s32 width;

    if (mode != 1 && (window->flags & 8) != 0) {
        u8 *font;
        GlyphRoutine draw;
        u32 size;
        s32 result;

        if (((struct BattleDisplayWork *)gWindowWork[22])->window == window) {
            Resource_GetTableEntry((s32)&ResourceId_SmallFont);
            Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
            width = 3;
            if (character == ' ')
                goto done;
        }
        font = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
        width = 4;
        if (character == ' ')
            goto done;
        size = (u32)UiText_DrawGlyphCodeSize;
        draw = (GlyphRoutine)Runtime_BumpAllocate(size);
        Dma_Set(Func_080155d0, draw, 0x84000000 | (size >> 2), REG_DMA3);
        result = draw(window, character, x, y, font);
        Runtime_BumpFree(draw);
        return result;
    }

    width = 5;
    if (character == ' ')
        goto done;
    {
        u32 tiles[32];
        struct RenderOutput *output;
        s32 index;
        struct GlyphSprite *sprite;

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
            {
                s32 drawn = UiText_RenderGlyphPair(character, tiles);

                if (drawn == 0)
                    drawn = 1;
                width = drawn;
            }
        }

        if ((u8)output->active == 2) {
            sprite = OAM(output);
            if (work->glyph_resource == 99)
                work->glyph_resource = Resource_FindFreeEntry();
            sprite->pos.x = (window->x << 3) + ((window->width - 2) << 3) + 4;
            sprite->pos.y = (window->y << 3) + ((window->height - 2) << 3) - 1;
        } else {
            Dma_Set(tiles, (void *)(0x06010000 + (work->glyph_tiles + index) * 32),
                0x84000020, REG_DMA3);
            ((u16 *)&output->packed)[0] = y + (spacing_y >> 1) + window->y * 8 - 2;
            ((u16 *)&output->packed)[1] = (x + (spacing_x >> 1) + window->x * 8 + 2) | 0x4000;
            ((u16 *)&output->packed)[2] = work->glyph_tiles + index;
            sprite = OAM(output);
        }
        output->sentinel = 254;
        output->x = sprite->pos.x;
        output->y = sprite->pos.y;
        output->index = index;
        output->next = NULL;
        RenderOutput_AppendToList(&window->output, output);
    }
done:
    return width;
}
