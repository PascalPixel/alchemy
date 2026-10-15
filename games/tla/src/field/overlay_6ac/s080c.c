#include "ov6ac.h"

extern u16 Data_02003a7e;

void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

// Calls Func_02003178 with (0, 125, 3, 3) for four places, steps
// Data_02003a7e through 12 and 8 to 0, then calls it with (0, 122, 3, 3) for
// the same places.
void Func_0200080c(void)
{
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
    Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
    WaitFrames(1);
    Data_02003a7e = 12;
    WaitFrames(12);
    Data_02003a7e = 8;
    WaitFrames(4);
    Data_02003a7e = 12;
    WaitFrames(4);
    Data_02003a7e = 8;
    WaitFrames(4);
    Data_02003a7e = 12;
    WaitFrames(4);
    Data_02003a7e = 0;
    WaitFrames(12);
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 67);
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 72);
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 17, 72);
    Call6((void (*)())Func_02003178, 0, 122, 3, 3, 8, 67);
    WaitFrames(1);
}
