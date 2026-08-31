#include "psynergy_menu.h"

extern void UiText_DrawAt(s32, s32, s32, s32);
extern s32 Func_08015108(s32, s32, s32, s32, s32);
extern void UiText_DrawQuantity(s32, s32);
extern void Func_08015090(void *, void *, s32, s32);
extern void *Runtime_GetObject(s32);
extern void UiPalette_SetColor(s32);

#define UiText_GetResourceDimensionsFar Func_08015108
#define UiText_DrawStringAtOffsetFar Func_08015090
#define PsynergyMenu_DrawShortcuts Func_080a6614

s32 PsynergyMenu_DrawShortcuts(
    s32 window,
    s32 unused,
    struct MenuResult *result)
{
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 second_is_tall;
    s32 first_is_tall;

    if (Data_02000240.psynergy_shortcuts[0] != 0 &&
        Data_02000240.psynergy_shortcuts[1] != 0) {
        UiText_DrawAt(0xAE4, window, 0, -8);
    } else {
        UiText_DrawAt(0xAE0, window, 0, -8);
    }
    UiText_GetResourceDimensionsFar(
        (Data_02000240.psynergy_shortcuts[0] & 0x3FF) + 0x333,
        (s32)&left,
        (s32)&top,
        (s32)&width,
        (s32)&height);
    first_is_tall = 1;
    if ((u32)width <= 10) {
        first_is_tall = 0;
    }
    if (Data_02000240.psynergy_shortcuts[0] != 0) {
        UiText_DrawQuantity(Data_02000240.psynergy_shortcuts[0] & 0x3FF, 4);
        UiText_DrawAt(0xAE7, window, 0, 0);
        if (first_is_tall == 0) {
            UiText_DrawStringAtOffsetFar(
                Runtime_GetObject(
                    Data_02000240.psynergy_shortcuts[0] >> 10),
                (void *)window,
                0x50,
                0);
        }
    } else {
        UiText_DrawAt(0xAE5, window, 0, 0);
    }
    UiText_GetResourceDimensionsFar(
        (Data_02000240.psynergy_shortcuts[1] & 0x3FF) + 0x333,
        (s32)&left,
        (s32)&top,
        (s32)&width,
        (s32)&height);
    second_is_tall = 1;
    if ((u32)width <= 10) {
        second_is_tall = 0;
    }
    if (Data_02000240.psynergy_shortcuts[1] != 0) {
        UiText_DrawQuantity(Data_02000240.psynergy_shortcuts[1] & 0x3FF, 4);
        UiText_DrawAt(0xAE8, window, 0, 8);
        if (second_is_tall == 0) {
            UiText_DrawStringAtOffsetFar(
                Runtime_GetObject(
                    Data_02000240.psynergy_shortcuts[1] >> 10),
                (void *)window,
                0x50,
                8);
        }
        UiPalette_SetColor(0xF);
    } else {
        UiText_DrawAt(0xAE6, window, 0, 8);
    }
    return 1;
}
