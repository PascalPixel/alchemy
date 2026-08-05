#include "types.h"

/* Complete actor-8 flag-setting dialogue scene through its two-word pool. */







extern void Func_020015c4(void);
extern void Func_02001636(s32 actor, s32 mode, s32 value);
extern void Func_020015b4(s32 flag);
extern void Func_0200165a(s32 dialogue);
extern void Func_02001672(s32 actor, s32 mode);
extern void Func_020015ee(void);
void Func_020004d8(void)
{
    Func_020015c4();
    Func_02001636(8, 0, 2);
    Func_020015b4(0x305);
    Func_0200165a(0x1cab);
    Func_02001672(8, 0);
    Func_020015ee();
}
