#include "types.h"
#include "item.h"
#include "text/msg_ids.h"

void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);

// Writes the name of each equipped item of the inventory into window, on the
// row of its equipment slot: item type 1 on the first row, 4 on the second,
// 3 on the third and 2 on the fourth.
void ItemMenu_DrawEquippedItemNames(s32 window, u16 *items) {
  s32 i;
  s32 item;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (items[i] & INVENTORY_EQUIPPED) {
      item = items[i] & ITEM_ID_MASK;
      switch (Item_Get(item)->type) {
      case 1:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, 16, 8);
        break;
      case 2:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, 16, 56);
        break;
      case 3:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, 16, 40);
        break;
      case 4:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, 16, 24);
        break;
      }
    }
  }
}
