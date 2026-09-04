#include "types.h"

#define Battle_GetObjectTableValue Func_080b8530

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern void *Runtime_GetObject(s32);
extern s32 Func_080c2454(s32 index);
extern s32 Func_080c23c0(s32 index);

u32 Battle_GetObjectTableValue(s32 id) {
    u32 value;
    u8 no;

    no = FIELD_AT_OFFSET(Runtime_GetObject(id), u8 *, 0x128);
    value = (u32) (Func_080c2454((s32) no) << 0x18) >> 8;
    if (value == 0) {
        no = FIELD_AT_OFFSET(Runtime_GetObject(id), u8 *, 0x128);
        if (Func_080c23c0((s32) no) != 0) {
            value = 0x180000;
        } else {
            value = 0x300000;
        }
    }
    return value;
}
