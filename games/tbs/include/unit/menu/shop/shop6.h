#ifndef ALCHEMY_UNIT_MENU_SHOP_SHOP6_H
#define ALCHEMY_UNIT_MENU_SHOP_SHOP6_H

#include "types.h"

s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item);
s32 Shop_SelUse(s32 actor);
void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id);

#endif
