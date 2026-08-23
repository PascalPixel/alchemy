#include "shop.h"
#include "battle_runtime.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_GetSelectionState(s32, s32);
void Func_080b04dc(s32);
s32 Func_080b1614(s32, s32, s32);
void WaitFrames(u32);
void Func_080a1038(void *);
extern char Value_00000cad;

s32 Shop_SelectSaleQuantity(s32 unit_id, s32 slot)
{
    s32 result;
    s16 saved_x;
    s16 saved_y;
    s32 effect;
    s32 state;
    s32 entry_offset;
    s32 selection;
    struct ItemDefinition *item;
    struct ShopRuntime *shop;
    struct BattleUnit *unit;

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    entry_offset = (slot * 2) + 0xd8;
    item = Item_Get(*(u16 *)((u8 *)unit + entry_offset));
    result = 1;
    effect = Shop_ComputeSalePrice(*(u16 *)((u8 *)unit + entry_offset));
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        Func_080b04dc((s32)&Value_00000cad);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->kind = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = Func_080b1614(0, selection, effect);
        WaitFrames(1);
        Func_080a1038(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}
