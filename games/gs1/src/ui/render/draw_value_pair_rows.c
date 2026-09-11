#include "types.h"
#include "scene.h"
#include "abi/ui/render/draw_value_pair_rows.h"

s32 UiPalette_SetColor(s32 color);

void Ui_DrawValuePairRows(void *obj, s32 layer)
{
    s16 val;

    Ui_SetMode(0x080af210, layer, 0, 40);
    Ui_SetMode2(0x080af214, layer, 48, 40);
    val = *(s16 *)((u8 *)obj + 52);
    Ui_SetMode3(val, layer, 88, 40);
    val = *(s16 *)((u8 *)obj + 56);
    if (val < ((s32)(u16)*(s16 *)((u8 *)obj + 52) << 16) >> 18) {
        UiPalette_SetColor(4);
    }
    if (val == 0) {
        UiPalette_SetColor(2);
    }
    Ui_SetMode3(val, layer, 48, 40);
    UiPalette_SetColor(15);
    Ui_SetMode(0x080af218, layer, 0, 48);
    Ui_SetMode2(0x080af214, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 58);
    Ui_SetMode3(val, layer, 48, 48);
    val = *(s16 *)((u8 *)obj + 54);
    Ui_SetMode3(val, layer, 88, 48);
}
