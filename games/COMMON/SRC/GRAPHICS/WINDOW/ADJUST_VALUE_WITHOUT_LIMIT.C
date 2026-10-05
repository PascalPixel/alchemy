#include "TYPES.H"
#include "SCENE.H"
#include "WINDOW.H"

void Ui_AdjustValueWithoutLimit(s32 no, s32 value)
{
    UiText_DecodeMessage(no, value, 0x7fffffff);
}
