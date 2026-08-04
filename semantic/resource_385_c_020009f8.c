#include "types.h"

/*
 * Complete flag-0x867 actor-21 arrival scene.  The one-time path stages two
 * actor cues around timed waits, runs the bounded arrival helper, then places
 * actor 21 at the final two coordinates before recording completion.
 */
extern void Func_0808a018(void);
extern void Func_080f9010(s32 value);
extern void Func_0808a010(s32 frames);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a1f0(s32 actor, s32 value);
extern void Func_0808a128(s32 actor, s32 mode, s32 value);
extern void Func_02000970(s32 actor, s32 x, s32 y, s32 wait_value);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_080770c8(s32 flag);
extern void Func_0808a020(void);

void Func_020009f8(void)
{
    Func_0808a018();
    Func_080f9010(100);
    Func_0808a010(40);

    if (Func_080770c0(0x867) == 0) {
        Func_0808a1f0(21, 258);
        Func_0808a128(21, 4, 0);
        Func_0808a010(12);
        Func_0808a128(21, 4, 0);
        Func_0808a010(20);
        Func_02000970(21, 392, 104, 0x70000);
        Func_0808a010(20);
        Func_0808a0d0(21, 408, 104);
        Func_0808a0d0(21, 408, 120);
        Func_080770c8(0x867);
    }

    Func_0808a020();
}
