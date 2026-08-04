#include "types.h"

/*
 * Complete actor-18 three-cue movement scene through its literal pool.  The
 * actor enters at y=408, performs three evenly timed cues, moves to y=392,
 * and records story flag 0x858 before the scene closes.
 */
extern void Func_080772e8(s32 cue);
extern void Func_0808a018(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a090(s32 actor, s32 width, s32 height);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a128(s32 actor, s32 mode, s32 value);
extern void Func_080770c8(s32 flag);
extern void Func_0808a020(void);

void Func_02000d84(void)
{
    Func_080772e8(231);
    Func_0808a018();
    Func_0808a010(10);
    Func_0808a138(18, 2);
    Func_0808a090(18, 0xcccc, 0x6666);
    Func_0808a0d0(18, 216, 408);
    Func_0808a010(10);
    Func_0808a1b8(18, 0x4000, 20);

    Func_0808a128(18, 6, 0);
    Func_0808a010(30);
    Func_0808a128(18, 6, 0);
    Func_0808a010(30);
    Func_0808a128(18, 6, 0);
    Func_0808a010(30);

    Func_0808a0d0(18, 216, 392);
    Func_0808a010(10);
    Func_0808a1b8(18, 0x4000, 20);
    Func_080770c8(0x858);
    Func_0808a020();
}
