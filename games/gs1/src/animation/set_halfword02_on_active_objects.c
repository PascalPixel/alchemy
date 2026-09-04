#include "types.h"

#define AnimationObjects_SetHalfword02OnActive Func_0800bacc

s32 AnimationObjects_SetHalfword02OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    void **p;
    s32 cnt;

    if (n != 0) {
        /* 有効な登録項目だけへ下位16bitを4bit左へずらして設定する。 */
        val = (s32)((u32)val << 4);
        p = (void **)(grp + 0x28);
        cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                *(s16 *)(obj + 2) = val;
            cnt--;
        } while (cnt != 0);
    }
    return 0;
}
