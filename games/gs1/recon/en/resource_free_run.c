#include "types.h"

extern u8 ResourceBlockOwners[];

/* Canonical C candidate: loop registers differ from the reference. */
s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 current_run = 0;
    s32 remaining = 0x200;
    s32 longest_run = 0;

    do {
        if (*marker++ != 0xff) {
            current_run = 0;
        } else {
            current_run++;
            if (longest_run < current_run)
                longest_run = current_run;
        }
        remaining--;
    } while (remaining != 0);
    return longest_run;
}
