#include "TYPES.H"

s32 Func_080f4168();
#define FunctionHead_080f4168 Func_080f4168
s32 Audio_PlayCue(s32);

s32 Runtime_BlankDisplayAndRun(void)
{
    *(s16 *)0x04000000 = 0x40;
    Audio_PlayCue(9);
    FunctionHead_080f4168();
    return 0;
}
