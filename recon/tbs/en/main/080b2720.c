#include "TYPES.H"

/* Draft: a separate signed sentinel value removes the extra saved register,
   but the loop still uses ldrh and a left shift instead of ldrsh; counter
   and pointer registers differ. Value_00000000 restores the word pool load. */

extern s16 EventTable_AbilityLoadouts[][33];
extern u8 Value_00000000;

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
    s16 *src;
    s16 *dst;
    s32 count;
    s32 value;

    value = EventTable_AbilityLoadouts[row_no][0];
    count = 0;
    if (value != 0) {
        dst = output;
        src = EventTable_AbilityLoadouts[row_no];
        do {
            *dst = *src;
            count++;
            src++;
            dst++;
            if (count > 23)
                break;
            value = *src;
        } while (value != 0);
    }
    output[count] = (s32)&Value_00000000;
    return count;
}
