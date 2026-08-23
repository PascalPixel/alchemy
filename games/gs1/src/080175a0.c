#include "types.h"

void WaitFrames(s32);
s32 Func_08017364(void);
void Func_080174f8(s32);

void Func_080175a0(s32 argument) {
    Func_080174f8(argument);
    goto check;
again:
    WaitFrames(1);
check:
    if (Func_08017364() == 0) {
        goto again;
    }
    WaitFrames(1);
}
