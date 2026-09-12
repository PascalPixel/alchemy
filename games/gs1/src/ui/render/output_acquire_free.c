#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "gs1_edition.h"
#include "runtime_interfaces.h"

/* ui/render/output_list/acquire_free.c */
void *RenderOutput_AcquireFree(void)
{
  void **p;
  void *state;
  /* Detach and return the head of the free list. */
  state = *((void **)ADDR_03001E8C);
  p = *((void ***)(((u8 *)state) + RENDER_FREE_HEAD_OFS));
  if (p != ((void *) 0))
  {
    if ((*p) == ((void *) 0))
    {
      *((s32 *)(((u8 *)state) + RENDER_FREE_TAIL_OFS)) = (s32)(state + RENDER_FREE_HEAD_OFS);
    }
    *((void ***)(((u8 *)state) + RENDER_FREE_HEAD_OFS)) = *p;
    *p = (void *) 0;
  }
  return p;
}

/* ui/render/output_list/release_free.c */
extern struct State_080173ac *gIw;

void RenderOutput_ReleaseFree(u32 arg0)
{
    u8 *base = (u8 *)gIw;
    /* 管理領域内の要素だけを空きリストへ戻す。 */
    if (arg0 >= (u32)(base + 0x698) && arg0 < (u32)(base + 0xd98)) {
        u32 old = *(u32 *)(base + 0xd9c);
        *(u32 *)(base + 0xd9c) = arg0;
        *(u32 *)old = arg0;
        *(u32 *)arg0 = 0;
    }
}

/* ui/runtime/init/init_free_list.c */

void UiWork_InitFreeList(void)
{
    s32 count;
    u8 *base;
    u8 *item;
    u8 *next;

    base = (u8 *)gIw;
    /* 0x1cバイト単位の空きリストを初期化する。 */
    item = base + 0x698;
    *(u8 **)(base + 0xd98) = item;
    count = 0x3e;
    do {
        next = item + 0x1c;
        count--;
        *(u8 **)item = next;
        item = next;
    } while (count >= 0);
    *(s32 *)next = 0;
    *(u8 **)(base + 0xd9c) = next;
}

/* ui/window/erase_border_rect.c */

void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);

void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height)
{
    u8 *base = (u8 *)gIw;
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

/* ui/runtime/misc/draw_work_by_attributes.c */
typedef void (*UiFillFn)(s32 dst, s32 size, s32 value);

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
    work = *(void **)ADDR_03001E8C;
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
            FunctionHead_080170f8(v0, v1, v2, v3);
            fill = (UiFillFn)0x03000168;
            dst = 0x06002500;
            fill(dst, 0xF00, 0x44444444);
        } else {
            fill = (UiFillFn)0x03000168;
            dst = 0x06002500;
            fill(dst, 0xF00, 0);
        }
        Ui_SetRange(v0, v1, v2, v3, 0);
    } else {
        FunctionHead_080170f8(v0, v1, v2, v3);
    }
    FIELD_AT_OFFSET(work, s8 *, RENDER_DIRTY_OFS) = 1;
}

/* ui/runtime/misc/wait_until_field1a_clear.c */
void WaitFrames(s32);

void UiWork_WaitUntilField1aClear(void *work)
{
    /* 値が0になるまで更新処理を進める。 */
    if (!(2 & FIELD_AT_OFFSET(work, u16 *, 0x16)) && (FIELD_AT_OFFSET(work, s16 *, 0x1A) != 0)) {
        do {
            WaitFrames(1);
        } while (FIELD_AT_OFFSET(work, s16 *, 0x1A) != 0);
    }
}

/* ui/render/finalize.c */
struct Work {
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
    u16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

void RenderOutput_PrepareForRedraw(void *work);

void UiWork_Finalize(struct Work *work, s32 release)
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
        FunctionHead_08016178(work->x, work->y, work->width, work->height);
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

/* ui/render/output_list/prepare_for_redraw.c */
void RenderOutput_RedrawSavedRect(void *);
void RenderOutput_ClearList(void *);

void RenderOutput_PrepareForRedraw(void *arg0)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & FIELD_AT_OFFSET(arg0, u16 *, 0x16))) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}

/* ui/render/output_list/redraw_saved_rect.c */
void RenderOutput_RedrawSavedRect(void *arg0)
{
    /* 保存済みの矩形を再描画する。 */
    FunctionHead_0801de5c(FIELD_AT_OFFSET(arg0, u16 *, 0xC), FIELD_AT_OFFSET(arg0, u16 *, 0xE), FIELD_AT_OFFSET(arg0, u16 *, 8), FIELD_AT_OFFSET(arg0, u16 *, 0xA));
}

/* ui/render/output_list/clear.c */
void RenderOutput_Release();

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

/* ui/runtime/misc/update_list_tail.c */
void UiWork_UpdateListTail(s32 *list)
{
    s32 *prev;
    s32 *node;

    /* 末尾要素をキャッシュする。 */
    node = FIELD_AT_OFFSET(list, s32 **, 0);
    prev = list;
    if (node != NULL) {
        do {
            prev = node;
            node = *prev;
        } while (node != NULL);
    }
    FIELD_AT_OFFSET(list, s32 **, 4) = prev;
}

/* ui/render/output_list/append.c */
void RenderOutput_AppendToList(void *arg0, s8 *arg1)
{
    /* 新しい末尾をリストとキャッシュの両方へ接続する。 */
    if (arg0 != NULL) {
        *(volatile s32 *)FIELD_AT_OFFSET(arg0, s32 **, 4) = (s32)arg1;
        *(s32 *volatile *)((u8 *)arg0 + 4) = (s32 *)arg1;
    }
}

/* ui/render/output_list/release.c */
void RenderOutput_ReleaseFree(u32 arg0);
s32 Resource_ResetEntry(u32 index);

struct Entry_08016594 {
    u8 unknown_00[4];
    u8 kind;
    s8 active;
    u8 unknown_06[8];
    u8 index;
    u8 unknown_0f[0x0a];
    u8 palette;
};

void RenderOutput_Release(struct Entry_08016594 *entry)
{
    RenderOutput_ReleaseFree((u32)entry);
    if (entry->kind != 0) {
        Resource_ResetEntry(entry->index);
        if (entry->kind == 2) {
            u8 *dst = (u8 *)(*(s32 *)ADDR_03001E8C);
            s32 idx = ((u32)entry->palette >> 4) * 2 + 0x12D0;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    entry->active = 0;
}

/* ui/render/activate_channel.c */
struct ChannelWork {
    u8 padding00[0x14];
    u16 state;
};

struct Slot {
    struct ChannelWork *work;
    u16 field04;
    u16 field06;
    u16 values[4];
    u16 field10;
    u16 field12;
    u16 field14;
    u16 field16;
    u16 field18;
    u16 field1a;
    u16 field1c;
    u16 field1e;
    u16 field20;
    u16 field22;
    u16 field24;
    u16 field26;
};

void UiWork_ResetChannelTransition(void *);

struct Slot *UiWork_ActivateChannel(struct ChannelWork *work, s32 value, s32 preserve)
{
    struct Slot *slot;
    struct Slot *selected;
    u16 *destination;
    u16 val04;
    u16 zero;
    u32 index;

    slot = (struct Slot *)((u8 *)gIw + RENDER_CHANNEL_OFS);
    selected = 0;
    for (index = 0; index != 3; slot++, index++) {
        if (slot->work == 0 || slot->work->state != 0) {
            selected = slot;
            break;
        }
    }

    if (selected != 0) {
        if (selected->work == 0) {
            selected->field06 = 0xA00;
            val04 = 0x300;
            selected->work = work;
            goto reset_field04;
        }
        if (preserve == 0) {
            if (selected->field06 == 0) {
                selected->field06 = 0xA00;
            } else if ((u32)selected->field06 < 0xD00) {
                selected->field06 += 0xD00;
            } else {
                UiWork_ResetChannelTransition(selected);
            }
            val04 = 0x300;
reset_field04:
            selected->field04 = val04;
        }

        zero = 0;
        selected->field1e = 0x300;
        selected->work->state = zero;
        selected->field16 = 15;
        selected->field1a = 10;
        selected->field12 = value;
        selected->field14 = zero;
        selected->field18 = zero;
        selected->field10 = zero;
        selected->field20 = zero;
        index = 0;
        destination = selected->values;
        do {
            index++;
            *destination++ = zero;
        } while (index <= 3);
    }
    return selected;
}

/* ui/render/clear_vram_block.c */
typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

s32 Ui_ClearVramBlock(void)
{
    FillWordsFn fill = (FillWordsFn)0x03000168;

    return fill((void *)0x06002500, 0xF00, 0);
}

/* ui/render/fill_vram_block_pattern.c */
s32 Ui_FillVramBlockPattern(void)
{
    FillWordsFn fill = (FillWordsFn)0x03000168;

    return fill((void *)0x06002500, 0xF00, 0x44444444);
}

/* ui/runtime/init/reset_free_channel.c */
struct EntryState {
    u8 padding00[0x14];
    u16 state;
};

struct EntrySlot {
    struct EntryState *entry;
    s16 field04;
    s16 field06;
    u8 padding08[0x0C];
    s16 field14;
    s16 field16;
    s16 field18;
    s16 field1a;
    u8 padding1c[0x0C];
};


void UiWork_ResetFreeChannel(void)
{
    struct EntrySlot *slot =
        (struct EntrySlot *)((u8 *)gIw + RENDER_CHANNEL_OFS);
    struct EntrySlot *sel = 0;
    s32 i;

    for (i = 0; i != 3; slot++, i++) {
        if (slot->entry == 0 || slot->entry->state != 0) {
            sel = slot;
            break;
        }
    }
    if (sel != 0) {
        if (sel->entry != 0) {
            FunctionHead_0801671c();
            sel->field06 = 0;
        }
        sel->field04 = 0;
        sel->field14 = 0;
        sel->field16 = 0xF;
        sel->field18 = 0;
        sel->field1a = 0xA;
    }
}

/* ui/runtime/misc/copy_params_to_render_work.c */
void UiWork_CopyParamsToRenderWork(void *work)
{
    u32 p2;
    s32 p1;
    s32 z1;
    u32 p0;
    u32 z0;
    void *render;

    render = *(void **)ADDR_03001E8C;
    p0 = (s32)(FIELD_AT_OFFSET(work, u16 *, 0x16)); FIELD_AT_OFFSET(render, u16 *, RENDER_PARAM_OFS) = (u16)p0;
    p1 = (s32)(FIELD_AT_OFFSET(work, u16 *, 0x18)); (s32)z0 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD_OFS) = (u16)p1;
    p2 = (FIELD_AT_OFFSET(work, u16 *, 0x1A)); (s32)z1 = 0; FIELD_AT_OFFSET(render, u16 *, RENDER_WORD2_OFS) = (u16)p2;
}

/* ui/render/reset_channel_transition.c */
void UiWork_ResetChannelTransition(void *work)
{
  int mode;
  int val;
  u8 *p;
  p = ((u8 *)work) + 0x1C;
  val = 2;
  mode = val;
  *((s16 *)p) = mode;
}

/* ui/runtime/proc/process_render_channels.c */
struct Work;

void UiWork_Finalize(struct Work *work, s32 release);

void UiWork_ProcessRenderChannels(void)
{
    u8 *channel = *(u8 **)ADDR_03001E8C + RENDER_CHANNEL_OFS;
    s32 channel_no = 0;
    s32 one = 1;

    do {
        u8 *current = *(u8 **)channel;

        if (current != 0 && *(s32 *)(current + 0x18) == 0) {
            u16 flags = *(u16 *)(current + 0x16);

            if (flags == 0) {
                *(u8 **)channel = (u8 *)0;
            } else {
                s32 pending = *(u16 *)(current + 0x12);
                s32 kind;

                if (pending != 0) {
                    FunctionHead_08019854(channel);
                } else {
                    kind = FunctionHead_080168f4(channel);
                    switch (kind) {
                    case 8:
                        *(u16 *)(*(u8 **)channel + 0x14) = one;
                        break;
                    case 9:
                    {
                        u8 *entity = *(u8 **)channel;

                        UiWork_Finalize(
                            (struct Work *)entity,
                            (u16)(*(u16 *)(entity + 0x16) & 2)
                        );
                        *(u16 *)(channel + 0x04) = pending;
                        *(u16 *)(channel + 0x06) = pending;
                        *(u16 *)(channel + 0x12) = pending;
                        *(u16 *)(channel + 0x14) = pending;
                        *(u16 *)(channel + 0x16) = pending;
                        *(u16 *)(channel + 0x18) = pending;
                        *(u16 *)(channel + 0x1a) = pending;
                        *(u16 *)(*(u8 **)channel + 0x14) = one;
                        break;
                    }
                    }
                }
            }
        }
        channel_no++;
        channel += 0x28;
    } while (channel_no != 3);
}

/* ui/runtime/proc/process_direct_work.c */
void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);

void UiWork_ProcessDirectWork(void)
{
    u8 *base = (u8 *)gIw;
    struct Work *work = (struct Work *)(base + 0x500);
    s32 index = 0;
    u8 dirty;

loop:
    if (work->flags != 0) {
        if (work->frame != 0) {
            UiWindow_UpdateInterpolatedGeometry(work, 0);
            work->frame--;
        } else if (work->duration != 0) {
            FunctionHead_08016230(work);
        }
    } else if (work->duration != 0) {
        if (work->frame != work->duration) {
            UiWindow_EraseBorderRect(work->previous_x, work->previous_y,
                                     work->previous_width,
                                     work->previous_height);
            UiWindow_UpdateInterpolatedGeometry(work, 1);
            work->frame++;
            dirty = 1;
            base[RENDER_DIRTY_OFS] = dirty;
        } else {
            UiWindow_EraseBorderRect(work->previous_x, work->previous_y,
                                     work->previous_width,
                                     work->previous_height);
            work->unknown00 = 0;
            work->unknown04 = 0;
            work->width = 0;
            work->height = 0;
            work->x = 0;
            work->y = 0;
            work->unknown10 = 0;
            work->unknown12 = 0;
            work->state = 0;
            work->flags = 0;
            work->frame = 0;
            work->duration = 0;
            work->previous_x = 0;
            work->previous_y = 0;
            work->previous_width = 0;
            work->previous_height = 0;
            base[RENDER_DIRTY_OFS] = 1;
        }
    }
    index++;
    work++;
    if (index != 8)
        goto loop;
}

/* ui/window/update_interpolated_geometry.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define WINDOW_WIDTH(window) FIELD(window, u16, 0x08)
#define WINDOW_HEIGHT(window) FIELD(window, u16, 0x0a)
#define WINDOW_X(window) FIELD(window, u16, 0x0c)
#define WINDOW_Y(window) FIELD(window, u16, 0x0e)
#define WINDOW_FRAME(window) FIELD(window, s16, 0x18)
#define WINDOW_DURATION(window) FIELD(window, s16, 0x1a)
#define WINDOW_PREVIOUS_X(window) FIELD(window, u16, 0x1c)
#define WINDOW_PREVIOUS_Y(window) FIELD(window, u16, 0x1e)
#define WINDOW_PREVIOUS_WIDTH(window) FIELD(window, u16, 0x20)
#define WINDOW_PREVIOUS_HEIGHT(window) FIELD(window, u16, 0x22)

struct UiWindowInterpolationScratch {
    s32 scaled_part;
    s32 scaled_duration;
    s32 result;
};

typedef s32 (*UiWindowScaleFunction)(s32, s32);

void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position)
{
    struct UiWindowInterpolationScratch scratch;
    s32 frame;
    s32 duration;
    s32 remaining;
    s32 x;
    s32 y;
    s32 width;
    s32 height;

    frame = WINDOW_FRAME(window);
    duration = WINDOW_DURATION(window);
    remaining = duration - frame;
    scratch.scaled_part =
        (s32)((u32)(frame *WINDOW_WIDTH(window)) << 16);
    scratch.scaled_duration = (s32)((u32)duration << 17);
    scratch.result =
        ((UiWindowScaleFunction)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    x = (scratch.result >> 16) + WINDOW_X(window);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_WIDTH(window)) << 16);
    scratch.result =
        ((UiWindowScaleFunction)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    width = scratch.result >> 15;

    scratch.scaled_part =
        (s32)((u32)(frame *WINDOW_HEIGHT(window)) << 16);
    scratch.scaled_duration = (s32)((u32)WINDOW_DURATION(window) << 17);
    scratch.result =
        ((UiWindowScaleFunction)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    y = (scratch.result >> 16) + WINDOW_Y(window);

    scratch.scaled_part =
        (s32)(((u32)remaining *WINDOW_HEIGHT(window)) << 16);
    scratch.result =
        ((UiWindowScaleFunction)0x0300013c)(
            scratch.scaled_duration, scratch.scaled_part);
    height = scratch.result >> 15;

    FunctionHead_08018a50(x, y, width, height);
    if (save_position != 0) {
        WINDOW_PREVIOUS_X(window) = x;
        WINDOW_PREVIOUS_Y(window) = y;
        WINDOW_PREVIOUS_WIDTH(window) = width;
        WINDOW_PREVIOUS_HEIGHT(window) = height;
    }
}

/* ui/render/is_complete.c */
struct WorkSlot {
    struct ChannelWork *work;
    u8 padding04[0x24];
};

s32 UiWork_IsComplete(void)
{
    s32 result;
    s32 channel_index;
    struct WorkSlot *channel;
    struct ChannelWork *work;

    channel = (struct WorkSlot *)(*(u8 **)ADDR_03001E8C + 0x620);
    channel_index = 0;
next_channel:
    work = channel->work;
    if ((work == NULL) || (result = 0, (work->state != 0))) {
        channel_index += 1;
        channel++;
        if (channel_index == 3) {
            result = 1;
        } else {
            goto next_channel;
        }
    }
    return result;
}

/* ui/runtime/misc/is_idle.c */
s32 UiWork_IsIdle(void *arg0)
{
    if (*(u16 *)((u8 *)arg0 + 0x16) == 0) {
        if (*(s16 *)((u8 *)arg0 + 0x1A) == 0) {
            return 1;
        }
    }
    return 0;
}

/* ui/runtime/init/reset_counters.c */
struct State_080173ac {
    u8 filler0[0xea8];
    u16 ten;
    u16 one;
    u16 zero;
    u16 fifteen;
    u8 fillerEb0[0x400];
    u16 nine;
};


void UiWork_ResetCounters(void)
{
    struct State_080173ac *state = gIw;

    state->fifteen = 15;
    state->ten = 10;
    state->nine = 9;
    state->zero = 0;
    state->one = 1;
}

/* ui/runtime/init/init_counters_with_resource_and_schedule_refresh.c */
struct State_080173f4 {
    u8 filler0[0xEA8];
    u16 ten;
    u16 unusedEaa;
    u16 zero;
    u16 fifteen;
    u8 fillerEb0[0x400];
    u16 nine;
    u16 secondZero;
    u8 filler12b4[4];
    u16 result;
};


s32 Resource_CopyData(s32, s32, s32);
void ScheduleCallbackAfterFrames(void *, s32);

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct State_080173f4 *state = gIw;
    s32 size;

    state->result = Resource_CopyData(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->secondZero = 0;
    size = 200;
    size <<= 4;
    ScheduleCallbackAfterFrames((void *)0x0801789D, size);
}

/* ui/runtime/init/init_counters_and_schedule_refresh.c */
struct State_08017464 {
#if defined(GS1_EDITION_JA)
    u8 filler0[0xF38];
#else
    u8 filler0[0xEA8];
#endif
    u16 ten;
    u16 unusedEaa;
    u16 zero;
    u16 fifteen;
#if defined(GS1_EDITION_JA)
    u8 fillerEb0[0x200];
#else
    u8 fillerEb0[0x400];
#endif
    u16 nine;
    u16 secondZero;
    u8 filler12b4[4];
    u16 result;
};

extern u8 gRom;


void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct State_08017464 *state = gIw;
    s32 size;

    if (initialize != 0)
        state->result = Resource_CopyData(95, 128 << 6, 0);

    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->secondZero = 0;
    size = 200;
    size <<= 4;
    ScheduleCallbackAfterFrames(&gRom, size);
}

/* ui/runtime/misc/finalize_shared_slot.c */

void UiWork_FinalizeSharedSlot(void)
{
    struct Work **slot;
    struct Work *work;

    slot = *(struct Work ***)ADDR_03001EE4;
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}

/* ui/text/misc/prepare_message_work.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Slot;

struct UiTextMessageWorkGlobals {
    void *state;
    u8 padding4[0x54];
    void *control;
};

extern s32 UiText_BuildRenderEntries(s32, s32);
struct Work *UiWindow_Create(s32, s32, s32, s32, s32);

struct Slot *UiWork_ActivateChannel(struct ChannelWork *, s32, s32);

void UiText_PrepareMessageWork(s32 argument)
{
    s32 index;
    s32 result;
    s32 one;
    s32 active_offset;
    struct Work *existing;
    struct Work *work;
    void *state;
    void *control;
    volatile struct UiTextMessageWorkGlobals *text_iw =
        (volatile struct UiTextMessageWorkGlobals *)ADDR_03001E8C;

    state = text_iw->state;
    control = text_iw->control;
    result = 0;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = 2;
    index = UiText_BuildRenderEntries(argument, 1);
    one = 1;
    FIELD(state, s8, RENDER_MENU_STATE_OFS) = one;
    active_offset = RENDER_ENTRY_TBL_OFS + index * 2;

    if (FIELD(state, u16, active_offset) != 0) {
        existing = FIELD(control, struct Work *, 0);
        if (existing != NULL) {
            goto use_existing;
        }
        {
            work = UiWindow_Create(0, 15, 30, 6, 10);
            existing = work;
            FIELD(control, struct Work *, 0) = existing;
            Ui_SetRange(0, 15, 30, 6, one);
            FIELD(control, s32, 8) = result;
            goto have_work;
        }
use_existing:
        work = existing;
have_work:
        if (work != NULL) {
            result = (s32)UiWork_ActivateChannel((struct ChannelWork *)work, index, FIELD(control, s32, 8));
            FIELD(control, s32, 4) = result;
            FIELD(control, s32, 8) = 0;
            if (result == 0) {
                UiWork_Finalize(work, one);
            }
        }
    }
}

/* ui/text/msg/show_message_and_wait.c */
s32 UiWork_IsComplete(void);
void UiText_ShowMessageAndWaitCore(s32 argument)
{
    UiText_PrepareMessageWork(argument);
    goto check;
again:
    WaitFrames(1);
check:
    if (UiWork_IsComplete() == 0) {
        goto again;
    }
    WaitFrames(1);
}

/* ui/runtime/set/set_busy_flags.c */
void UiWork_SetBusyFlags(s32 flags)
{
    void *work;

    work = *(void **)ADDR_03001E8C;
    if (work != NULL) {
        if (flags & 1) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 1) = 1;
        }
        if (2 & flags) {
            FIELD_AT_OFFSET(work, s8 *, RENDER_BUSY_OFS + 2) = 1;
        }
    }
}

/* ui/runtime/proc/process_all.c */
void UiWork_ProcessRenderChannels(void);

void UiWork_ProcessAll(void)
{
    FunctionHead_08016f2c();
    UiWork_ProcessRenderChannels();
    Ui_unk2();
}

/* ui/text/size/get_wide_string_width.c */
s32 UiText_GetWideStringWidth(u16 *text)
{
    s32 width;
    u16 *p;
    u32 c;
    u32 idx;

    c = *text;
    width = 0;
    p = text + 1;
    if (c != 0) {
        do {
            if (c == 0x20) {
                width += 4;
            } else if (c <= 0xFFU) {
                idx = c - 0xDE;
                if (idx > 1U) {
                    width += *(u16 *)((u8 *)((idx + 0xBE) << 5) + 0x08032224);
                }
            } else {
                width += 0xA;
            }
            c = *p;
            p += 1;
        } while (c != 0);
    }
    return width;
}

/* ui/text/misc/render_glyph_tile_at_work_offset.c */
struct Work_08017c1c {
    u8 pad_00[12];
    u16 x;
    u16 y;
};


void UiText_RenderGlyphTileAtWorkOffset(
    u16 *buffer,
    const struct Work_08017c1c *work,
    s32 offset_x,
    s32 offset_y)
{
    u8 *base = (u8 *)gIw;
    s32 index;
    u32 cell;

    if (buffer == NULL) {
        u16 *counter = (u16 *)(base + 0x12B2);

        index = *counter * 2 + 0xEB0;
        buffer = (u16 *)(base + 0xEB0);
        *(u16 *)(base + index) = 0;
        *counter = (*counter + 1) & 0x1FF;
    }

    cell = ((work->y + offset_y + 1) << 5)
        + (work->x + offset_x) + 1;
    if (cell < 0x280) {
        s32 src;
        s32 dst;

        cell *= 2;
        dst = 0x06002000 + cell;
        src = (s32)base + cell;

        FunctionHead_08018a50(
            buffer,
            src,
            dst,
            0x06002000);
    }
}

/* ui/text/fmt/format_number.c */
s32 FixedPoint_Ratio(s32 dividend, s32 divisor);
s32 Modulo(s32 dividend, s32 divisor);

u8 *Text_FormatNumber(u8 *buffer, s32 input, s32 width)
{
    s32 offset;
    s32 value;
    s32 negative;
    s32 space;
    s32 minus;
    u8 *leading;
    u8 *trim;

    value = input;
    negative = 0;
    if (value < 0) {
        if (width == 0)
            negative = 1;
        value = (s32)(0U - (u32)value);
    }

    buffer[0] = ' ';
    for (offset = 12; offset != 0; offset--) {
        buffer[offset] = Modulo(value, 10) + '0';
        value = FixedPoint_Ratio(value, 10);
    }

    offset = 0;
    buffer[13] = offset;
    space = ' ';
    offset = 1;
    minus = '-';
    for (leading = buffer; offset != 13; leading++, offset++) {
        if (leading[1] == '0') {
            if (offset != 12)
                leading[1] = space;
        } else {
            if (negative != 0)
                leading[0] = minus;
            break;
        }
    }

    if (width == 0) {
        offset = 0;
        if (buffer[0] == ' ') {
            trim = buffer;
            do {
                offset++;
                if (offset == 12)
                    break;
                trim++;
            } while (*trim == ' ');
        }
        return buffer + offset;
    }

    if ((u32)width > 12)
        width = 12;
    return buffer + 13 - width;
}

/* ui/window/fit_on_screen.c */

void Ui_Run(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, s32 mode, u32 flags)
{
    u8 *base;
    s32 x;
    s32 y;
    s32 limit;
    s32 right;
    s32 bottom;
    s32 over;
    s32 pos;

    base = (u8 *)gIw;
    x = *px;
    y = *py;
    limit = 30;

    if (!(flags & 2)) {
        if (flags & 1)
            FunctionHead_08018a50(no, (s32 *)pw, (s32 *)ph, mode);
        else
            Ui_unk2_3(no, (s32 *)pw, (s32 *)ph, mode);
    }

    if (*pw == 0 && *ph == 0)
        return;

    if (!(flags & 2)) {
        *pw = (*pw + 19) >> 3;
        *ph = (*ph + 15) >> 3;
        if (base[RENDER_MODE_OFS] != 0) {
            *pw += 2;
            limit = 29;
        }
    }

    right = x + *pw;
    if (right > limit) {
        over = right - limit;
        pos = x - over;
        if (pos >= 0)
            x = pos;
        else
            x = 0;
    }
    bottom = y + *ph;
    if (bottom > 20) {
        over = bottom - 20;
        pos = y - over;
        if (pos >= 0)
            y = pos;
        else
            y = 0;
    }
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (x > limit - *pw)
        x = limit - *pw;
    if (y > 20 - *ph)
        y = 20 - *ph;
    *px = x;
    *py = y;
}

/* ui/text/size/measure_resource_entries.c */

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    FunctionHead_08018850(UiText_BuildRenderEntries(no, 0), x, y, 0);
}

/* ui/text/size/get_resource_dimensions.c */

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 temp;
    s32 offset;

    base = *(u16 **)ADDR_03001E8C;
    temp = UiText_BuildRenderEntries(no, 0);
    offset = temp * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + offset) == 0)
    {
        return 0;
    }
    Ui_SetRect(temp, arg1, arg2, arg3, arg4, 0, 0);
    return 1;
}

/* ui/text/size/get_resource_dimensions_alt.c */

s32 UiText_GetResourceDimensionsAlt(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 idx;
    s32 ofs;

    base = *(u16 **)ADDR_03001E8C;
    idx = UiText_BuildRenderEntries(no, 0);
    ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + ofs) == 0)
    {
        return 0;
    }
    Ui_SetRect(idx, arg1, arg2, arg3, arg4, 0, 1);
    return 1;
}
