#include "types.h"
#include "item.h"

extern u8 MsgItemName;

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
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, 8, 8);
                break;
            case 2:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, 8, 56);
                break;
            case 3:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, 8, 40);
                break;
            case 4:
                UiText_DrawCharacterAtOffsetFar(item + (s32)&MsgItemName, window, 8, 24);
                break;
            }
        }
    }
}
