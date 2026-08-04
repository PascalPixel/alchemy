#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern s32 Func_080770c0(s32);
extern void *Func_08077008(s32);

s32 Func_080b6ae0(s16 *entries) {
    s16 *output;
    s32 result;
    s32 id;
    s32 limit;
    s32 count;

    output = entries;
    count = 0;
    limit = 6;
    result = 0;
    if (output != NULL) {
        if (Func_080770c0(0x16C) != 0) {
            limit = 3;
        }
        id = 0x80;
        limit += 0x80;
        for (; id < limit; id += 1) {
            if (M2C_FIELD(Func_08077008(id), u8 *, 0x12A) != 0) {
                *output = (s16) id;
                count += 1;
                output += 1;
            }
        }
        *output = 0xFF;
        result = count;
    }
    return result;
}
