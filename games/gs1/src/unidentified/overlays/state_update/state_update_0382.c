#include "types.h"

extern void Func_02004412(void);
extern void Func_020043ce(s32, s32, s32, s32, s32, s32);
extern void Func_020013fa(void);
extern void Func_020036da(void);
extern void Func_0200443a(void);

void Func_02001318(void)
{
    Func_02004412();
    {
        s32 width = 9;
        s32 height = 38;

        Func_020043ce(73, 38, 5, 5, width, height);
    }
    Func_020013fa();
    Func_020036da();
    Func_0200443a();
}
