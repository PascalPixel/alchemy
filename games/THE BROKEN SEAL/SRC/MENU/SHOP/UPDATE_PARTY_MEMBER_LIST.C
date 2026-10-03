#include "SHOP.H"

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *, s32);
s32 Item_IsCompatibleWithOwnerFar(s16, s32);

extern struct ShopRuntime *gMenuWork;

void Shop_DrawParty(s32 window, s32 selected, s32 requirement)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 index;

    if (window != 0) {
        for (index = 0; index < shop->party_member_count; index++) {
            if (index == selected)
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
            else
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
            shop->party_member_scale[index] = 0x10000;
            if (!Item_IsCompatibleWithOwnerFar(shop->party_member_ids[index], requirement))
                shop->party_member_scale[index] = 0xcccc;
        }
    }
}
