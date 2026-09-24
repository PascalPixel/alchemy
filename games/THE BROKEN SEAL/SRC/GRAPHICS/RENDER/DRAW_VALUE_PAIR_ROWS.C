#include "TYPES.H"

void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawStringInWindowFar(s32 image, s32 layer, s32 x, s32 y);
void UiText_DrawNumberRightAlignedFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 UiWork_SetParamNibbleFar(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

    UiText_DrawStringAtOffsetFar(0x080af210, layer, 0, 40);
    UiText_DrawStringInWindowFar(0x080af214, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiWork_SetParamNibbleFar(4);
    }
    if (val == 0) {
        UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 40);
    UiWork_SetParamNibbleFar(15);
    UiText_DrawStringAtOffsetFar(0x080af218, layer, 0, 48);
    UiText_DrawStringInWindowFar(0x080af214, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    UiText_DrawNumberRightAlignedFar(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    UiText_DrawNumberRightAlignedFar(val, layer, 88, 48);
}
