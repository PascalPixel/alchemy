#include "types.h"

/*
 * Resource 3b1 scene owner at 0x020038ac (1,124 bytes).
 *
 * The saved-register prologue is the two-stage high-register form at
 * 0x020038ac-0x020038ba.  All reachable paths join at the final branch to
 * 0x02003cec; the interworking epilogue is the pop/mov sequence at
 * 0x02003cfc-0x02003d0a.  The word at 0x02003d0c is the trailing literal
 * 0x00000926, and the next independent push {r5,lr} begins at 0x02003d10.
 * The complete owner therefore occupies 0x020038ac-0x02003d0f (0x464,
 * 1,124 bytes).
 *
 * There is one branch-skipped literal pool at 0x02003cce-0x02003cea: an
 * alignment halfword at 0x02003cce followed by seven words containing the
 * overlay-local descriptor values 0x01000001, 0x03001ebc, 0x0000cccc,
 * 0x00006666, 0x00001e27, 0x00000103 and 0x00000101.  The final 0x00000926
 * word follows the epilogue.  All 112 resolver-confirmed BL sites are kept
 * in machine order below.  The three actor-availability branches correspond
 * to the three getter calls at 0x020039c0, 0x020039f4 and 0x02003a2a.
 */

extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a0c8(s32 actor, s32 x, s32 y);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a170(s32 value);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a360(void);

/* Local scene helpers already used by the neighbouring resource_3b1 owners. */
extern void Func_0200486c(s32 id);
extern void Func_02004880(s32 id, s32 value);
extern void Func_02004890(s32 id, s32 x, s32 y, s32 mode);
extern void Func_020048ac(s32 x, s32 y, s32 z, u32 flags);
extern void Func_020048e8(s32 id, s32 arg1, s32 arg2);

void Func_020038ac(void)
{
    u8 *actor;

    Func_0808a018();
    Func_020048e8(25, 0, 0);
    Func_020048e8(24, 1, 0);

    Func_020048ac(0xdc << 17, -1, 0xa8 << 16, 0x01000001u);
    Func_02004890(27, 0xdc << 1, 0xa4, 0xa0 << 7);
    Func_02004890(8, 0xd6 << 1, 0xbe, 0xd0 << 8);
    Func_02004890(9, 0xe2 << 1, 0xbe, 0xb0 << 8);
    Func_0808a100(9, 1);
    Func_02004890(0, 0xdc << 1, 0x86, 0x80 << 8);
    Func_0808a360();

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 0xcc << 1, 0x86);
    Func_0808a0d0(0, 0xcc << 1, 0x94);
    Func_0808a0d0(0, 0xd4 << 1, 0x94);
    Func_0808a1b8(0, 0x80 << 7, 20);
    Func_0808a138(27, 1);
    Func_0808a170(0x1e27);
    Func_0200486c(27);
    Func_0808a138(8, 1);
    Func_0200486c(8);
    Func_0808a110(27, 3);
    Func_0200486c(27);
    Func_02004880(27, 0xd0 << 8);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(1, 0xcccc, 0x6666);
        Func_0808a090(1, 0xcccc, 0x6666);
        Func_0808a0d0(1, 0xdc << 1, 0x94);
        Func_0808a1b8(1, 0x80 << 7, 0);
    }

    actor = Func_0808a080(1);
    if (actor != 0) {
        Func_0808a0f0(2, 0xcccc, 0x6666);
        Func_0808a090(2, 0xcccc, 0x6666);
        Func_0808a0d0(2, 0xe4 << 1, 0x94);
        Func_0808a1b8(2, 0x80 << 7, 0);
    }

    actor = Func_0808a080(2);
    if (actor != 0) {
        Func_0808a0f0(3, 0xcccc, 0x6666);
        Func_0808a090(3, 0xcccc, 0x6666);
        Func_0808a0d0(3, 0xec << 1, 0x94);
        Func_0808a1b8(3, 0x80 << 7, 20);
    }

    Func_020048e8(0, 0, 60);
    Func_020048e8(1, 0x80 << 7, 60);
    Func_020048e8(27, 0xa0 << 7, 60);
    Func_0808a1b8(27, 0xa0 << 7, 60);
    Func_0200486c(27);
    Func_0808a130(9, 1);
    Func_0808a1e8(9, 0x100, 40);
    Func_0200486c(9);
    Func_0808a130(1, 3);
    Func_0808a1e8(1, 0x103, 60);
    Func_0808a110(27, 3);
    Func_0200486c(27);
    Func_0808a138(10, 1);
    Func_0808a100(10, 3);
    Func_0200486c(10);
    Func_0808a100(8, 3);
    Func_0808a100(9, 3);
    Func_0808a100(11, 3);
    Func_0808a100(12, 3);
    Func_0808a110(13, 3);

    Func_020048e8(0, 0, 40);
    Func_020048e8(2, 1, 0);
    Func_020048e8(1, 0x80 << 7, 20);
    Func_0808a110(27, 4);
    Func_0200486c(27);
    Func_0808a1e8(8, 0x81 << 1, 60);
    Func_0808a130(8, 1);
    Func_0200486c(8);
    Func_0808a110(27, 3);
    Func_0200486c(27);
    Func_0808a1b8(8, 0, 0);
    Func_0808a1b8(9, 0x80 << 8, 40);
    Func_0808a1e8(8, 0x81 << 1, 0);
    Func_0808a1e8(8, 0x81 << 1, 40);
    Func_0808a138(27, 1);
    Func_0808a100(27, 3);
    Func_0808a188(27, 0, 20);
    Func_0808a100(9, 3);
    Func_0808a110(9, 3);
    Func_0808a010(40);
    Func_0808a1e8(9, 0x100, 20);
    Func_02004880(9, 0xb0 << 8);
    Func_0200486c(9);
    Func_02004880(27, 0xc0 << 6);
    Func_0808a1e8(27, 0xc0 << 6, 60);
    Func_0808a188(27, 0, 60);
    Func_0808a1e8(27, 0x81 << 1, 20);
    Func_02004880(27, 0xb0 << 8);
    Func_0808a110(27, 3);
    Func_0200486c(27);

    Func_020048e8(3, 2, 80);
    Func_02004880(8, 0xd0 << 8);
    Func_0808a130(8, 2);
    Func_0200486c(8);
    Func_0808a110(9, 3);
    Func_0808a130(9, 2);
    Func_0200486c(9);
    Func_02004880(27, 0xa0 << 7);
    Func_0808a110(27, 3);
    Func_0808a138(27, 1);
    Func_0200486c(27);
    Func_0808a090(27, 0xcccc, 0x6666);
    Func_0808a0d0(27, 0xcc << 1, 0x9e);
    Func_0808a0d0(27, 0xcc << 1, 0x94);
    Func_0808a1b8(27, 0, 20);
    Func_0808a138(27, 1);
    Func_0200486c(27);
    Func_020048e8(1, 0x80 << 8, 20);
    Func_020048e8(2, 1, 0);
    Func_0808a0d0(27, 0xcc << 1, 0x86);
    Func_0808a0c8(27, 0xdc << 1, 0x86);
    Func_0808a010(40);

    Func_020048e8(9, 10, 0);
    Func_080770c8(0x926);
}
