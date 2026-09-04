#include "types.h"

#define UiPalette_SetColor15 Func_080a24ac

s32 UiPalette_SetColor(s32);

void UiPalette_SetColor15(void) {
    UiPalette_SetColor(15);
}
