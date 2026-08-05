#include "types.h"

/*
 * Complete flag-0x867 actor-21 arrival scene.  The one-time path stages two
 * actor cues around timed waits, runs the bounded arrival helper, then places
 * actor 21 at the final two coordinates before recording completion.
 */











extern void Func_02001ae4(void);
extern void Func_02001bb2(s32 value);
extern void Func_02001ae8(s32 frames);
extern s32 Func_02001ace(s32 flag);
extern void Func_02001bb4(s32 actor, s32 value);
extern void Func_02001b66(s32 actor, s32 mode, s32 value);
extern void Func_02001b0c(s32 frames);
extern void Func_02001b76(s32 actor, s32 mode, s32 value);
extern void Func_02001b1c(s32 frames);
extern void Func_020013bc(s32 actor, s32 x, s32 y, s32 wait_value);
extern void Func_02001b32(s32 frames);
extern void Func_02001b86(s32 actor, s32 x, s32 y);
extern void Func_02001b92(s32 actor, s32 x, s32 y);
extern void Func_02001b38(s32 flag);
extern void Func_02001b64(void);
void Func_020009f8(void)
{
    Func_02001ae4();
    Func_02001bb2(100);
    Func_02001ae8(40);

    if (Func_02001ace(0x867) == 0) {
        Func_02001bb4(21, 258);
        Func_02001b66(21, 4, 0);
        Func_02001b0c(12);
        Func_02001b76(21, 4, 0);
        Func_02001b1c(20);
        Func_020013bc(21, 392, 104, 0x70000);
        Func_02001b32(20);
        Func_02001b86(21, 408, 104);
        Func_02001b92(21, 408, 120);
        Func_02001b38(0x867);
    }

    Func_02001b64();
}
