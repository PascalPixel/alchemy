#include "types.h"

/*
 * resource_3b1 owner at 0x02001b34, 1,032 bytes.  The owner starts with the
 * saved-register prologue at 0x02001b34 and returns through the single
 * epilogue (pop {r3,r5,r6}, restore r8/r9/sl, pop {r3}, restore fp,
 * pop {r5,r6}, pop {r0}, bx r0) at 0x02001eea-0x02001efa.  The sixteen-word
 * literal pool at 0x02001efc-0x02001f3b is part of this owner; the next
 * saved-register prologue begins at 0x02001f3c.
 *
 * The 0x911 gate is the only early exit.  All 107 calls in the body resolve
 * either to the established resource_3b1 helper owners at 0x0200486c,
 * 0x02004880 and 0x020048ac or to the imported overlay veneers.  The two
 * Func_0808a080 calls return the slot-zero record used by the byte updates at
 * offset 0x5a; Func_0808a0a0 is the separate one-argument operation at the
 * end of the body.  Keeping those effects explicit makes this a readable
 * semantic reconstruction without introducing an assembly escape hatch.
 */

extern void Func_0200486c(s32 id);
extern void Func_02004880(s32 id, u16 value);
extern void Func_020048ac(s32 a, s32 b, s32 c, u32 flags);

extern s32 Func_080770c0(s32 flag);
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 amount);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 value, s32 limit);
extern void Func_0808a098(s32 id, const void *table);
extern void Func_0808a0a0(s32 id);
extern void Func_0808a0d0(s32 id, s32 x, s32 y);
extern void Func_0808a100(s32 id, s32 value);
extern void Func_0808a110(s32 id, s32 value);
extern void Func_0808a128(s32 id, s32 value, s32 duration);
extern void Func_0808a130(s32 id, s32 value);
extern void Func_0808a138(s32 id, s32 value);
extern void Func_0808a170(s32 message);
extern void Func_0808a188(s32 id, s32 value, s32 duration);
extern void Func_0808a1b8(s32 id, s32 value, s32 duration);
extern void Func_0808a1e8(s32 id, s32 value, s32 duration);
extern void Func_0808a1f0(s32 id, s32 value);
extern void Func_0808a208(s32 left, s32 right);
extern void Func_0808a460(void);

void Func_02001b34(void)
{
    u8 *record;

    if (Func_080770c0(0x911) == 0)
        return;

    Func_0808a018();
    Func_0808a460();
    Func_0808a208(0x26666, 0x4ccc);
    Func_020048ac(0x05b70000, -1, 0xe8 << 17, 0x10000014u);
    Func_0808a138(13, 1);
    Func_0808a170(0x1d56);

    Func_0200486c(0x200d);
    Func_02004880(12, 0xd000);
    Func_0808a1e8(12, 0x102, 20);
    Func_0808a130(12, 2);
    Func_0200486c(0x800c);
    Func_0808a138(14, 1);
    Func_0808a188(0xa00e, 0, 20);
    Func_02004880(12, 0xd000);
    Func_0808a1e8(12, 0x102, 40);
    Func_0808a1e8(14, 0x103, 40);
    Func_0808a130(14, 3);
    Func_0200486c(0xa00e);
    Func_0808a1f0(12, 0x102);
    Func_0808a010(40);
    Func_0808a130(12, 3);
    Func_0200486c(0x800c);
    Func_0808a138(14, 1);
    Func_0200486c(0xa00e);
    Func_02004880(14, 0xb000);
    Func_0200486c(0xa00e);
    Func_02004880(12, 0xd000);
    Func_0808a1e8(12, 0x102, 30);
    Func_0808a130(12, 4);
    Func_0200486c(0x800c);
    Func_0808a110(13, 4);
    Func_0200486c(0x200d);
    Func_0808a130(13, 2);
    Func_0200486c(0x800c);
    Func_0808a100(13, 4);
    Func_0200486c(0xa00e);
    Func_0808a110(13, 4);
    Func_0200486c(0x800c);
    Func_02004880(14, 0x8000);
    Func_0808a130(14, 2);

    Func_0808a188(0xa00e, 0, 20);
    Func_0808a1b8(12, 0, 0);
    Func_0808a1e8(12, 0x102, 80);
    Func_0808a188(0x800c, 0, 20);
    Func_0808a1e8(14, 0x103, 0);
    Func_0808a1e8(13, 0x103, 60);
    Func_0808a130(14, 2);
    Func_0200486c(0xa00e);
    Func_02004880(14, 0xb000);
    Func_0808a138(14, 1);
    Func_0200486c(0xa00e);
    Func_02004880(13, 0xc000);
    Func_0808a1e8(13, 0x101, 0);
    Func_0808a1e8(12, 0x101, 60);
    Func_0808a138(13, 1);
    Func_0200486c(0xa00e);
    Func_0808a1e8(14, 0x103, 40);
    Func_0808a138(14, 1);
    Func_0200486c(0xa00e);
    Func_0808a1b8(12, 0xd000, 0);
    Func_0808a1b8(13, 0x5000, 40);
    Func_0808a1b8(12, 0, 0);
    Func_02004880(13, 0xc000);
    Func_0808a138(12, 2);
    Func_0808a188(0x800c, 0, 20);
    Func_0808a1b8(14, 0x4000, 40);
    Func_0200486c(0xa00e);
    Func_0808a130(12, 2);
    Func_0808a138(13, 2);
    Func_0808a010(60);
    Func_0808a138(13, 1);
    Func_0200486c(13);
    Func_0808a110(14, 3);
    Func_0200486c(0x800c);
    Func_0808a1e8(12, 0x102, 40);
    Func_0808a138(13, 2);
    Func_0200486c(0x800c);
    Func_0808a110(13, 3);
    Func_0200486c(13);
    Func_0808a1b8(14, 0xb000, 40);
    Func_0808a100(14, 3);
    Func_0808a110(13, 3);
    Func_0808a090(14, 0x19999, 0xcccc);
    Func_0808a090(13, 0x19999, 0xcccc);
    Func_0808a098(14, (const void *)0x0200e6a8);
    Func_0808a098(13, (const void *)0x0200e6a8);
    Func_0808a010(20);
    Func_0808a1b8(12, 0x4000, 0);
    Func_0808a090(0, 0x26666, 0x13333);

    record = Func_0808a080(0);
    record[0x5a] &= 0xfe;
    Func_0808a0d0(0, 0xb8 << 2, 0x82 << 2);
    Func_0808a010(1);
    record = Func_0808a080(0);
    record[0x5a] |= 1;

    Func_0808a1b8(0, 0x8000, 20);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a128(12, 4, 20);
    Func_0808a1b8(0, 0xa000, 20);
    Func_0808a130(12, 2);
    Func_0200486c(12);
    Func_0808a090(12, 0x19999, 0xcccc);
    Func_0808a098(12, (const void *)0x0200e6a8);
    Func_0808a010(40);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a0a0(12);
    Func_080770c8(0x922);
    Func_0808a020();
}
