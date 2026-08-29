#include "types.h"
#include "runtime_interfaces.h"

struct Record_080797fc {
    u8 padding[2];
    u8 values[148];
};

struct Record_080797fc *Func_08078ed8(s32);
#include "preset_table.h"

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4]) {
    s32 i;
    s32 *cursor;

    if (record > 7) {
        u32 index;

        index = Owner_GetRecord(record)[52];
        if (index > 43)
            index = 0;

        i = 0;
        cursor = output;
        for (; i <= 3; i++)
            *cursor++ = Data_08088e38[index].digits[i] * 10;
    } else {
        cursor = output;
        source += 36;
        for (i = 3; i >= 0; i--) {
            u32 value = *source;
            source++;
            *cursor++ = value * 10;
        }

        if (record <= 7) {
            for (i = 0; i <= 3; i++) {
                *output += Func_08078ed8(record)->values[144 + i];
                output++;
            }
        }
    }

    return 0;
}
