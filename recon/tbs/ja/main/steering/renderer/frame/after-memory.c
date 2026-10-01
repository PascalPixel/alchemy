/* 2026-10-01 Japanese glyph frame probe: after-memory.
 * Compiled extent 332 bytes against the complete 328-byte Japanese extent;
 * 293 differing bytes, first at +0xa. Approved TBS flags and
 * current typed source imports; no compiler output patch. The nearest current
 * draft still differs in the adjacent frame/height instruction order. */
#include "TYPES.H"
#include "TBS_EDITION.H"

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

static __inline__ u8 *CopyWork(u8 *base, u32 bound)
{
    u8 *copy;
    /* FAKEMATCH: retain the work copy after the height bound is calculated. */
    asm ("mov %0, %1" : "=r" (copy) : "r" (base), "r" (bound));
    return copy;
}

/* Places a glyph: mode 1 queues it as a sprite at the window cell, other
   modes write tiles up to 0xff into the window tilemap. */
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    /* FAKEMATCH: this measured put helper height check once draft preserves the source
       experiment and its remaining instruction differences stated above. */
    struct RenderOutput *out = (struct RenderOutput *)gWindowWork;
    u8 *base = (u8 *)out;
    u8 *read_base;
    s32 idx;
    u16 *slot;
    struct SpriteAttr *attr;
    u32 pos;
    u16 row;

    /* FAKEMATCH: preserve the work copy after the height bound is calculated. */
    read_base = CopyWork(base, win->height - 2);
    /* FAKEMATCH: frame scheduling experiment; retained only if measured. */
    asm volatile ("" : : : "memory");
    do {
    if (y > (u32)(win->height - 2))
        return;
    } while (0);
    if (x > (u32)(win->width - 2))
        return;
    if (mode == 1) {
        s32 column;
        u16 tmp;
        out = RenderOutput_AcquireFree();
        if (out == NULL)
            return;
        idx = (out - (struct RenderOutput *)(read_base + RENDER_OUTPUT_TBL_OFS)) * 4;
        out->one5 = 2;
        attr = &out->attr;
        slot = (u16 *)(read_base + RENDER_COUNTER_OFS);
        if (*slot == 99)
            *slot = Resource_FindFreeEntry();
        column = 0xfffe;
        /* FAKEMATCH: the volatile width read is scheduled before the column constant's pool load, as in the reference. */
        tmp = win->x;
        attr->x = (tmp + (column + *(volatile u16 *)&win->width)) * 8 + 4;
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
        /* The Japanese voicing marks 0xde and 0xdf go into the cell before
           them, joined to the kana tile 0x0e or 0x11 already there. */
        if (tile - 0xde <= 1) {
            u32 tmp2;
            tmp2 = (win->y + y) * 32;
            switch (*(((struct WindowTilemap *)read_base)->tiles + (tmp2 + (win->x + x)))) {
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
            ((struct WindowTilemap *)base)->tiles[pos] = tile | 0xf000;
    }
}
