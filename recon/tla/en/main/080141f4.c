#include "TYPES.H"

extern u8 ResourceBlockOwners[];

s32 ResourceTable_GetLongestFreeBlockRun(void)
{
    s32 run = 0;
    u8 *marker = ResourceBlockOwners;
    s32 longest = 0;
    s32 remaining = 0x200;

    do {
        if (*marker++ != 0xff) {
            run = 0;
        } else {
            run++;
            if (longest < run)
                longest = run;
        }
        remaining--;
    } while (remaining != 0);
    return longest;
}
