#include "TYPES.H"
#include "TBS_EDITION.H"

/* The glyph renderer. The Japanese one also joins a kana voicing mark to the
   kana before it, and draws its sprites two pixels lower. */

extern u8 *gWindowWork;

void *RenderOutput_AcquireFree(void);
s32 Resource_FindFreeEntry(void);
void RenderOutput_AppendToList(void *, s8 *);

struct UiWindow {
    u8 padding0[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

struct SpriteAttr {
    u8 unk0[4];
    u8 y;
    u8 unk5;
    u16 x : 9;
    u16 unk6 : 7;
    u32 unk8;
};

struct RenderOutput {
    s32 zero;
    u8 one4;
    u8 one5;
    s16 x;
    s16 y;
    u8 unknown_0a[4];
    s8 index;
    u8 sentinel;
    struct SpriteAttr attr;
};

struct WindowTilemap {
    u16 tiles[640];
};

#if EDITION_INTERNATIONAL

/* Places a glyph: mode 1 queues it as a sprite at the window cell, other
   modes write tiles up to 0xff into the window tilemap. */
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    struct RenderOutput *out = (struct RenderOutput *)gWindowWork;
    u8 *base = (u8 *)out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    if (y > (u32)(win->height - 2))
        return;
    if (x > (u32)(win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        out = RenderOutput_AcquireFree();
        if (out == NULL)
            return;
        idx = (out - (struct RenderOutput *)(base + 0x698)) * 4;
        out->one5 = 2;
        attr = &out->attr;
        slot = (u16 *)(base + 0x12b6);
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        /* FAKEMATCH: the volatile width read is scheduled before the column constant's pool load, as in the reference. */
        attr->x = (win->x + (column + *(volatile u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 - 1;
        out->x = attr->x;
        out->y = attr->y;
        out->zero = 0;
        out->index = idx;
        if (out->one5 == 0)
            out->one5 = mode;
        RenderOutput_AppendToList(win, (s8 *)out);
    } else if (tile <= 0xff) {
        x++;
        y++;
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct WindowTilemap *)out)->tiles[pos] = tile | 0xf000;
    }
}

#else

/* Places a glyph: mode 1 queues it as a sprite at the window cell, other
   modes write tiles up to 0xff into the window tilemap. */
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    /* FAKEMATCH: the game keeps the work block in r12 for the tile store and a copy in r8 for the rest; as one plain variable it lives in r8 alone. */
    register u8 *work asm("r12") = gWindowWork;
    u8 *base = work;
    struct RenderOutput *out;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    if (y > (u32)(win->height - 2))
        return;
    if (x > (u32)(win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        u16 left;
        out = RenderOutput_AcquireFree();
        if (out == NULL)
            return;
        idx = (out - (struct RenderOutput *)(base + RENDER_OUTPUT_TBL_OFS)) * 4;
        out->one5 = 2;
        attr = &out->attr;
        slot = (u16 *)(base + RENDER_COUNTER_OFS);
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        left = win->x;
        /* FAKEMATCH: the volatile width read is scheduled before the column constant's pool load, as in the reference. */
        attr->x = (left + (column + *(volatile u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 + 1;
        out->x = attr->x;
        out->y = attr->y;
        out->zero = 0;
        out->index = idx;
        if (out->one5 == 0)
            out->one5 = mode;
        RenderOutput_AppendToList(win, (s8 *)out);
    } else if (tile <= 0xff) {
        /* The voicing marks 0xde and 0xdf go into the cell before them,
           joined to the kana tile 0x0e or 0x11 already there. */
        if (tile - 0xde <= 1) {
            u32 line;

            line = (win->y + y) * 32;
            switch (*(((struct WindowTilemap *)base)->tiles + (line + (win->x + x)))) {
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
            ((struct WindowTilemap *)work)->tiles[pos] = tile | 0xf000;
    }
}

#endif
