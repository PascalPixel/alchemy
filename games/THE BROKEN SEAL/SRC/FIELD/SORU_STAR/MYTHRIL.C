/* The mythril bags. */
#include "STAR.H"
extern u8 MsgSoruGotFourMythrilBags[];
extern u8 MsgSoruTooManyItems[];

/*
 * Drain until room: save the s16 counter at scene workspace + 472, prime two
 * channels, then loop while fewer than four of thirty slots are free,
 * requesting more and passing on any event pair that is not -1. On exit it
 * flushes four times with id 224 and restores the saved counter. The 148-byte
 * owner includes its three-word literal pool. Callee roles are not established.
 */
void Scene_GiveMythrilBags(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 saved = *(s16 *)(work + 472);
    s32 first;
    s32 second;
    s32 cnt;

    Audio_PlayCue(0x53);
    Item_ShowFound(ITEM_MYTHRIL_BAG, 3);
    Message_ShowCentered((s32)MsgSoruGotFourMythrilBags, 1);
    do {
        cnt = 30 - Inventory_Count(0);
        cnt -= Inventory_Count(1);

        if (cnt <= 3) {
            Message_ShowCentered((s32)MsgSoruTooManyItems, 1);
            if (Shop_PickUnitItem(&second, &first) != -1)
                Inventory_Discard(second, first);
        }
    } while (cnt <= 3);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    PartyInventory_Add(ITEM_MYTHRIL_BAG);
    *(s16 *)(work + 472) = saved;
}
