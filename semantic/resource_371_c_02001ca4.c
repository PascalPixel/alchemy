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

u8 *Func_02005fb2();
u8 *Func_02005fba();
s32 Func_02005f7c();
void Func_02005fc0();
void Func_02005f98();
void Func_02005fe4();
u8 *Func_0200600a();
void Func_02006078();
void Func_02005fda();
void Func_02005fec();
void Func_02006190();
void Func_020060ca();
void Func_02006018();
void Func_02006122();
void Func_020060ec();
void Func_02005eea();
void Func_02006110();
void Func_02006128();
void Func_020061dc();
void Func_0200620c();
void Func_02006218();
void Func_020061e4();
void Func_02005fd4();
void Func_02005f4e();
void Func_02006164();
void Func_0200616e();
void Func_02005f6c();
void Func_02006050();
void Func_020061b6();
void Func_020062ac();
void Func_02006284();
void Func_02006082();
void Func_020060aa();
void Func_02005eb6();
void Func_02005fea();
void Func_020062ea();
void Func_020060d2();
void Func_02006202();
void Func_0200623a();
void Func_02006266();
void Func_0200624e();
void Func_02006278();
void Func_020061f6();
void Func_0200621e();
void Func_02006276();
void Func_020062a0();
void Func_0200624a();
void Func_0200614c();
void Func_0200629c();
void Func_020062c8();
void Func_020062b0();
void Func_02006288();
void Func_020062c0();
void Func_0200628a();
void Func_020062d2();
void Func_02006156();
void Func_020060c8();
void Func_020062b4();
void Func_0200630c();
void Func_02006106();
void Func_020062f2();
void Func_0200634a();
void Func_02006376();
void Func_02006338();
void Func_02006370();
void Func_0200633a();
void Func_0200637a();
s32 Func_020062cc();
void Func_0200635a();
void Func_02006364();
void Func_0200639e();
void Func_020063b6();
void Func_020063b0();
void Func_020063c0();
void Func_02006384();
void Func_020063a8();
void Func_020063de();
void Func_020063f6();
void Func_020062b8();
void Func_020064f6();
void Func_020062c2();
void Func_02006408();
void Func_020063e2();
void Func_0200642a();
void Func_02006520();
void Func_0200635e();
u8 *Func_02006384_b();
void Func_020063f2();
void Func_020062cc_b();
void Func_02006382();
void Func_0200650e();
void Func_02006448();
void Func_02006452();
void Func_02006432();
void Func_02006478();
void Func_0200645c();
void Func_020064a4();
void Func_0200647c();
void Func_020064b4();
void Func_020065aa();
void Func_02006392();
void Func_020065ba();
void Func_020064c8();
void Func_02006354();
void Func_0200640a();
void Func_020064f2();
void Func_020064d4();
void Func_02006504();
s32 Func_02006454();
void Func_020064d0();
void Func_020064f8();
void Func_0200651e();
void Func_0200652e();
s32 Func_0200647e();
void Func_0200654a();
void Func_020064e8();
void Func_02003db0();
void Func_02006610();
void Func_0200651a();
void Func_02006560();
void Func_0200653a();
void Func_02006544();
void Func_020065b8();
void Func_020065a0();
void Func_02006486();
void Func_0200648c();
void Func_02006698();
void Func_0200657a();
void Func_020065c2();
void Func_02006676();
void Func_020066a0();
void Func_020066c6();
void Func_0200650a();
void Func_020064ca();
void Func_020064d6();

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

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

    actor = Func_02005fb2(8);
    reference = Func_02005fba(0);

    /* Midpoints between the reference object and (0x15d00000, 0x05300000).
     * The pooled constants are the negatives, 0xea300000 and 0xfad00000. */
    x = (*(s32 *)(reference + 0x08) - 0x15d00000) / 2 + 0x15d00000;
    z = (*(s32 *)(reference + 0x10) - 0x05300000) / 2 + 0x05300000;

    alternate_tail = 0;

    if (Func_02005f7c(366) == 0) {
        Func_02005fc0(1);
        Func_02005f98(366);
        Func_02005fe4();

        other = Func_0200600a(0);
        if (other != 0) {
            Func_02006078(8, *(s32 *)(other + 0x08), *(s32 *)(other + 0x10));
        }

        Func_02005fda(0, 0, 0);
        Func_02005fec(0, 0, 0);
        Func_02006190();
        Func_020060ca(0, 8, 0);
        Func_02006018(10);
        Func_02006122(0, 0x101, 60);

        *(u16 *)(actor + 0x66) = 1;

        Func_020060ec(8, 0, 0);
        Func_02005eea(16);
        Func_02006110(0xc4f);
        Func_02006128(8, 0);
        Func_020061dc();
        Func_0200620c(0x13333, 6);
        Func_02006218();
        Func_020061e4();

        flag = actor + 0x55;
        *(s32 *)(actor + 0x48) = 0x4000;
        *flag = 2;
        *(s32 *)(actor + 0x34) = 0x10000;
        *(s32 *)(actor + 0x28) = 0;
        *(s32 *)(actor + 0x30) = 0x10000;
        *(s32 *)(actor + 0x14) = 0;

        Func_02005fd4(actor, 0x15d00000, 0, 0x05300000);

        count = 15;
        do {
            *(s32 *)(actor + 0x18) += 0x800;
            *(s32 *)(actor + 0x1c) += 0x800;
            count--;
            Func_02005f4e(1);
        } while (count >= 0);

        Func_02006164(8, 0, 0);
        Func_0200616e(0, 8, 0);
        Func_02005f6c(16);

        *(s32 *)(actor + 0x6c) = 0;

        *(s32 *)(actor + 0x48) = 0x10000;
        Func_02006050(actor, 0);
        Func_020061b6(8, 0);
        Func_020062ac(131);
        Func_02006284(140, 0);

        count = 59;
        do {
            if ((Data_03001e40 & 2) != 0) {
                Func_02006082(actor, 7);
            } else {
                Func_020060aa(actor, 0);
            }
            if ((Data_03001e40 & 0xf) == 0) {
                Func_02005eb6(actor);
            }
            count--;
            Func_02005fea(1);
        } while (count >= 0);

        Func_020062ea();
        Func_020060d2(actor, 0);
        Func_02006202(8, 2);
        Func_0200623a(8, 0);
        Func_02006266(0, 258, 30);
        Func_0200624e(8, 0);
        Func_02006278(0, 0x101, 30);
        Func_020061f6(8, x >> 16, z >> 16);
        Func_0200621e(0, 22);
        Func_02006276(8, 0);
        Func_020062a0(0, 0x101, 40);
        Func_0200624a(8, 4, 30);
        Func_0200614c(300, 4);
        Func_0200629c(8, 0);
        Func_020062c8(0, 256, 30);
        Func_020062b0(8, 0);
        Func_02006288(0, 2);
        Func_020062c0(8, 0);
        Func_0200628a(8, 2, 30);
        Func_020062d2(8, 0);

        *flag = 0;
        Func_02006156(actor, x, 0x100000, z);

        count = 15;
        do {
            *(u16 *)(actor + 0x06) = (u16)(*(u16 *)(actor + 0x06) + 0x1000);
            count--;
            Func_020060c8(1);
        } while (count >= 0);

        Func_020062b4(0, 1);
        Func_0200630c(8, 0);

        *flag = 2;
        *(s32 *)(actor + 0x28) = 0;
        *(s32 *)(actor + 0x14) = 0;

        count = 7;
        do {
            *(u16 *)(actor + 0x06) = (u16)(*(u16 *)(actor + 0x06) + 0x1000);
            count--;
            Func_02006106(1);
        } while (count >= 0);

        Func_020062f2(0, 22);
        Func_0200634a(8, 0);
        Func_02006376(8, 258, 30);
        Func_02006338(8, 0, 0);
        Func_02006338(8, 2);
        Func_02006370(8, 0);
        Func_0200633a(8, 2, 30);
        Func_0200637a(8, 0);

        step = 0;
loop_test:
        if (Func_020062cc(0, 0) != 1) {
            goto loop_failed;
        }
        Func_0200635a(8, 2, 20);
        Func_02006364(8, 2, 20);
        if (step == 6) {
            Func_0200639e(0xc62);
            Func_020063b6(8, 0);
            goto loop_done;
        }
        Func_020063b0(0xc5c + step);
        step++;
        goto loop_test;
        Func_020063c0(8, 0);

loop_failed:
        Func_02006384(0, 22);
        Func_0200639e(8, 2, 20);
        Func_020063a8(8, 4, 20);
        Func_020063de(0xc63);
        Func_020063f6(8, 0);

loop_done:

        Func_020062b8(300, 4);
        Func_020064f6(81);
        Func_020062c2(0xc64, 3);
        Func_02006408(0xc65);
        Func_020063e2(8, 2, 20);
        Func_0200642a(8, 0);
        Func_02006520(9);
    } else {
        Func_0200635e();

        other = Func_02006384_b(0);
        if (other != 0) {
            Func_020063f2(8, *(s32 *)(other + 0x08), *(s32 *)(other + 0x10));
        }

        *(s32 *)(actor + 0x28) = 0xa0000;
        Func_020062cc_b(actor, x, 0, z);

        Func_02006382(30);
        Func_0200650e();
        Func_02006448(8, 0, 0);
        Func_02006452(0, 8, 0);
        Func_02006432(0, 22);
        Func_02006478(0xc68);
        Func_02006452(8, 2, 20);
        Func_0200645c(8, 2, 20);
        Func_020064a4(8, 0);
        Func_0200647c(8, 2);
        Func_020064b4(8, 0);
        Func_020065aa(111);
        Func_02006392(0, 2);
        Func_020063a8(0x16f);
        Func_020063b6(0x171);
        Func_020065ba();
        Func_020064c8(0xc6a);

        /* r7 still carries 0x05300000 here: the first arm's reloads of r7
         * are on the other path. */
        Func_02006354(actor, 0x15d00000, 0, 0x05300000);

        Func_0200640a(30);
        Func_020064f2(8, 0);
        Func_020064d4(8, 0, 0);
        Func_02006504(8, 0);
        Func_02006504(8, 0);

        if (Func_02006454(0, 0) != 1) {
            alternate_tail = 1;
        } else {
            Func_020064d0(0, 22);
            Func_020064f8(8, 2);
            Func_0200651e(0xc6d);
            Func_0200652e(8, 0);
            if (Func_0200647e(0, 0) == 1) {
                alternate_tail = 1;
            } else {
                Func_0200654a(8, 0);
                Func_020064e8(8, x >> 16, z >> 16);
            }
        }
    }

    if (alternate_tail == 0) {
        Func_02003db0();
        Func_02006610();
        return;
    }

    Func_0200651a(0, 22);
    Func_02006560(0xc6f);
    Func_0200653a(8, 2, 20);
    Func_02006544(8, 2, 20);
    Func_02006544(0, 3);
    Func_020065b8(8, 256, 30);
    Func_020065a0(8, 0);
    Func_02006486(0x16f);
    Func_0200648c(0x171);
    Func_02006698();
    Func_0200657a(8, 2, 20);
    Func_020065c2(8, 0);
    Func_02006676();
    Func_020066a0(8, 0, 0);
    Func_020066c6(42);
    Func_0200650a();
    Func_020064ca(366);
    Func_020064d0(0x16f);
    Func_020064d6(0x171);
}
