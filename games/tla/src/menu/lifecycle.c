#define OWN_UiWindow_CreateFar
//--------------------------------------------------------------------------
// lifecycle.c
// ウィンドウ：ハンドルで持つウィンドウを開く・閉じる
//--------------------------------------------------------------------------

#include "uiwindow.h"

void             UiWork_FinalizeFar();
struct UiWindow *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void             RenderOutput_RedrawSavedRectFar(struct UiWindow *window);

//--------------------------------------------------------------------------
// *handle のウィンドウがなければ作って 1 を返す
// すでにあれば 0 を返し、flags に 0x100 がなければ保存した矩形で描きなおす
// flags : 下位 8 ビットがウィンドウの種類
//--------------------------------------------------------------------------
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags)
{
    s32 cur;
    s32 style;

    cur = *handle;
    if (cur != 0)
    {
        if (0x100 & flags)
        {
            return 0;
        }
        RenderOutput_RedrawSavedRectFar((struct UiWindow *)cur);
        return 0;
    }
    style = flags & 0xff;
    *handle = (s32)UiWindow_CreateFar(x, y, w, h, style);
    return 1;
}

//--------------------------------------------------------------------------
// *handle のウィンドウがあれば閉じて、ハンドルを NULL にする
// 引数 1 つの宣言だが、呼び出し元の 2 つめの引数が r1 のまま
// UiWork_FinalizeFar に渡る
//--------------------------------------------------------------------------
void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL)
    {
        UiWork_FinalizeFar(*handle);
        *handle = NULL;
    }
}
