#include "TBS_EDITION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"

extern u8 *gWindowWork;
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

extern u8 Data_03001e8c[];
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))
s32 UiWindow_MapTextCanvasTiles(s32, s32, s32, s32, s32);
void UiWindow_DrawFrame(s32, s32, s32, s32);
typedef void (*UiFillFn)(s32 dst, s32 size, s32 value);

/* One of the eight window records at gWindowWork + 0x500. */
struct UiWindow {
    s32 state;
    struct UiWindow *self;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown_10;
    u16 unknown_12;
    u16 unknown_14;
    u16 flags;
    u16 unknown_18;
    s16 timer;
    u8 unknown_1c[8];
};

void WaitFrames(s32 frames);
void UiWork_ResetCounters();

struct UiWindowWork {
    s32 unknown00;
    s32 unknown04;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown10;
    u16 unknown12;
    u16 state;
    u16 flags;
    u16 frame;
    s16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

void RenderOutput_PrepareForRedraw(void *work);
void RenderOutput_RedrawSavedRect(void *work);
void RenderOutput_ClearList(void *work);
void RenderOutput_Release(void *node);
void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(void *arg0);
void UiWork_WaitUntilField1aClear(void *work);

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = gWindowWork;
    u16 *cursor = (u16 *)((y * 32 + x) * 2 + (u32)base);
    s32 tile;
    u32 bottom;
    u32 row;

    tile = 240;
    bottom = y + height;
    tile <<= 8;
    if (bottom > 20)
        height = 20 - y;
    if (width <= 1)
        width = 2;
    if (width > 30)
        width = 30;
    if (height <= 1)
        height = 2;
    if (height > 30)
        height = 30;

    UiWindow_ClearTileAttributesInRect(x, y, width, height);

    row = 0;
    if (row < height) {
        do {
            u32 column;

            if (base[RENDER_MENU_STATE_OFS] != 0) {
                if ((u32)(y + row) > 16)
                    tile = 0xF07F;
                else
                    tile = 0xF000;
            }
            column = 0;
            if (column < width) {
                do {
                    *cursor++ = tile;
                    column++;
                } while (column < width);
            }
            row++;
            cursor += 32 - width;
        } while (row < height);
    }
    base[RENDER_DIRTY_OFS] = 1;
}

void UiWork_DrawByAttributes(void *arg0)
{
    u32 attr;
    u32 tmp;
    u32 v0;
    u32 v1;
    u32 v2;
    u32 v3;
    s32 dst;
    UiFillFn fill;
    void *work;

    /* 描画属性に従い転送方法を切り替える。 */
    work = *(void **)((u32)&Data_03001e8c);
    tmp = FIELD_AT_OFFSET(arg0, u16 *, 0xA);
    attr = FIELD_AT_OFFSET(arg0, u16 *, 0x16);
    v3 = tmp;
    tmp = 0;
    FIELD_AT_OFFSET(arg0, s16 *, 0x1A) = tmp;
    v0 = FIELD_AT_OFFSET(arg0, u16 *, 0xC);
    v1 = FIELD_AT_OFFSET(arg0, u16 *, 0xE);
    v2 = FIELD_AT_OFFSET(arg0, u16 *, 8);
    if (8 & attr) {
        if (0x20 & attr) {
            UiWindow_DrawFrame(v0, v1, v2, v3);
            fill = (UiFillFn)Iwram_FillWords;
            dst = 0x06002500;
            fill(dst, 0xF00, 0x44444444);
        } else {
            fill = (UiFillFn)Iwram_FillWords;
            dst = 0x06002500;
            fill(dst, 0xF00, 0);
        }
        UiWindow_MapTextCanvasTiles(v0, v1, v2, v3, 0);
    } else {
        UiWindow_DrawFrame(v0, v1, v2, v3);
    }
    FIELD_AT_OFFSET(work, s8 *, RENDER_DIRTY_OFS) = 1;
}

/* Open a window at a tile position and size in the first free record, with
   the attribute bits that choose its frame and drawing; a window drawn
   through its attributes appears at once, any other one opens over eight
   frames. Returns the record, or 0 when every record is in use. */
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = (struct UiWindow *)(gWindowWork + 0x500);
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->timer != 0) {
        i++;
        slot++;
        if (i == WINDOW_COUNT) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->y = y;
        found->width = width;
        found->height = height;
        found->x = x;
        found->state = 0;
        found->unknown_14 = 0;
        found->self = slot;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters(x); /* the reset ignores the x it is passed */
        if (attrs & 8) {
            found->flags |= 8;
        }
        if (attrs & 32) {
            found->flags |= 32;
        }
        if (attrs & 64) {
            found->flags |= 64;
        }
        if (attrs & 128) {
            found->flags |= 128;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->flags |= 2;
            found->unknown_18 = 0;
            found->timer = 1;
            UiWork_DrawByAttributes(found);
        } else {
            found->timer = 8;
            found->unknown_18 = 7;
            UiWork_WaitUntilField1aClear(found);
            WaitFrames(1);
        }
    }
    return found;
}

void UiWork_WaitUntilField1aClear(void *work)
{
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & ((struct UiWindowWork *)work)->flags) && (((struct UiWindowWork *)work)->duration != 0)) {
        do {
            WaitFrames(1);
        } while (((struct UiWindowWork *)work)->duration != 0);
    }
}

void UiWork_Finalize(struct UiWindowWork *work, s32 release)
{
    u16 zero;

    if (work == 0)
        return;

    RenderOutput_PrepareForRedraw(work);
    work->previous_x = work->x;
    work->previous_y = work->y;
    work->previous_width = work->width;
    zero = 0;
    work->flags = zero;
    work->previous_height = work->height;

    if (release != 0) {
        UiWindow_EraseBorderRect(work->x, work->y, work->width, work->height);
        work->unknown00 = zero;
        work->unknown04 = zero;
        work->width = zero;
        work->height = zero;
        work->x = zero;
        work->y = zero;
        work->unknown10 = zero;
        work->unknown12 = zero;
        work->state = zero;
        work->flags = zero;
        work->frame = zero;
        work->duration = zero;
        work->previous_x = zero;
        work->previous_y = zero;
        work->previous_width = zero;
        work->previous_height = zero;
    } else {
        work->frame = release;
        work->duration = 4;
    }
}

void RenderOutput_PrepareForRedraw(void *arg0)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & ((struct UiWindowWork *)arg0)->flags)) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}

void RenderOutput_RedrawSavedRect(void *arg0)
{
    /* 保存済みの矩形を再描画する。 */
    struct UiWindowWork *work = arg0;
    UiWindow_DrawFrame(work->x, work->y, work->width, work->height);
}

void RenderOutput_ClearList(void *arg0)
{
    void *next;
    void *node;

    next = NULL;
    /* 単方向リストを先頭から解放する。 */
    if (arg0 != NULL) {
        node = *(void **)arg0;
        *(void **)((u8 *)arg0 + 4) = arg0;
        *(void **)arg0 = next;
        while (node != NULL) {
            next = *(void **)node;
            RenderOutput_Release(node);
            node = next;
        }
    }
}
