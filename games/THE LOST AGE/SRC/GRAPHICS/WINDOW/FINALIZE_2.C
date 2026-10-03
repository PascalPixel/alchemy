#include "UIFLOW.H"

void RenderOutput_PrepareForRedraw(struct UiWindow *work);
void RenderOutput_RedrawSavedRect(void *work);
void RenderOutput_ClearList(void *work);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);

void UiWork_Finalize(struct UiWindow *work, s32 release)
{
    u16 zero;

    if (work == 0)
        return;

    RenderOutput_PrepareForRedraw(work);
    zero = 0;
    work->flags = zero;
    work->previous_x = work->x;
    work->previous_y = work->y;
    work->previous_width = work->width;
    work->previous_height = work->height;

    if (release != 0) {
        UiWindow_EraseBorderRect((s16)work->x, (s16)work->y, work->width, work->height);
        work->output.head = NULL;
        work->output.tail_link = NULL;
        work->width = zero;
        work->height = zero;
        work->x = zero;
        work->y = zero;
        work->unknown_10 = zero;
        work->unknown_12 = zero;
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

void RenderOutput_PrepareForRedraw(struct UiWindow *work)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & work->flags)) {
        RenderOutput_RedrawSavedRect(work);
        RenderOutput_ClearList(work);
    }
}
