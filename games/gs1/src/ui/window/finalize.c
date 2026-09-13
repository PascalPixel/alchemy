#include "types.h"

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
s32 WaitFrames(s32 frames);
void Func_080170f8(s32 x, s32 y, s32 width, s32 height);
void Func_08016178(u16 x, u16 y, u16 width, u16 height);

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
        Func_08016178(work->x, work->y, work->width, work->height);
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
    Func_080170f8(work->x, work->y, work->width, work->height);
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
