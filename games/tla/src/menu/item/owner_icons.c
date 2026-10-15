#include "types.h"
#include "ram_buffer.h"
#include "frame.h"

struct ItemMenuIcons {
    u8 unk000[0xf8];
    void *ownerIcons[8];
    u8 unk118[0x40];
    s16 owners[10];
    u16 item;
};

void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Item_CanOwnerEquip(s32 owner, s32 item);

void Func_080fabe0(void)
{
    struct ItemMenuIcons *menu = Ram_HeapSlots->menuRuntime;
    s8 i;

    if ((gFrameCount & 31) == 0) {
        for (i = 0; i <= 3; i++) {
            s32 owner = menu->owners[i];

            if (owner != -1) {
                if (Item_CanOwnerEquip(owner, menu->item & 0x1ff))
                    Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 3);
                else
                    Animation_ApplyChildArgumentFar(menu->ownerIcons[i], 1);
            }
        }
    }
}
