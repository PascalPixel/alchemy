#include "BATTLE_RUNTIME.H"
#include "ITEM.H"

u8 Item_GetTargetMode(s32 item_id)
{
    return BattleAction_GetDirect(
        Item_GetDirect(item_id)->action_id)->target_mode;
}

#include "ITEM.H"

extern u8 gItemCounters[128];

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
    s32 counter_slot = index;
    u8 *data = gItemCounters;

    index = 0;
    if (counter_slot <= 127) {
        s32 value = data[counter_slot];

        value += delta;

        if (value < 0) {
            value = 0;
        } else if (value > 99) {
            value = 99;
            index = 99;
        } else {
            index = value;
        }
        data[counter_slot] = value;
    }
    return index;
}

#include "ITEM.H"

extern u8 Item_ArtifactSlotTable[];

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 item_id_mask = 0x1ff;
    u8 counter;
    s32 result = 0;

    counter = Item_ArtifactSlotTable[item_id & item_id_mask];
    if (counter != 0) {
        result = ItemCounter_Adjust(counter - 1, delta);
    }
    return result;
}
