#include "types.h"
#include "scene.h"

/* shop/unit/hilite.c */
void Object_InitializeMode(void *, s32);

extern u8 *gIw;

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
    u8 *state;
    u8 *half_base;
    s32 *item;
    s32 index;
    s32 offset;
    s32 variant;
    s16 id;

    state = gIw;
    variant = *(s8 *)(state + 0x3aa);
    if (enabled != 0) {
        index = 0;
        if (index < *(s8 *)(state + 0x3a7)) {
            half_base = state + 2;
            offset = 0x36c;
            item = (s32 *)(state + 0x114);
            do {
                if (index == selected)
                    Object_InitializeMode((void *)*item, 30);
                else
                    Object_InitializeMode((void *)*item, 1);
                item[16] = 0x10000;
                id = *(s16 *)(half_base + offset);
                if (Sys_Apply(id, variant) == 0)
                    item[16] = 0xb333;
                index++;
                offset += 2;
                item++;
            } while (index < *(s8 *)(state + 0x3a7));
        }
    }
}

/* shop/draw/sel_msg.c */
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
