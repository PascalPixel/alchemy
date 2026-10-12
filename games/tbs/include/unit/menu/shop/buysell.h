#ifndef ALCHEMY_UNIT_MENU_SHOP_BUYSELL_H
#define ALCHEMY_UNIT_MENU_SHOP_BUYSELL_H

#include "types.h"

s32 Shop_SelBuy(void);
void Shop_DrawStock(s32 window, s32 selected);
void Shop_DrawMoney(void);
void Shop_DrawItemPrice(s32 window, s32 item, s32 price, s32 price_mode);
void Shop_DrawMsg(s32 win, s32 mes);
void Shop_DrawParty(s32 window, s32 selected, s32 requirement);
void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id);
void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id);
s32 Shop_SelBuyNum(s32 unit_id, s32 item_id);
s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price);
void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity);
s32 Shop_ConfirmEquip(s32 unit_id, s32 slot);
s32 Shop_SellOld(s32 unit_id, s32 slot);
s32 Shop_SalePrice(s32 item_id);
s32 Shop_PickUnit(void);
s32 Shop_SelSell(s32 unit_id);
void Shop_DrawUnitGrid(s32 window, s32 unit_id);
s32 Shop_SelSellNum(s32 unit_id, s32 slot);
void Shop_SellItem(s32 unit_id, s32 slot, s32 count);
s32 Shop_RepairPrice(s32 item_id);
s32 Shop_SelUnit(void);
s32 Shop_SelRepair(s32 unit_id);
void Shop_RepairItem(s32 unit_id, s32 slot);
void Shop_BuySpecialItem(void *window, s32 item_window);

#endif
