/*
 * Draft of Owner_GetDigitValues, ported from the Broken Seal function as
 * three plain indexed loops. 2 instructions differ: the listing loads the
 * preset table address between doubling and tripling the index (lsls r3;
 * ldr r2; adds r3), the draft after the add. Tried: a preset pointer (12
 * differ, the index moves to r2) and a doubled temporary behind an asm
 * barrier (12).
 */
#include "types.h"

struct ElementPreset {
    u8 unknown_00[4];
    u8 digits[4];
    u8 unknown_08[16];
};

extern const struct ElementPreset Data_080c6684[];

const u8 *Owner_GetRecord(s32 record);
const u8 *Character_GetDefinition(s32 record);

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4])
{
    if (record > 7) {
        u32 index;
        s32 i;

        index = Owner_GetRecord(record)[42];
        if (index > 47)
            index = 0;
        for (i = 0; i <= 3; i++)
            output[i] = Data_080c6684[index].digits[i] * 10;
    } else {
        s32 i;

        for (i = 0; i <= 3; i++)
            output[i] = source[36 + i] * 10;
        if (record <= 7) {
            const u8 *definition = Character_GetDefinition(record);

            for (i = 0; i <= 3; i++)
                output[i] += definition[146 + i];
        }
    }
    return 0;
}
