#include "types.h"

extern u8 ResourceBlockOwners[];

/* Keep the 128-block scan bound live independently from the run counter. */
#define SCAN_BLOCKS(seed) (((u32)(seed) | ~(u32)(seed)) + 129)

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    u8 *marker = ResourceBlockOwners;
    s32 current_run = 0;
    s32 remaining = SCAN_BLOCKS(marker);
    s32 longest_run = 0;

    remaining <<= 2;
    do {
        if (*marker++ != 0xff) {
            current_run = 0;
        } else {
            current_run++;
            if (longest_run < current_run) {
                longest_run = current_run;
            }
        }
        remaining--;
    } while (remaining != 0);
    return longest_run;
}
