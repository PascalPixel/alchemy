/* Draft of resource_3ac 0x020081d8 (ItemMerchant_ReadMind): it matches the
 * ROM byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgRunpaIsntWeaponsVendors,
 * MsgRunpaItemMerchantSealedThoughts). The listing keeps these rows until the
 * draft is adopted. */
#include "INTERIORS.H"
extern u8 MsgRunpaIsntWeaponsVendors[];
extern u8 MsgRunpaItemMerchantSealedThoughts[];


void ItemMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaIsntWeaponsVendors);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgRunpaItemMerchantSealedThoughts);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    }
}

