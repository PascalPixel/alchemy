#include "types.h"

#define Battle_AdvanceCounterAndCheckChance Func_080bf4c4

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Runtime_GetObject(s32);
s32 Func_080bf208(s32 id, s32 arg1, s32 arg2);

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 t2;
    s32 ret;
    u8 t3;
    s32 t;
    s32 cnt;
    void *obj;

    obj = Runtime_GetObject(id);
    cnt = FIELD_AT_OFFSET(obj, u8 *, 0x13D);
    t = cnt & 0xFF;
    if (t != 0) {
        if ((u32)t > 7U) {
            t += 0xF8;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t;
            cnt = t;
        }
        if (cnt & 7) {
            t2 = cnt + 0xFF;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t2;
            cnt = t2;
        }
        ret = 1;
        t3 = cnt;
        if (t3 != 0) {
            if ((u32)t3 <= 7U &&
                Func_080bf208(id, FIELD_AT_OFFSET(obj, u8 *, 0x13D), 0x1E) != 0) {
                FIELD_AT_OFFSET(obj, u8 *, 0x13D) = 0U;
                return 1;
            }
            goto block_9;
        }
        return ret;
    }
block_9:
    ret = 0;
    return ret;
}
