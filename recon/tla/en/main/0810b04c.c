#include "TYPES.H"
#include "SCENE.H"
s32 Shop_CanServe(s32, s32);
s32 Shop_ServicePrice(s32 selection, s32 variant);
s32 Shop_MsgByMode(s32 msg);

/* shop/unit/hilite.c */
void AnimationObjects_SelectAnimationFar(void *, s32);

extern u8 *gMenuWork;

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = (s8)gMenuWork[0x3AA];
    message = Shop_ServicePrice(selection, variant);
    if (target != 0) {
        UiWindow_Clear(target);
        if (Shop_CanServe(selection, variant) != 0) {
            variant = (s32)&MsgReviveCost;
        } else {
            variant = (s32)&MsgNoHealingNeeded;
        }
        variant = Shop_MsgByMode(variant);
        UiWork_PushValueSlotFar(message, 5);
        UiText_DrawMessageAt(variant, target, 0, 0);
    }
}
