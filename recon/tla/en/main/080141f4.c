/* Draft, differ 2 of 20: the native loads the table address before the first
 * half of the count (ldr r4; movs r2, #128), this schedules them the other
 * way round. Measured: an empty asm barrier after the marker with run, longest
 * and remaining set after it gives the native order but swaps the registers
 * of run and remaining (differ 18), in any order of the three and of the
 * declarations; with run set before the marker and the barrier after it, run's
 * zero comes before the load (differ 4); a "+r" or "+l" barrier on the marker
 * moves the marker out of r4 (differ 24); a count-up for loop is differ 2;
 * three minutes of permuter (23,351 candidates) found nothing better. */
#include "types.h"

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
