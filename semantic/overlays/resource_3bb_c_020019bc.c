typedef int s32;

/*
 * resource_3bb owner at 0x020019bc, 372 bytes.  The owner starts with
 * push {r5,r6,r7,lr} and the r8 spill at 0x020019bc, returns through
 * pop {r3}/restore r8/pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02001b1a-0x02001b22,
 * and owns the three literal words at 0x02001b24-0x02001b2f.  The next
 * saved-register prologue is at 0x02001b30, so the pool is included in the
 * 0x174-byte owner span.
 *
 * The first branch is the shared status-cell value 2 path, which delegates
 * to the already-described 0x02001df4 owner and then returns.  Otherwise the
 * owner prepares the requested record, performs the long setup sequence when
 * its 0x02001ffc result is zero, always runs the 0x020020b8 follow-up, and
 * conditionally emits the 0x20ad notification when that result is one.  The
 * common 0x02002114 and 0x0808a020 tail is kept after those branches so the
 * static call order remains the machine order (32 resolved sites).
 *
 * All imported names are address identities recovered from the overlay's
 * resolver table.  The fixed-point constants and the two pool values are
 * written directly as ordinary C expressions; no inline assembly or
 * fixed-register construct is used.
 */

extern void Func_02001df4(s32 arg0);
extern s32 Func_02001ffc(s32 arg0, s32 arg1);
extern void Func_02001898(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020020b8(s32 arg0, s32 arg1);
extern void Func_02002114(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02002ba8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02002d84(s32 arg0);
extern void Func_020030e8(s32 arg0, s32 arg1, s32 arg2);

extern void Func_0808a010(s32 amount);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a090(s32 id, s32 value, s32 limit);
extern void Func_0808a0f0(s32 id, s32 x, s32 y);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 id, s32 value);
extern void Func_0808a1b8(s32 id, s32 value, s32 duration);
extern void Func_0808a1e8(s32 id, s32 value, s32 duration);
extern void Func_0808a200(s32 id, s32 value);
extern void Func_0808a208(s32 x, s32 y);
extern void Func_0808a210(s32 a, s32 b, s32 c, s32 d);
extern void Func_0808a218(void);

void Func_020019bc(s32 arg0)
{
    s32 result;

    if (*(short *)(0x02000240 + 0x1c2) == 2) {
        Func_02001df4(arg0);
        return;
    }

    Func_0808a018();
    result = Func_02001ffc(arg0, 5);

    if (result == 0) {
        Func_0808a170(0x20ae);
        Func_0808a208(0x20000, 0x4000);
        Func_0808a210(0xa4 << 17, -1, 0x84 << 17, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a208(0xc0 << 9, 0xc0 << 6);
        Func_0808a210(0x9c << 17, -1, 0xb0 << 16, 1);
        Func_0808a218();
        Func_0808a180(arg0, 0);
        Func_02002ba8(0, 0xcc << 1, 0x84 << 1);
        Func_0808a090(0, 0xc0 << 9, 0xc0 << 8);
        Func_020030e8(0, 0xcc << 1, 0xd8);
        Func_0808a1b8(0, 0x80 << 8, 10);
        Func_0808a180(arg0, 0);
        Func_02001898(16, 0xb4 << 1, 0xd0);
        Func_0808a1e8(0, 0x84 << 1, 45);
        Func_0808a090(0, 0xc0 << 9, 0xc0 << 8);
        Func_020030e8(0, (0xcc << 1) - 32, 0xd8);
        Func_020030e8(0, (0xcc << 1) - 32, 0xf8);
        Func_020030e8(0, 0x9c << 1, 0xf8);
        Func_0808a180(arg0, 0);
        Func_02002d84(0);
        Func_0808a200(0, 0);
        Func_0808a0f0(16, 0xc4 << 17, 0xd0 << 16);
    }

    Func_020020b8(arg0, 5);

    if (result == 1) {
        Func_0808a170(0x20ad);
        Func_0808a180(arg0, 0);
    }

    Func_02002114(result, arg0, 5);
    Func_0808a020();
}
