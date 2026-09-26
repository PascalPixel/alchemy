/* Draft: complete 260-byte owner, 28 differing halfwords. First 45
 * instructions exact after correcting the resource callee and byte row
 * arithmetic. Remaining: X sum and masks, index-store scheduling.
 * Halfword columns and word/bitfield unions regressed; no allocator sweep.
 */
#include "TYPES.H"

extern u8 *Data_03001e8c;
extern const u8 Value_0000fffe;

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

void Func_08018efc(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode)
{
    struct RenderOutput *out = (struct RenderOutput *)Data_03001e8c;
    u8 *base = (u8 *)out;
    struct SpriteAttr *attr;
    s32 idx;
    u16 *slot;
    u32 pos;
    u16 row;

    if (y > (u32)(win->height - 2)) return;
    if (x > (u32)(win->width - 2)) return;
    if (mode == 1) {
        out = RenderOutput_AcquireFree();
        if (out == NULL) return;
        idx = (out - (struct RenderOutput *)(base + 0x698)) * 4;
        out->one5 = 2;
        slot = (u16 *)(base + 0x12b6);
        attr = &out->attr;
        if (*slot == 99) *slot = Resource_FindFreeEntry();
        attr->x = (win->width + (s32)&Value_0000fffe + win->x) * 8 + 4;
        row = (u8)win->height + 254;
        row += (u8)win->y;
        attr->y = row * 8 - 1;
        out->x = attr->x;
        out->y = attr->y;
        out->zero = 0;
        out->index = idx;
        if (out->one5 == 0) out->one5 = mode;
        RenderOutput_AppendToList(win, (s8 *)out);
    } else if (tile <= 0xff) {
        x++;
        y++;
        pos = (win->y + y) * 32 + (win->x + x);
        if (pos < 640) ((u16 *)out)[pos] = tile | 0xf000;
    }
}
