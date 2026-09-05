#include "types.h"

#define UiPalette_SetColor4 Func_080a24c4

s32 UiPalette_SetColor(s32);

void UiPalette_SetColor4(void) {
    UiPalette_SetColor(4);
}
