/* Item menu: write the names of the equipped items into the equipment
   window, one row per item type 1 to 4. The name message is the item id
   plus 0x182; its pool constant is hoisted into a register for the loop. */
#include "TYPES.H"
#include "ITEM.H"

extern u8 Value_00000182;

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items)
{
    s32 i;
    s32 item;

    for (i = 0; i < 15; i++) {
        if (items[i] & 0x200) {
            item = items[i] & 0x1ff;
            switch (Item_Get(item)->type) {
            case 1:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&Value_00000182, window, 8, 8);
                break;
            case 2:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&Value_00000182, window, 8, 56);
                break;
            case 3:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&Value_00000182, window, 8, 40);
                break;
            case 4:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&Value_00000182, window, 8, 24);
                break;
            }
        }
    }
}
