#include "TYPES.H"

extern s16 EventTable_AbilityLoadouts[][33];
s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void Ability_GetMaximum(s32, s32);

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
            /* FAKEMATCH: only the copy read is volatile, so its lifetime
               stays separate from the signed sentinel read. */
            *dst = *(volatile s16 *)src;
            count++;
            src++;
            dst++;
            if (count > 23)
                break;
        } while (*src != 0);
    }
    output[count] = 0;
    return count;
}
