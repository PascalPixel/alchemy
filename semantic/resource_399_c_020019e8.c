#include "types.h"

/*
 * Long scripted formation for actors 0, 3, 19 and 20. The body is linear
 * apart from the null check around actor 0's position copy. A branch skips the
 * fourteen-word literal pool at 0x02001df4-0x02001e2b and continues in the
 * same frame at 0x02001e2c.
 *
 * The owner begins at push {r5,r6,r7,lr} at 0x020019e8, returns at
 * 0x02001f12, and owns the four trailing literal words through 0x02001f23.
 * The next owner begins at 0x02001f24, making the complete span 1,340 bytes.
 */

extern u8 *Data_03001ebc;

extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 actor, s32 x, s32 z);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a170(s32 message);
extern void Func_0808a188(s32 actor, s32 target, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 value);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 *Func_0808a228(void);
extern void Func_0808a360(void);

void Func_020019e8(void)
{
    u8 *actor;
    u8 *shared;

    Func_0808a018();
    Func_0808a1b8(3, 0xa000, 0);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0c8(0, 0x2b2, 200);
    Func_0808a228()[0x55] = 0;
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x02b20000, 0, 0x00a40000, 1);

    shared = Data_03001ebc;
    *(s32 *)(shared + 448) = 256;
    *(s32 *)(shared + 456) = 48;
    Func_0808a360();
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    Func_0808a090(3, 0x9999, 0x4ccc);
    actor = Func_0808a080(0);
    if (actor != (void *)0) {
        Func_0808a0f0(3, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a0d0(3, 0x2a1, 183);
    Func_0808a1b8(3, 0xc000, 0);
    Func_0808a130(19, 2);
    Func_0808a138(20, 2);
    Func_0808a010(40);
    Func_0808a170(0x165b);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(3, 0xe000, 40);
    Func_0808a110(3, 3);
    Func_0808a1f0(20, 0x102);
    Func_0808a010(20);
    Func_0808a188(0x4014, 0, 10);
    Func_0808a1b8(3, 0xa000, 40);
    Func_0808a110(3, 4);
    Func_0808a188(0x2003, 0, 10);
    Func_0808a110(20, 3);
    Func_0808a010(20);
    Func_0808a1f0(19, 0x102);
    Func_0808a010(20);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(0, 0xa000, 0);
    Func_0808a1b8(3, 0xf000, 10);
    Func_0808a1b8(3, 0x2000, 40);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(3, 0xc000, 40);
    Func_0808a138(20, 2);
    Func_0808a010(20);
    Func_0808a188(0x4014, 0, 20);
    Func_0808a1b8(3, 0xa000, 20);
    Func_0808a110(3, 3);
    Func_0808a010(60);
    Func_0808a1e8(3, 0x105, 60);
    Func_0808a1e8(19, 0x101, 0);
    Func_0808a1e8(20, 0x101, 60);
    Func_0808a138(19, 1);
    Func_0808a010(20);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(3, 0xe000, 40);
    Func_0808a1b8(3, 0xa000, 40);
    Func_0808a1b8(3, 0xe000, 20);
    Func_0808a1b8(3, 0x6000, 80);
    Func_0808a188(0x2003, 0, 20);
    Func_0808a1b8(20, 0xf000, 0);
    Func_0808a1b8(19, 0x7000, 40);
    Func_0808a1b8(19, 0x5000, 0);
    Func_0808a1b8(20, 0x3000, 20);
    Func_0808a090(20, 0x10000, 0x8000);
    Func_0808a080(20)[0x5a] &= 0xfe;
    Func_0808a0d0(20, 0x290, 166);
    Func_0808a010(1);
    Func_0808a080(20)[0x5a] |= 1;
    Func_0808a010(20);
    Func_0808a188(0x4014, 0, 10);
    Func_0808a138(3, 2);
    Func_0808a010(40);
    Func_0808a1b8(3, 0xa000, 10);
    Func_0808a188(0x2003, 0, 40);
    Func_0808a1b8(3, 0x2000, 20);
    Func_0808a188(0x4003, 0, 10);
    Func_0808a1f0(19, 0x102);
    Func_0808a1f0(20, 0x102);
    Func_0808a010(40);
    Func_0808a1b8(3, 0xc000, 20);
    Func_0808a100(3, 4);
    Func_0808a188(0x2003, 0, 20);
    Func_0808a138(19, 1);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(3, 0x2000, 40);
    Func_0808a1b8(3, 0xc000, 20);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a138(20, 1);
    Func_0808a188(0x4014, 0, 20);
    Func_0808a138(3, 1);
    Func_0808a010(20);
    Func_0808a1b8(3, 0xa000, 20);
    Func_0808a110(3, 3);
    Func_0808a188(0x2003, 0, 80);
    Func_0808a1e8(19, 0x105, 0);
    Func_0808a1e8(20, 0x105, 60);
    Func_0808a110(19, 4);
    Func_0808a188(19, 0, 10);
    Func_0808a100(20, 4);
    Func_0808a188(0x4014, 0, 20);
    Func_0808a1e8(3, 0x102, 60);
    Func_0808a188(0x2003, 0, 40);
    Func_0808a110(19, 3);
    Func_0808a188(19, 0, 10);
    Func_0808a1b8(3, 0xe000, 20);
    Func_0808a110(20, 3);
    Func_0808a188(0x4014, 0, 10);
    Func_0808a1b8(3, 0xa000, 60);
    Func_0808a1b8(3, 0xe000, 20);
    Func_0808a1b8(3, 0xa000, 20);
    Func_0808a1b8(3, 0xc000, 40);
    Func_0808a110(3, 3);
    Func_0808a188(0x2003, 0, 10);
    Func_0808a100(19, 3);
    Func_0808a110(20, 3);
    Func_0808a010(40);
    Func_0808a1b8(3, 0x2000, 20);
    Func_0808a188(0x4003, 0, 20);
    Func_0808a1b8(0, 0xa000, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a0d0(3, 0x2b0, 200);
    Func_0808a0f0(3, 0, 0);
    Func_0808a080(20)[0x5a] &= 0xa1;
    Func_0808a0d0(20, 0x284, 166);
    Func_0808a010(1);
    Func_0808a080(20)[0x5a] |= 1;
    Func_080770c8(0x82e);
    *(s32 *)(shared + 448) = 521;
    Func_080770d0(0x82d);
    Func_0808a020();
}
