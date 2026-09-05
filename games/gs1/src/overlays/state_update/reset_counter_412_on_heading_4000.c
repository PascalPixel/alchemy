#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_0200221a(s32);
extern u8 *Func_020036a8(s32);
extern void Func_02003750(void);

void SceneState_ResetCounter412OnHeading4000(void)
{
    u8 *p;
    s16 *cnt;
    s32 zero;

    Func_0200221a(3);
    p = Func_020036a8(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Func_02003750();
            zero = 0;
            *cnt = zero;
        }
    }
}
