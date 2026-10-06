#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SHOP.H"

#define Item_IsCompatibleWithOwnerFar ((s32 (*)(s16,s32))Item_IsCompatibleWithOwnerFar)

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
