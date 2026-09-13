#include "types.h"

void Func_08015090(s32 image, s32 layer, s32 x, s32 y);
void Func_08015098(s32 image, s32 layer, s32 x, s32 y);
void Func_080a14f0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 UiPalette_SetColor(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

    Func_08015090(0x080af210, layer, 0, 40);
    Func_08015098(0x080af214, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    Func_080a14f0(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiPalette_SetColor(4);
    }
    if (val == 0) {
        UiPalette_SetColor(2);
    }
    Func_080a14f0(val, layer, 48, 40);
    UiPalette_SetColor(15);
    Func_08015090(0x080af218, layer, 0, 48);
    Func_08015098(0x080af214, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    Func_080a14f0(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    Func_080a14f0(val, layer, 88, 48);
}
