#include "TYPES.H"
#include "SCENE.H"

/* shop/unit/hilite.c */
void AnimationObjects_SelectAnimationFar(void *, s32);

extern u8 *gMenuWork;

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
    u8 *state;
    u8 *half_base;
    s32 *item;
    s32 index;
    s32 offset;
    s32 variant;
    s16 id;

    state = gMenuWork;
    variant = *(s8 *)(state + 0x3aa);
    if (enabled != 0) {
        index = 0;
        if (index < *(s8 *)(state + 0x3a7)) {
            half_base = state + 2;
            offset = 0x36c;
            item = (s32 *)(state + 0x114);
            do {
                if (index == selected)
                    AnimationObjects_SelectAnimationFar((void *)*item, 30);
                else
                    AnimationObjects_SelectAnimationFar((void *)*item, 1);
                item[16] = 0x10000;
                id = *(s16 *)(half_base + offset);
                if (Shop_CanServe(id, variant) == 0)
                    item[16] = 0xb333;
                index++;
                offset += 2;
                item++;
            } while (index < *(s8 *)(state + 0x3a7));
        }
    }
}

/* shop/draw/sel_msg.c */
extern u8 Value_00000d2c;
extern u8 Value_00000d2d;

void UiWindow_Clear(s32 target);
void UiText_DrawMessageAt(s32 message, s32 target, s32 arg2, s32 arg3);
void UiWork_PushValueSlotFar(s32 message, s32 style);

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = (s8)gMenuWork[0x3AA];
    message = Shop_ServicePrice(selection, variant);
    if (target != 0) {
        UiWindow_Clear(target);
        if (Shop_CanServe(selection, variant) != 0) {
            variant = (s32)&Value_00000d2c;
        } else {
            variant = (s32)&Value_00000d2d;
        }
        variant = Shop_MsgByMode(variant);
        UiWork_PushValueSlotFar(message, 5);
        UiText_DrawMessageAt(variant, target, 0, 0);
    }
}
