#include "types.h"

struct ResourceSprite {
    u8 unk00[0x10];
    u8 vramBlock;
    u8 flags;
};

extern u8 gResourceBlockOwners[];
void Resource_ResetEntry(s32 block);

s32 ResourceTable_CountFreeBlocks(void)
{
    u8 *marker = gResourceBlockOwners;
    s32 free_count;
    s32 remaining;

    /* FAKEMATCH: ⚓️ loads the owner table before counting */
    asm volatile("");
    free_count = 0;
    remaining = 0x200;
    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
    void *ret;
    struct ResourceSprite *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
        } else {
            Resource_ResetEntry(obj->vramBlock);
            obj->vramBlock = ((struct ResourceSprite *)alt)->vramBlock;
            *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
            obj = alt;
        }
        ret = obj;
    }
    return ret;
}
