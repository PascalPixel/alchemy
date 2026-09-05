#include "types.h"
#include "global_cells.h"

#define Map_ApplyWorkOriginAndSpan Func_0800fe9c

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08010000();
s32 Func_08010230(s32, s32);

void Map_ApplyWorkOriginAndSpan(void) {
    s32 first;
    s32 third;
    s32 second;
    s32 *p;

    p = **(s32 ***)ADDR_03001E70;
    first = 0;
    second = 0;
    third = 0;
    if (p != NULL) {
        first = *p++;
        second = *p++;
        third = *p;
    }
    Func_08010230(first, (s32)((u32)third - (u32)second));
    Func_08010000();
}
