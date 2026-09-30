#include "TYPES.H"

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

void RenderOutput_PrepareForRedraw(void *arg0)
{
    /* 属性0x8がない時だけ描画と子リストを解放する。 */
    if (!(8 & ((struct UiWindowWork *)arg0)->flags)) {
        RenderOutput_RedrawSavedRect(arg0);
        RenderOutput_ClearList(arg0);
    }
}
