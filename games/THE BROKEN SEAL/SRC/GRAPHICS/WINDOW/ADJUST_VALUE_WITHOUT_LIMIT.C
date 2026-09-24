#include "TYPES.H"
#include "SCENE.H"

/* 上限を指定しない呼出し。 */
void Ui_AdjustValueWithoutLimit(s32 no, s32 value)
{
    UiWindow_AdjustValue(no, value, 0x7fffffff);
}
