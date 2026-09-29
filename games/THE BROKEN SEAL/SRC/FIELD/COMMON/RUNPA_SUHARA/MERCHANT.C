/* The item merchant's thoughts when read with Psynergy: one line before
 * Lunpa's trade reopens and one after. */
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
