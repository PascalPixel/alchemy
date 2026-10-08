#include "types.h"

extern u8 ResourceBlockOwners[];

s32 ResourceTable_CountFreeBlocks(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 free_count = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ == 0xff) {
            free_count++;
        }
        remaining--;
    } while (remaining != 0);
    return free_count;
}
