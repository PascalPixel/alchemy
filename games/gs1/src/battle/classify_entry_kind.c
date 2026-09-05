#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080772b8(void *);

s32 Battle_ClassifyEntryKind(const u8 *entry) {
    s32 b3;
    s32 kind;
    s32 low;
    s32 ret;
    u8 k;

    ret = 0;
    low = entry[1] & 0xF;
    if (low == 1) {
        ret = 1;
    }
    if (low == 0xB) {
        ret = 2;
    }
    b3 = ((const s8 *)entry)[3]; kind = (s32)(b3);
    k = kind;
    if (k == 3) {
        ret = 3;
    }
    if (k == 4) {
        ret = 4;
    }
    if (k == 0x40) {
        ret = 6;
    }
    if (Func_080772b8(entry[3]) != 0) {
        ret = 5;
    }
    return ret;
}
