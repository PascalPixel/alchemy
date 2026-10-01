#include "TYPES.H"
#include "OWNER_STATE.H"


/*
 * The item menu's quantity of one item in an owner's inventory: the packed
 * count (bits 11-15) of the first nonempty slot holding the item, plus one,
 * or 0 when the owner does not carry it.
 */
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
    struct BattleUnit *state;
    s32 i;
    s32 quantity;

    quantity = 0;
    state = Owner_GetStateFar(owner);
    i = 0;
    do {
        if (state->inventory[i] != 0 && (state->inventory[i] & 0x1ff) == item) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
            /* The Spanish and Italian menus add up every stack of it. */
            quantity += (state->inventory[i] & 0xf800) >> 11;
            quantity++;
#else
            quantity = (state->inventory[i] & 0xf800) >> 11;
            quantity++;
            break;
#endif
        }
        i++;
    } while (i <= 14);
    return quantity;
}
