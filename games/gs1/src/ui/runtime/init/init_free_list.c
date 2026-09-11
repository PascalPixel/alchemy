#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/init/init_free_list.h"

extern u8 *gIw;

void UiWork_InitFreeList(void)
{
    s32 count;
    u8 *base;
    u8 *item;
    u8 *next;

    base = gIw;
    /* 0x1cバイト単位の空きリストを初期化する。 */
    item = base + 0x698;
    *(u8 **)(base + 0xd98) = item;
    count = 0x3e;
    do {
        next = item + 0x1c;
        count--;
        *(u8 **)item = next;
        item = next;
    } while (count >= 0);
    *(s32 *)next = 0;
    *(u8 **)(base + 0xd9c) = next;
}
