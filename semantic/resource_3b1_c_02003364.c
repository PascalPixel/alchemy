#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x02003364 (440 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` + `r8` spill at 0x02003364, epilogue `pop {r3} / mov
 * r8,r3 / pop {r5,r6,r7} / pop {r0} / bx r0` at
 * 0x020034f2-0x020034fa. The trailing pool at 0x020034fc-0x02003518
 * (0x00000928, 0x00000266, 0x00000925, 0x00000921, 0x000001db,
 * 0x00000256, 0x0000026a, 0x00000293) ends exactly where the next
 * owner's `push {r5,r6,lr}` begins (0x0200351c, already this overlay's
 * row `0x0200351c | 1 call`), so the span is 0x02003364-0x0200351c,
 * 440 bytes.
 *
 * A `Value_XXXXXXXX`-gated cascade (same low-address family used
 * throughout this overlay): an early guard call bails out entirely if
 * unset, then an installed-object setup block (writing two handler
 * pointers, `0x0200812d`/`0x02008285`, into `+0x6c` fields — the same
 * "veneer address + 1" idiom documented for `Func_0200012c` in
 * assets/code/resource_3b1_c_0200012c.c) either runs and returns, or
 * falls through to a three-way else-if chain of simpler setter pairs,
 * the last of which is unconditional-once-reached (no further gate).
 *
 * Raw callee naming.
 */

extern u8 Value_00000911;
extern u8 Value_00000928;
extern u8 Value_0000093e;
extern u8 Value_00000109;
extern u8 Value_00000925;
extern u8 Value_00000921;

s32 Func_080770c0();
void Func_020045d0(void);
u8 *Func_0808a080();
void Func_020048e8();
void Func_02004890();
void Func_0808a090();

void Func_02003364(void)
{
    s32 tailX;
    s32 tailY;
    s32 tailZ;

    if (Func_080770c0((s32)&Value_00000911) == 0) {
        Func_020045d0();
        return;
    }

    if (Func_080770c0((s32)&Value_00000928) != 0) {
        Func_020045d0();
    }

    {
        s32 guard = Func_080770c0((s32)&Value_0000093e);
        if (guard != 0) {
            return;
        }

        if (Func_080770c0(0x8a0) != 0) {
            u8 *obj = Func_0808a080(9);
            u8 *obj2;

            Func_020048e8(13, 0, 0);
            Func_02004890(8, 0x1c8, 0x28c, 0);
            Func_02004890(9, 0x1e0, 0x258, 0xb000);
            Func_0808a090(9, 0xcccc, 0x6666);
            *(u16 *)(obj + 0x66) = (u16)guard;
            obj[0x63] = 0;
            obj[0x59] |= 0x80;
            *(u32 *)(obj + 0x6c) = 0x0200812d;

            obj2 = Func_0808a080(8);
            obj2[0x62] = 0;
            *(u32 *)(obj2 + 0x6c) = 0x02008285;

            if (Func_080770c0((s32)&Value_00000109) != 0) {
                Func_02004890(0, 0x1e0, 0x29a, 0xa000);
            }
            return;
        }
    }

    if (Func_080770c0((s32)&Value_00000928) != 0) {
        tailX = 0x1bc;
        tailY = 0x266;
        tailZ = 0xd000;
        goto simple_tail;
    }

    if (Func_080770c0((s32)&Value_00000925) != 0) {
        tailX = 0x1c8;
        tailY = 0x288;
        tailZ = 0;
simple_tail:
        Func_02004890(8, tailX, tailY, tailZ);
        Func_020048e8(13, 0, 0);
        return;
    }

    if (Func_080770c0((s32)&Value_00000921) != 0) {
        u8 *obj3;
        u8 *obj4;

        Func_02004890(8, 0x1db, 0x256, 0x8000);
        Func_02004890(9, 0x1ce, 0x26a, 0xb000);

        obj3 = Func_0808a080(12);
        *(u16 *)(obj3 + 6) = 0x3000;

        obj4 = Func_0808a080(11);
        *(u16 *)(obj4 + 6) = 0xb000;

        Func_02004890(13, 0x1b6, 0x293, 0xd000);
        Func_02004890(10, 0x1e8, 0x2b0, 0xb000);
    }
}
