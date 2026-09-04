#include "types.h"

#define BattlePresentation_BuildTargetList Func_080b9d34

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Runtime_GetObject(u8);

s32 BattlePresentation_BuildTargetList(void *desc, void *output) {
    s32 sp0;
    void *sp4;
    s16 *dst;
    s32 v;
    s32 cnt;
    s32 i;
    u8 *p;
    void *out;

    out = output;
    cnt = 0;
    FIELD_AT_OFFSET(out, s32 *, 0x1C) = 0;
    v = FIELD_AT_OFFSET(desc, s32 *, 0x58);
    FIELD_AT_OFFSET(out, s32 *, 0) = (s32) (0xFFF & v);
    FIELD_AT_OFFSET(out, u32 *, 0x18) = (u32) ((u32) (v & 0x3000) >> 0xC);
    FIELD_AT_OFFSET(out, s32 *, 8) = (s32) FIELD_AT_OFFSET(desc, u8 *, 0);
    i = 0;
    if (i < (s32) FIELD_AT_OFFSET(desc, s8 *, 1)) {
        p = desc + 2;
        dst = out + 0x24;
        do {
            sp4 = out;
            sp0 = cnt;
            if ((FIELD_AT_OFFSET(Runtime_GetObject(*p), s16 *, 0x38) != 0) || (FIELD_AT_OFFSET(desc, s32 *, 0x58) & 0x10000)) {
                cnt += 1;
                *dst = (s16) *p;
                dst += 1;
            }
            i += 1;
            p += 1;
        } while (i < (s32) FIELD_AT_OFFSET(desc, s8 *, 1));
    }
    if (cnt == 0) {
        FIELD_AT_OFFSET(out, s16 *, 0x24) = (s16) FIELD_AT_OFFSET(desc, u8 *, 2);
        cnt = 1;
    }
    FIELD_AT_OFFSET(out, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(desc, u8 *, 2);
    FIELD_AT_OFFSET(out, s32 *, 0x14) = cnt;
    FIELD_AT_OFFSET(out, s32 *, 0x10) = 1;
}
