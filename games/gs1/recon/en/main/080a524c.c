#include "types.h"
#include "item.h"
#include "global_cells.h"

s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
struct ItemDefinition *Item_Get(s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Audio_PlayCue(s32);
s32 Modulo(s32, s32);
void WaitFrames(s32);
void UiWindow_Close(s32, s32);
s32 GameFlag_IsSet(s32);
void Func_080a1ac0(s32, s32);
void Func_080a1a40(s32, s32);

s32 Func_080a524c(s32 item_id)
{
    s32 selection;
    s32 window;
    s32 changed;
    s32 msg;

    window = UiWindow_CreateFar(13, 3, 17, 10, 2);
    Item_Get(item_id & 0x1FF);
    msg = item_id & 0x1FF;
    msg += 0x182;
    UiText_DrawAt(msg, window, 24, 0);
    item_id = 0xAD4;
    UiText_DrawAt(item_id, window, 0, 16);
    item_id += 1;
    UiText_DrawAt(item_id, window, 0, 24);
    item_id = 0xB2C;
    UiText_DrawAt(item_id, window, 24, 40);
    item_id += 1;
    UiText_DrawAt(item_id, window, 24, 56);
    selection = 1;
    changed = selection;
    Func_080a1ac0(104, 86);

    for (;;) {
        Func_080a1a40(104, selection * 16 + 70);
        if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
            changed = 1;
            selection -= 1;
            Audio_PlayCue(111);
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
            changed = 1;
            selection += 1;
            Audio_PlayCue(111);
        }
        WaitFrames(1);

        if (GameFlag_IsSet(0x150)) {
            break;
        }
        if (changed != 0) {
            changed = 0;
            selection = Modulo(selection + 2, 2);
        }
        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            Audio_PlayCue(112);
            break;
        }
        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(113);
            selection = 1;
            break;
        }
    }
    if (GameFlag_IsSet(0x150)) {
        selection = 1;
    }
    UiWindow_Close(window, 1);
    return selection;
}
