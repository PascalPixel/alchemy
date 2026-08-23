#include "types.h"

extern u8 *Data_03001f2c;
extern u8 Value_00000d2c;
extern u8 Value_00000d2d;

void UiWindow_Clear(s32 target);
void UiText_DrawMessageAt(s32 message, s32 target, s32 arg2, s32 arg3);
void UiText_DrawQuantity(s32 message, s32 style);
s32 Func_080b2778(s32 selection, s32 variant);
s32 Func_080b27b0(s32 selection, s32 variant);
s32 Func_080b2884(s32 messageId);

void Func_080b2ed8(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = (s8)Data_03001f2c[0x3AA];
    message = Func_080b2778(selection, variant);
    if (target != 0) {
        UiWindow_Clear(target);
        if (Func_080b27b0(selection, variant) != 0) {
            variant = (s32)&Value_00000d2c;
        } else {
            variant = (s32)&Value_00000d2d;
        }
        variant = Func_080b2884(variant);
        UiText_DrawQuantity(message, 5);
        UiText_DrawMessageAt(variant, target, 0, 0);
    }
}
