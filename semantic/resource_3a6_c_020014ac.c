#include "types.h"

/* Actor-0/9 staged scene, complete 288-byte owner through pool at 0x15c4. */

extern void Func_02000cd0(void *record);
extern void Func_02000528();
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0e0(s32 actor, s32 value, s32 mode);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_080f9010(s32 cue);

void Func_020014ac(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a010(10);
    Func_0808a090(0, 0x8000, 0x1999);
    Func_0808a100(0, 8);
    Func_0808a010(15);
    Func_0808a0e0(0, 8, 0);
    Func_0808a010(4);
    Func_080f9010(0x120);
    Func_080f9010(0xef);

    Func_0808a090(9, 0x8000, 0x1999);
    Func_0808a100(9, 2);
    record = Func_0808a080(9);
    record[0x55] = 0;
    record = Func_0808a080(9);
    *(s32 *)(record + 0x44) = 0;
    Func_0808a0e0(9, 12, 0);
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    Func_0808a0e8(9);
    Func_080f9010(0x120);
    Func_080f9010(0xd5);
    Func_0808a100(9, 3);
    record = Func_0808a080(9);
    Func_0808a0e0(9, 6, 0);
    record = Func_0808a080(9);
    record[0x55] = 3;
    Func_02000cd0(record);
    Func_0808a100(9, 8);
    Func_0808a1e0(9, 3);
    record = Func_0808a080(9);
    record[0x23] = 2;

    Func_02000528(0, 12, 16, 1, 4, 0);
    Func_02000528(0, 13, 16, 1, 4, 0);
    Func_080770c8(0x202);
    Func_080f9010(0xf0);
    Func_0808a020();
}
