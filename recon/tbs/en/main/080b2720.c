#include "TYPES.H"

/* Draft: signed source and destination alone retain an extra saved register
   and replace the loop's signed load with a halfword load and left shift.
   The final zero also uses a halfword pool load instead of a word load. */

extern s16 EventTable_AbilityLoadouts[][33];

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
    s16 *src;
    s16 *dst;
    s32 count;

    count = 0;
    if (EventTable_AbilityLoadouts[row_no][0] != 0) {
        dst = output;
        src = EventTable_AbilityLoadouts[row_no];
        do {
            *dst = *src;
            count++;
            src++;
            dst++;
        } while (count <= 23 && *src != 0);
    }
    output[count] = 0;
    return count;
}
