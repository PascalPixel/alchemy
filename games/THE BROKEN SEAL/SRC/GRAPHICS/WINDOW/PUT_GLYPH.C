#include "TYPES.H"
#include "WINDOW.H"
#include "RESOURCE.H"
#include "TBS_EDITION.H"

/* The glyph renderer. The Japanese one also joins a kana voicing mark to the
   kana before it, and draws its sprites two pixels lower. */


struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_AppendToList(struct RenderOutputList *, struct RenderOutput *);


struct SpriteAttr {
    u8 unk0[4];
    u8 y;
    u8 unk5;
    u16 x : 9;
    u16 unk6 : 7;
    u32 unk8;
};

#if EDITION_INTERNATIONAL

/* Places a glyph: mode 1 queues it as a sprite at the window cell, other
   modes write tiles up to 0xff into the window tilemap. */
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    void *work = gWindowWork[0];
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
        work = RenderOutput_AcquireFree();
        if (work == NULL)
            return;
        out = work;
        idx = (out - ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = (struct SpriteAttr *)out->unknown_10;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        /* FAKEMATCH: the volatile width read is scheduled before the column constant's pool load, as in the reference. */
        attr->x = (win->x + (column + *(volatile u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 - 1;
        /* FAKEMATCH: retain the existing integer-address halfword stores
           at the canonical member offsets. Member assignments and same-type
           field casts reorder the packed OAM x store and logical x store
           in all six editions at the same native extent. */
        *(s16 *)((u32)out + (u32)&((struct RenderOutput *)0)->x) = attr->x;
        *(s16 *)((u32)out + (u32)&((struct RenderOutput *)0)->y) = attr->y;
        /* FAKEMATCH: retain the existing scalar link-word clear and
           unsigned active-byte read at their canonical member addresses.
           Pointer/signed member access changes the glyph store schedule. */
        *(s32 *)&out->next = 0;
        out->index = idx;
        if ((u8)out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, out);
    } else if (tile <= 0xff) {
        x++;
        y++;
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640)
            ((struct UiRenderWork *)work)->tilemap[pos] = tile | 0xf000;
    }
}

#else

/* Places a glyph: mode 1 queues it as a sprite at the window cell, other
   modes write tiles up to 0xff into the window tilemap. */
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    /* FAKEMATCH: the game keeps the work block in r12 for the tile store and a copy in r8 for the rest; as one plain variable it lives in r8 alone. */
    register u8 *work asm("r12") = gWindowWork[0];
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
        idx = (out - ((struct UiRenderWork *)base)->outputs) * 4;
        out->active = 2;
        attr = (struct SpriteAttr *)out->unknown_10;
        slot = &((struct UiRenderWork *)base)->glyph_resource;
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        left = win->x;
        /* FAKEMATCH: the volatile width read is scheduled before the column constant's pool load, as in the reference. */
        attr->x = (left + (column + *(volatile u16 *)&win->width)) * 8 + 4;
        row = (u8)win->y + (row = (u8)win->height + 254);
        attr->y = row * 8 + 1;
        /* FAKEMATCH: retain the existing integer-address halfword stores
           at the canonical member offsets. Member assignments and same-type
           field casts reorder the packed OAM x store and logical x store
           in all six editions at the same native extent. */
        *(s16 *)((u32)out + (u32)&((struct RenderOutput *)0)->x) = attr->x;
        *(s16 *)((u32)out + (u32)&((struct RenderOutput *)0)->y) = attr->y;
        /* FAKEMATCH: retain the existing scalar link-word clear and
           unsigned active-byte read at their canonical member addresses.
           Pointer/signed member access changes the glyph store schedule. */
        *(s32 *)&out->next = 0;
        out->index = idx;
        if ((u8)out->active == 0)
            out->active = mode;
        RenderOutput_AppendToList(&win->output, out);
    } else if (tile <= 0xff) {
        /* The voicing marks 0xde and 0xdf go into the cell before them,
           joined to the kana tile 0x0e or 0x11 already there. */
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
