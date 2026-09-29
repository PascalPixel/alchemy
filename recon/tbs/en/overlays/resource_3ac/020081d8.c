/* Draft of resource_3ac 0x020081d8 (ItemMerchant_ReadMind), from
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/RUNPA_SUHARA/INTERIORS.C.
 * Remaining difference: the ROM loads message 0x1be0 from the literal pool,
 * as a link-time message symbol would; C builds it with movs and a shift.
 * The listing keeps these rows. */
#include "INTERIORS.H"

extern u8 LinkedMessage_ItemMerchantSealedThoughts;

void ItemMerchant_ReadMind(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_Begin();
        Event_SetMessage(MSG_ITEM_MERCHANT_REOPENED_THOUGHTS);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    } else {
        Event_Begin();
        Event_SetMessage((s32)&LinkedMessage_ItemMerchantSealedThoughts);
        Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
        Event_End();
    }
}

