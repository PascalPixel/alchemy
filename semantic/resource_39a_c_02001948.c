#include "types.h"

extern s16 Data_02000240[];

extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_02001050(void);
extern void Func_02001154(void);
extern void Func_020012cc(void);
extern void Func_020015dc(void);

void Func_02001948(void)
{
    s32 state = Data_02000240[224];

    if (state == 0x3e) {
        Func_080091c0(8, 29, 15, 5, 8, 42);
        Func_02001050();
    } else if (state == 0x3f) {
        Func_080091c0(12, 8, 10, 18, 0, 28);
        Func_02001154();
    } else if (state == 0x40 && Data_02000240[225] != 1) {
        Func_080091c0(12, 21, 9, 16, 12, 3);
        Func_020012cc();
    } else if (state == 0x41) {
        u32 selector = (u16)Data_02000240[225];

        if (selector - 1 <= 1)
            Func_080091c0(14, 10, 9, 8, 22, 20);
        else
            Func_080091c0(7, 45, 11, 4, 20, 45);
        Func_020015dc();
    }
}
