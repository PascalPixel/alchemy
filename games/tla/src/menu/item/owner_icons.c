#include "types.h"
#include "ram_buffer.h"
#include "frame.h"

struct ItemMenuIcons {
    u8 unknown_000[0xf8];
    void *owner_icons[8];
    u8 unknown_118[0x40];
    s16 owners[10];
    u16 item;
};

void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Item_CanOwnerEquip(s32 owner, s32 item);

void Func_080fabe0(void)
{
    struct ItemMenuIcons *menu = Ram_HeapSlots->menu_runtime;
    s8 i;

    if ((gFrameCount & 31) == 0) {
        for (i = 0; i <= 3; i++) {
            s32 owner = menu->owners[i];

            if (owner != -1) {
                if (Item_CanOwnerEquip(owner, menu->item & 0x1ff))
                    Animation_ApplyChildArgumentFar(menu->owner_icons[i], 3);
                else
                    Animation_ApplyChildArgumentFar(menu->owner_icons[i], 1);
            }
        }
    }
}
