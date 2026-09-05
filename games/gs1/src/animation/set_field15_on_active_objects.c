#include "types.h"

void AnimationObjects_SetField15OnActive(u8 *grp, s32 val)
{
    u8 n = grp[0x27];
    if (n != 0) {
        /* 登録順を保ったまま有効な項目へ値を配る。 */
        void **p = (void **)(grp + 0x28);
        s32 cnt = n;
        do {
            u8 *obj = *p++;
            if (obj != 0 && *(s32 *)(obj + 0xc) != 0)
                obj[0x15] = val;
            cnt--;
        } while (cnt != 0);
    }
}
