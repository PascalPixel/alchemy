/*
 * Draft: Shop_SellOld does not yet match; 2 halfwords differ from ☀️'s C, first at +0x20 (ldrh r3, [r0, r3]).
 * Links as recon/tla/raw/08109188.s.
 */
#include "SHOP.H"

void Shop_SellItem(s32, s32, s32);

#if defined(TBS_EDITION_JA)
#define BASE_W 11
#else
#define BASE_W 12
#endif

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_SellOld(s32 unit_id, s32 slot)
{
    struct BattleUnit *unit;
    s32 item_offset;
    s32 item_id;

    unit = Owner_GetStateFar(unit_id);
    if (slot == -1)
    {
        return 0;
    }
    item_offset = slot * 2 + 216;
    item_id = 0x1ff & *(u16 *)((u8 *)unit + item_offset);
    if (Item_Get(item_id)->type == 6)
    {
        return 0;
    }
    if (Item_Get(item_id)->flags & 8)
    {
        return 0;
    }
    Shop_SellItem(unit_id, slot, -1);
    return 1;
}
