#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void Func_0800bc48(void *destination);
void Func_0800b9a4(u8 *arg0, u32 arg1)
{
    void **p;
    void *t3;
    s32 off;
    void *v;
    s32 cnt;
    u32 i;

    if (arg0 != 0 && arg1 <= 3) {
        off = (arg1 * 4) + 0x28;
        v = M2C_FIELD(arg0, void *, off);
        if (v != 0) {
            Func_0800bc48(v);
            M2C_FIELD(arg0, void *, off) = NULL;
            i = arg1 + 1;
            cnt = 0;
            if (i <= 3) {
                p = (void **)((i * 4) + (u32)arg0 + 0x28);
                do {
                    t3 = *p++;
                    if (t3 != 0) {
                        cnt++;
                    }
                    i++;
                } while (i <= 3);
            }
            if (cnt == 0) {
                M2C_FIELD(arg0, s8, 0x27) = (s8)arg1;
            }
        }
    }
}
