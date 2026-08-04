#include "types.h"

/*
 * Flag-gated actor-8 scene at 0x02001258, complete 168-byte span through its
 * sole return and three-word pool before 0x02001300.  Both early gates, the
 * callback installation and all sixteen ordered calls are explicit.
 */

extern void Func_02000cf8(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 cue);

void Func_02001258(void)
{
    u8 *record;

    if (Func_080770c0(0x200) == 0)
        return;
    if (Func_080770c0(0x201) != 0)
        return;

    Func_080770c8(0x201);
    Func_080770c8(0x302);
    Func_0808a018();
    Func_0808a1f0(8, 0x102);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_0808a090(8, 0x20000, 0x10000);
    Func_0808a0d0(8, 760, 280);
    Func_0808a0d0(8, 760, 312);
    Func_0808a0d0(8, 792, 312);
    Func_0808a010(10);
    Func_0808a1b8(8, 0xc000, 20);
    record = Func_0808a080(8);
    *(void (**)(void))(record + 0x6c) = Func_02000cf8;
    Func_0808a020();
}
