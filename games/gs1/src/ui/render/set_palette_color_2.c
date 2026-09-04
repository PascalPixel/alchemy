#include "types.h"

#define UiPalette_SetColor2 Func_080a24b8

s32 UiPalette_SetColor(s32);

void UiPalette_SetColor2(void) {
    UiPalette_SetColor(2);
}
