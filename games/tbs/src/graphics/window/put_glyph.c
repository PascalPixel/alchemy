#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "window.h"
#include "resource.h"
#include "tbs_edition.h"
#include "btlwork.h"
#include "resource_ids.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"
#include "edition.h"
#include "text_font.h"


typedef s32 (*GlyphRoutine)(struct UiWindow *, s32, s32, s32, u8 *);

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
    s32 width;
    u32 spacing_y = work->line_spacing;

    if (mode != 1 && (window->flags & 8) != 0) {
        u8 *font;
        GlyphRoutine draw;
        s32 result;
        u32 size;
#if !EDITION_INTERNATIONAL
        s32 mark;

        font = NULL;
        if (((struct BattleDisplayWork *)gWindowWork[22])->window == window) {
            font = Resource_GetTableEntry((s32)&ResourceId_SmallFont);
            width = 3;
            if (character == ' ')
                goto done;
        }
        mark = (s16)UiText_MarkGlyphCodes[(u32)character >> 14];
        character &= 0x3fff;
        width = 4;
        if (character == ' ')
            goto done;
#else
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
#endif
        size = (u32)UiText_DrawGlyphCodeSize;
        draw = (GlyphRoutine)MemAlloc(size);
        Dma_Set(UiText_DrawGlyphArm, draw, 0x84000000 | (size >> 2), REG_DMA3);
        result = draw(window, character, x, y, font);
#if !EDITION_INTERNATIONAL
        if ((u16)mark != 0)
            result += draw(window, (u16)mark, x + result, y, font);
#endif
        MemFree(draw);
        return result;
    }

    if (character == ' ') {
#if !EDITION_INTERNATIONAL
        width = 7;
#else
        width = 5;
#endif
        goto done;
    }
    {
        u32 tiles[32];
        struct RenderOutput *output;
        struct GlyphSprite *sprite;
        s32 index;

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
                output->anim_step = 8;
                break;
            case 5:
                output->active = 7;
                output->anim_step = 0;
                break;
            case 2:
                output->active = 4;
                output->anim_step = 0;
                break;
            }
#if !EDITION_INTERNATIONAL
            width = UiText_RenderGlyphPair(character, tiles);
#else
            {
                s32 drawn = UiText_RenderGlyphPair(character, tiles);

                if (drawn == 0)
                    drawn = 1;
                width = drawn;
            }
#endif
        }

        if ((u8)output->active == 2) {
            sprite = OAM(output);
            if (work->glyph_resource == 99)
                work->glyph_resource = Resource_FindFreeEntry();
            sprite->pos.x = (window->x << 3) + ((window->width - 2) << 3) + 4;
#if !EDITION_INTERNATIONAL
            sprite->pos.y = (window->y << 3) + ((window->height - 2) << 3) + 1;
#else
            sprite->pos.y = (window->y << 3) + ((window->height - 2) << 3) - 1;
#endif
        } else {
            Dma_Set(tiles, (void *)(0x06010000 + (work->glyph_tiles + index) * 32),
                0x84000020, REG_DMA3);
#if !EDITION_INTERNATIONAL
            ((u16 *)&output->packed)[0] = y + (spacing_y >> 1) + window->y * 8;
#else
            ((u16 *)&output->packed)[0] = y + (spacing_y >> 1) + window->y * 8 - 2;
#endif
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

struct SpriteAttr {
    u8 unk0[4];
    u8 y;
    u8 unk5;
    u16 x : 9;
    u16 unk6 : 7;
    u32 unk8;
};

struct GlyphSpriteOutput {
    s32 link_word;
    u8 kind;
    u8 active;
    s16 x;
    s16 y;
    u8 unknown_0a[4];
    s8 index;
    u8 sentinel;
    struct SpriteAttr attr;
};

LAYOUT_SIZE_GUARD(GlyphSpriteOutput_Size, struct GlyphSpriteOutput,
    sizeof(struct RenderOutput));
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_X, struct GlyphSpriteOutput, x,
    (u32)&((struct RenderOutput *)0)->x);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Y, struct GlyphSpriteOutput, y,
    (u32)&((struct RenderOutput *)0)->y);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Active, struct GlyphSpriteOutput, active,
    (u32)&((struct RenderOutput *)0)->active);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Index, struct GlyphSpriteOutput, index,
    (u32)&((struct RenderOutput *)0)->index);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Attr, struct GlyphSpriteOutput, attr,
    (u32)&((struct RenderOutput *)0)->unknown_10);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Oam, struct GlyphSpriteOutput, attr.y,
    (u32)&((struct RenderOutput *)0)->packed);
LAYOUT_OFFSET_GUARD(GlyphSpriteOutput_Table, struct GlyphSpriteOutput, attr.unk8,
    (u32)&((struct RenderOutput *)0)->table);

#if EDITION_INTERNATIONAL

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    void *work = gWindowWork[0];
    u8 *base = work;
    struct GlyphSpriteOutput *out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    if (y > (win->height - 2))
        return;
    if (x > (win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        work = RenderOutput_AcquireFree();
        if (work == NULL)
            return;
        out = work;
        idx = ((struct RenderOutput *)out -
            ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = &out->attr;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        attr->x = (win->x + (column + *(u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 - 1;
        /* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
        out->x = attr->x;
        out->y = attr->y;
        out->link_word = 0;
        out->index = idx;
        if (out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
    } else if (tile <= 0xff) {
        x++;
        y++;
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
    }
}

#else

void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    /* FAKEMATCH: the game keeps the work block in r12 for the tile store and a copy in r8 for the rest; as one plain variable it lives in r8 alone. */
    register u8 *work asm("r12") = gWindowWork[0];
    u8 *base = work;
    struct GlyphSpriteOutput *out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    if (y > (win->height - 2))
        return;
    if (x > (win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        u16 left;
        out = (struct GlyphSpriteOutput *)RenderOutput_AcquireFree();
        if (out == NULL)
            return;
        idx = ((struct RenderOutput *)out -
            ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = &out->attr;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        left = win->x;
        attr->x = (left + (column + *(u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 + 1;
        /* FAKEMATCH: needed to match: the embedded OAM view preserves packed/logical position store order changed by byte-span and scalar views. */
        out->x = attr->x;
        out->y = attr->y;
        out->link_word = 0;
        out->index = idx;
        if (out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, (struct RenderOutput *)out);
    } else if (tile <= 0xff) {
        if (tile - 0xde <= 1) {
            u32 line;

            line = (win->y + y) * 32;
            switch (*(((struct UiRenderWork *)base)->tilemap + (line + (win->x + x)))) {
            case 0xf011:
                tile -= 0xc0;
                break;
            case 0xf00e:
                tile -= 0xd0;
                break;
            }
        } else {
            x++;
            y++;
        }
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
    }
}

#endif
