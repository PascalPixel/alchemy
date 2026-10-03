#include "UIWINDOW.H"

void RenderOutput_PrepareForRedraw(void *work);
void RenderOutput_RedrawSavedRect(void *work);
void RenderOutput_ClearList(void *work);

void RenderOutput_PrepareForRedraw(void *arg0)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & ((struct UiWindow *)arg0)->flags)) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}
