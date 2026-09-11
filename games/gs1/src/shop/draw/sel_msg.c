#include "types.h"
#include "scene.h"
#include "abi/shop/draw/sel_msg.h"

extern u8 *gIw;
extern u8 Value_00000d2c;
extern u8 Value_00000d2d;

void UiWindow_Clear(s32 target);
void UiText_DrawMessageAt(s32 message, s32 target, s32 arg2, s32 arg3);
void UiText_DrawQuantity(s32 message, s32 style);

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = (s8)gIw[0x3AA];
    message = Sys_Apply(selection, variant);
    if (target != 0) {
        UiWindow_Clear(target);
        if (Sys_Apply2(selection, variant) != 0) {
            variant = (s32)&Value_00000d2c;
        } else {
            variant = (s32)&Value_00000d2d;
        }
        variant = Sys_Check(variant);
        UiText_DrawQuantity(message, 5);
        UiText_DrawMessageAt(variant, target, 0, 0);
    }
}
