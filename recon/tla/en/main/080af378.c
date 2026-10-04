/* Draft: Item_AdjustCounter, 40-byte EN extent (u16 counter table).
 * 2026-10-04: differ 2 (alchemy diff). The native loads the table address
 * from the pool between building the 0x1ff mask and the ands; this build
 * puts the ands first. Tried: ☀️'s mask local (differ 2, same swap), an
 * indexed table local (4: also swaps the ldrh operands), the mask applied to
 * item_id first (4-11), "+r" barriers on the mask or table (2-4) and
 * volatile input barriers (4-6, they move movs r4 #0), devsearch 120
 * iterations (no gain). Same split-constant scheduling gap as
 * Resource_FindFreeEntry and EffectSlot_Initialize.
 */
#include "INVENTORY.H"

extern u16 Data_080b1f40[];

s32 Item_AdjustCounter(s32 item_id, s32 delta)
{
    s32 result = 0;
    u16 counter = Data_080b1f40[item_id & ITEM_ID_MASK];

    if (counter)
        result = ItemCounter_Adjust(counter - 1, delta);
    return result;
}
