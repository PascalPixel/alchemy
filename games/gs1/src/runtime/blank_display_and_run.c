#include "types.h"

#define Runtime_BlankDisplayAndRun Func_080f4008

s32 Func_080f4168();
s32 Audio_PlayCue(s32);

s32 Runtime_BlankDisplayAndRun(void) {
    *(s16 *)0x04000000 = 0x40;
    Audio_PlayCue(9);
    Func_080f4168();
    return 0;
}
