#include "types.h"

/*
 * Resource 371 owner at 0x02001ca4 (1488 bytes, 133 distinct call targets).
 *
 * Complete owner: the two-stage prologue at 0x02001ca4 (`push {r5, r6, r7,
 * lr}`, r9/sl/fp copied into r5/r6/r7 and pushed, r8 copied into r7 and
 * pushed) and its mirror at 0x0200223c ending in `pop {r0} ; bx r0`.  r0
 * holds the popped return address, so the owner returns nothing.
 * 0x02001ca4 + 1488 = 0x02002274, exactly where the next owner starts.
 * Three literal pools sit inside the span and are branched or fallen over:
 * 0x02001e2a-0x02001e47, 0x02001f6a-0x02001f73 and
 * 0x0200224c-0x02002273.
 *
 * This owner is installed as a step handler by the spawn-rule table: its
 * first entry's handler word is 0x02009ca5, which under this overlay's
 * proven 0x02008000 link base is this address plus the Thumb bit (see
 * resource_371_c_02002768.c, which patches that same table).
 *
 * 143 `bl` sites reach 133 distinct targets, matching the inventory count
 * exactly.  Ten targets are reached twice, and eight of those ten are reached
 * *with different argument counts and different result usage* — 0x02006384 is
 * both `(0, 22)` and a one-argument object lookup, 0x020062cc is both a
 * four-argument placement and a two-argument predicate, and 0x0200639e,
 * 0x020063a8, 0x020063b6, 0x02006452, 0x020064d0, 0x02006544 and
 * 0x02006338 each appear under two shapes.  That is why the declarations
 * below are old-style: they are load-time-relocated import slots, not
 * functions with known interfaces, and the same numeric name can stand for
 * two different callees.
 *
 * Geometry: `v + K ; lsr #31 ; add ; asr #1` is signed division by two, so
 * the two coordinates computed up front are the midpoints between the
 * reference object's position and the fixed point (0x15d00000, 0x05300000).
 * The pool holds the negated constants 0xea300000 and 0xfad00000.  Those
 * midpoints are later passed whole (0x02001f30, 0x020020a6) and as
 * `>> 16` integers (0x02001eb0, 0x020021a2).
 *
 * Four counted loops all use the same shape — `subs r5,#1` before the
 * per-frame call and `bge` after it — so they run one more time than the
 * initial value suggests: 16, 60, 16 and 8 iterations.
 *
 * The two arms are selected by 0x02005f7c(366).  The zero arm builds the
 * whole scene; the non-zero arm is the shorter resume path.  Both converge on
 * 0x02003db0 / 0x02006610, except for the 0x020021b0 tail, which is reached
 * only from the resume arm and returns without them.
 *
 * The field offsets used on the actor (+0x06 facing, +0x14, +0x18/+0x1c
 * amplitude pair, +0x28, +0x30/+0x34, +0x48, +0x55 flag, +0x66, +0x6c step
 * callback) are the layout proven by the byte-exact siblings
 * `assets/code/resource_371_c_020004a0.c` and
 * `assets/code/resource_371_c_02000250.c`; +0x6c is cleared to 0 at
 * 0x02001dea, which removes the actor's step callback.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).
 */

extern u32 Data_03001e40;

void Func_02001c08();
void Func_02004058();
void Func_080000c0();
s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();
void Func_08077260();
void Func_080771a8();
void Func_0808a018();
void Func_080771c8();
void Func_0808a010();
void Func_08009240();
void Func_0808a0f0();
void Func_0808a148();
void Func_0808a170();
void Func_0808a1e8();
void Func_0808a180();
void Func_08015120();
void Func_0808a3d8();
void Func_0808a3e0();
void Func_0808a0d0();
void Func_0808a138();
void Func_0808a5c0();
void Func_0808a5c8();
void Func_0808a128();
void Func_0808a428();
void Func_080f9010();
void Func_08015040();
s32 Func_0808a070();
void Func_08009150();
void Func_0808a440();
void Func_0808a178();
void Func_0808a100();
void Func_080153e8();
void Func_080770d0();
void Func_0808a020();
void Func_0808a110();
void Func_080a1040();
void Func_0808a4f8();

void Func_02001ca4(void)
{
    u8 *actor;
    u8 *reference;
    u8 *other;
    u8 *flag;
    s32 x;
    s32 z;
    s32 count;
    s32 step;
    s32 alternate_tail;

    actor = Func_0808a080(8);
    reference = Func_0808a080(0);

    /* Midpoints between the reference object and (0x15d00000, 0x05300000).
     * The pooled constants are the negatives, 0xea300000 and 0xfad00000. */
    x = (*(s32 *)(reference + 0x08) - 0x15d00000) / 2 + 0x15d00000;
    z = (*(s32 *)(reference + 0x10) - 0x05300000) / 2 + 0x05300000;

    alternate_tail = 0;

    if (Func_080770c0(366) == 0) {
        Func_08077260(1);
        Func_080770c8(366);
        Func_0808a018();

        other = Func_0808a080(0);
        if (other != 0) {
            Func_0808a0f0(8, *(s32 *)(other + 0x08), *(s32 *)(other + 0x10));
        }

        Func_080771a8(0, 0, 0);
        Func_080771c8(0, 0, 0);
        Func_0808a3d8();
        Func_0808a148(0, 8, 0);
        Func_0808a010(10);
        Func_0808a1e8(0, 0x101, 60);

        *(u16 *)(actor + 0x66) = 1;

        Func_0808a148(8, 0, 0);
        Func_080000c0(16);
        Func_0808a170(0xc4f);
        Func_0808a180(8, 0);
        Func_0808a3e0();
        Func_0808a5c0(0x13333, 6);
        Func_0808a5c8();
        Func_0808a3d8();

        flag = actor + 0x55;
        *flag = 2;
        *(s32 *)(actor + 0x48) = 0x4000;
        *(s32 *)(actor + 0x30) = 0x10000;
        *(s32 *)(actor + 0x34) = 0x10000;
        *(s32 *)(actor + 0x28) = 0;
        *(s32 *)(actor + 0x14) = 0;

        Func_08009150(actor, 0x15d00000, 0, 0x05300000);

        count = 15;
        do {
            *(s32 *)(actor + 0x18) += 0x800;
            *(s32 *)(actor + 0x1c) += 0x800;
            count--;
            Func_080000c0(1);
        } while (count >= 0);

        Func_0808a148(8, 0, 0);
        Func_0808a148(0, 8, 0);
        Func_080000c0(16);

        *(s32 *)(actor + 0x6c) = 0;

        Func_08009240(actor, 0);
        *(s32 *)(actor + 0x48) = 0x10000;
        Func_0808a180(8, 0);
        Func_080f9010(131);
        Func_0808a428(140, 0);

        count = 59;
        do {
            if ((Data_03001e40 & 2) != 0) {
                Func_08009240(actor, 7);
            } else {
                Func_08009240(actor, 0);
            }
            if ((Data_03001e40 & 0xf) == 0) {
                Func_02004058(actor);
            }
            count--;
            Func_080000c0(1);
        } while (count >= 0);

        Func_0808a440();
        Func_08009240(actor, 0);
        Func_0808a138(8, 2);
        Func_0808a180(8, 0);
        Func_0808a1e8(0, 258, 30);
        Func_0808a180(8, 0);
        Func_0808a1e8(0, 0x101, 30);
        Func_0808a0d0(8, x >> 16, z >> 16);
        Func_0808a100(0, 22);
        Func_0808a180(8, 0);
        Func_0808a1e8(0, 0x101, 40);
        Func_0808a128(8, 4, 30);
        Func_08015120(300, 4);
        Func_0808a180(8, 0);
        Func_0808a1e8(0, 256, 30);
        Func_0808a180(8, 0);
        Func_0808a138(0, 2);
        Func_0808a180(8, 0);
        Func_0808a128(8, 2, 30);
        Func_0808a180(8, 0);

        *flag = 0;
        Func_08009150(actor, x, 0x100000, z);

        count = 15;
        do {
            *(u16 *)(actor + 0x06) = (u16)(*(u16 *)(actor + 0x06) + 0x1000);
            count--;
            Func_080000c0(1);
        } while (count >= 0);

        Func_0808a100(0, 1);
        Func_0808a180(8, 0);

        *flag = 2;
        *(s32 *)(actor + 0x28) = 0;
        *(s32 *)(actor + 0x14) = 0;

        count = 7;
        do {
            *(u16 *)(actor + 0x06) = (u16)(*(u16 *)(actor + 0x06) + 0x1000);
            count--;
            Func_080000c0(1);
        } while (count >= 0);

        Func_0808a100(0, 22);
        Func_0808a180(8, 0);
        Func_0808a1e8(8, 258, 30);
        Func_0808a148(8, 0, 0);
        Func_0808a138(8, 2);
        Func_0808a180(8, 0);
        Func_0808a128(8, 2, 30);
        Func_0808a178(8, 0);

        step = 0;
loop_test:
        if (Func_0808a070(0, 0) != 1) {
            goto loop_failed;
        }
        Func_0808a128(8, 2, 20);
        Func_0808a128(8, 2, 20);
        if (step == 6) {
            Func_0808a170(0xc62);
            Func_0808a180(8, 0);
            goto loop_done;
        }
        Func_0808a170(0xc5c + step);
        Func_0808a178(8, 0);
        step++;
        goto loop_test;

loop_failed:
        Func_0808a100(0, 22);
        Func_0808a128(8, 2, 20);
        Func_0808a128(8, 4, 20);
        Func_0808a170(0xc63);
        Func_0808a180(8, 0);

loop_done:

        Func_08015120(300, 4);
        Func_080f9010(81);
        Func_08015040(0xc64, 3);
        Func_0808a170(0xc65);
        Func_0808a128(8, 2, 20);
        Func_0808a180(8, 0);
        Func_080f9010(9);
    } else {
        Func_0808a018();

        other = Func_0808a080(0);
        if (other != 0) {
            Func_0808a0f0(8, *(s32 *)(other + 0x08), *(s32 *)(other + 0x10));
        }

        *(s32 *)(actor + 0x28) = 0xa0000;
        Func_08009150(actor, x, 0, z);

        Func_0808a010(30);
        Func_0808a3d8();
        Func_0808a148(8, 0, 0);
        Func_0808a148(0, 8, 0);
        Func_0808a100(0, 22);
        Func_0808a170(0xc68);
        Func_0808a128(8, 2, 20);
        Func_0808a128(8, 2, 20);
        Func_0808a180(8, 0);
        Func_0808a138(8, 2);
        Func_0808a180(8, 0);
        Func_080f9010(111);
        Func_080153e8(0, 2);
        Func_080770c8(0x16f);
        Func_080770d0(0x171);
        Func_080a1040();
        Func_0808a170(0xc6a);

        /* r7 still carries 0x05300000 here: the first arm's reloads of r7
         * are on the other path. */
        Func_08009150(actor, 0x15d00000, 0, 0x05300000);

        Func_0808a010(30);
        Func_0808a180(8, 0);
        Func_0808a148(8, 0, 0);
        Func_0808a180(8, 0);
        Func_0808a178(8, 0);

        if (Func_0808a070(0, 0) != 1) {
            alternate_tail = 1;
        } else {
            Func_0808a100(0, 22);
            Func_0808a138(8, 2);
            Func_0808a170(0xc6d);
            Func_0808a178(8, 0);
            if (Func_0808a070(0, 0) == 1) {
                alternate_tail = 1;
            } else {
                Func_0808a180(8, 0);
                Func_0808a0d0(8, x >> 16, z >> 16);
            }
        }
    }

    if (alternate_tail == 0) {
        Func_02001c08();
        Func_0808a3e0();
        return;
    }

    Func_0808a100(0, 22);
    Func_0808a170(0xc6f);
    Func_0808a128(8, 2, 20);
    Func_0808a128(8, 2, 20);
    Func_0808a110(0, 3);
    Func_0808a1e8(8, 256, 30);
    Func_0808a180(8, 0);
    Func_080770c8(0x16f);
    Func_080770c8(0x171);
    Func_080a1040();
    Func_0808a128(8, 2, 20);
    Func_0808a180(8, 0);
    Func_0808a3e0();
    Func_0808a4f8(8, 0, 0);
    Func_080f9010(42);
    Func_0808a020();
    Func_080770d0(366);
    Func_080770d0(0x16f);
    Func_080770d0(0x171);
}
