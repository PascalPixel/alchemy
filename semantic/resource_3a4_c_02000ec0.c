#include "types.h"

/*
 * resource_3a4 owner at 0x02000ec0, 1240 bytes: the overlay's shared
 * set-piece. Four actor handles are taken at the head (0, 8, 9 and 10),
 * a long staged sequence runs over them with seven sound cues and seven
 * screen fades, two of the actors are waited on through their own
 * completion halfwords, and the row closes by setting story flag 0x908.
 *
 * A SWEEP-A ROW WHOSE BOTH CALLERS ARE SWEEP-B. It is called from
 * 0x02000d2c with 5 and from 0x02001398 with 10, and both of those are
 * published callbacks that no `bl` reaches. That is the worked example
 * behind the closure standard's rule that A and B are one bill.
 *
 * ITS ONE ARGUMENT IS A FRAME COUNT, and that is now proved rather than
 * inferred. r0 is stored to sp + 8 at 0x02000ed0 and read back exactly
 * once, at 0x020010a8, straight into Func_0808a010 -- the wait between
 * the actor-9 setup and the actor-8 approach. So Func_02000ec0(5) waits
 * five frames there and Func_02000ec0(10) waits ten; nothing else in the
 * body depends on the parameter.
 *
 * THE FLAG WIRING RUNS BOTH WAYS BETWEEN THIS ROW AND 0x02001398, and
 * neither direction is visible from either row alone:
 *   - 0x02001398 SETS 0x205 before calling this row, and this row TESTS
 *     0x205 four times to decide whether the actor-1 material runs. So
 *     the same 1240 bytes play differently depending on which caller
 *     arrived, with no argument distinguishing them.
 *   - this row SETS 0x908 at the very end, and 0x908 is exactly what
 *     0x02001398 tests to bail out. This row is the once-only latch for
 *     the cutscene that calls it.
 * A reader who takes either row on its own sees a flag id and no story.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000ec0 through
 * `add sp, #12` at 0x0200130a and `b.n 0x2001388` at 0x0200130c, which
 * JUMPS THE THIRTY-TWO-WORD LITERAL POOL 0x0200130e-0x02001387 to the
 * shared unwind `pop {r3, r5, r6, r7} / mov r8, r3 / mov r9, r5 /
 * mov sl, r6 / mov fp, r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001388-0x02001396; the next owner is the already-drafted
 * 0x02001398. That is 1240 bytes -- exactly what both drafted
 * neighbours already fixed. measureSpan agrees at 1240 here, the second
 * row of fourteen on this overlay where it does.
 *
 * The pool word at 0x02001338 disassembles as `pop {r3, r4, r5, r6, pc}`
 * and the one at 0x02001340 as `b.n 0x2001010`. Neither is code. A walk
 * that stopped at the first of those would end the row 80 bytes early
 * and land inside its own literal pool.
 *
 * Ninety-three call sites, every one resolved with the `+2` rule via
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3a4 0xec0 0x1398` and none
 * read from the listing's arrows. Site count cross-checked: the resolver
 * reports sites=93 and the listing carries 93 `bl` lines. They collapse
 * onto twenty-eight targets, all import veneers, with no in-image
 * prologue among them.
 *
 * TWO MORE PUBLISHED POINTERS, taking this overlay's count of the
 * mechanism to six. Under the base + 0x8000 in-image spelling:
 *   - 0x02008099 at 0x02001318 is image offset 0x98 with the Thumb bit,
 *     the already-drafted 0x02000098, stored into actor 9's handle at
 *     +108 at 0x02000f96 and cleared again at 0x0200112c. So the SAME
 *     +108 field this overlay uses for 0x02001770 also carries 0x98;
 *   - 0x0200abe1 at 0x02001364 is image offset 0x2be0 with the Thumb
 *     bit, the already-drafted 0x02002be0, handed to Func_080000d0 at
 *     the established 200 << 4 rate and taken back with Func_080000d8
 *     eighty frames later -- the install-and-remove shape 0x02003028
 *     also uses.
 *
 * SIX CALLEE-SAVED REGISTERS ARE RECYCLED and none may be read as live
 * across the change:
 *   - r6 holds the actor-10 handle from 0x02000ef2, becomes actor 0's
 *     handle + 100 at 0x02001056, and becomes the constant 0xffc00000 at
 *     0x02001126, in which form it is passed to Func_0808a210 at
 *     0x02001276;
 *   - r8 holds the actor-0 handle from 0x02000ed8 and becomes the
 *     constant 128 << 11 at 0x02001152, in which form it is passed to
 *     Func_0808a208 at 0x0200129e;
 *   - r5 is 128 << 10, then actor 8's handle + 100, then 0, then
 *     0x0200abe1;
 *   - r9 holds 0x03120000 and is passed as an argument at 0x020010a2;
 *   - r7 (actor 8) and sl (actor 9) are the only two that survive whole;
 *   - fp is 0 from 0x02000f9a and is the value both completion halfwords
 *     are cleared with.
 *
 * The two waits at 0x020010d0 and 0x020010e6 are the same shape: clear
 * the completion halfword at handle + 100, then yield one frame at a
 * time through Func_080000c0(1) until it reads back non-zero. They are
 * read as SIGNED halfwords (`ldrsh`) and are UNBOUNDED -- no counter, no
 * escape -- unlike the 60-frame cap on 0x02001d0c's wait.
 *
 * Constants are written in the shifted-literal form the encodings build
 * -- 196 << 18, 232 << 15, 139 << 18, 197 << 18, 146 << 18, 132 << 18,
 * 198 << 2, 151 << 2, 161 << 2, 206 << 2, 210 << 2, 138 << 2, 146 << 2,
 * 129 << 1, 224 << 12, 180 << 15, 128 << 14, 128 << 6, 128 << 7,
 * 128 << 8, 128 << 9, 128 << 10, 128 << 11, 160 << 6, 160 << 7,
 * 160 << 9, 160 << 10, 160 << 11, 192 << 7, 192 << 8, 192 << 10,
 * 224 << 11 -- because that shape, not the flattened value, is what the
 * row shows. Pool constants 0x101, 0x13333, 0x03120000, 0xfff80000,
 * 0x205, 0x036e0000, 0x195c2, 0xcae1, 0x121, 0xe666, 0xffc00000,
 * 0x026a0000, 0x19999, 0xcccc, 0x1999, 0x3333, 0x312, 0x33333, 0x22e,
 * 0x9999, 0x4ccc, 0x356, 0x02620000, 0x03560000, 0x232 and 0x908 are
 * transcribed as words.
 */

extern void Func_080000c0(s32 frames);
extern void Func_080000d0(s32 callback, s32 rate);
extern void Func_080000d8(s32 callback);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_080770c8(s32 flag_id);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, u8 *set);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a4f0(void);
extern void Func_080f9010(s32 sound_id);

extern u8 Data_0200bd78[];   /* image offset 0x3d78: sprite set */
extern u8 Data_0200bdec[];   /* image offset 0x3dec: sprite set */

/* image offset 0x98 with the Thumb bit: this overlay's own 0x02000098 */
#define PUBLISHED_02000098 0x02008099
/* image offset 0x2be0 with the Thumb bit: this overlay's own 0x02002be0 */
#define PUBLISHED_02002be0 0x0200abe1

void Func_02000ec0(s32 wait_frames)
{
    u8 *actor0;
    u8 *actor8;
    u8 *actor9;
    u8 *actor10;
    u8 *handle;

    actor0 = Func_0808a080(0);
    actor8 = Func_0808a080(8);
    actor9 = Func_0808a080(9);
    actor10 = Func_0808a080(10);

    Func_0808a1f0(0, 129 << 1);
    Func_0808a010(40);
    Func_0808a208(128 << 9, 128 << 6);
    Func_0808a210(196 << 18, -1, 232 << 15, 1);
    Func_0808a090(0, 128 << 10, 128 << 9);
    Func_0808a100(0, 6);
    Func_0808a0c0(0, 198 << 2, 140);
    Func_0808a100(0, 1);
    Func_0808a1b8(0, 192 << 8, 100);
    Func_0808a1e8(0, 0x101, 60);
    Func_080f9010(183);
    Func_080091f0(192 << 10, 192 << 10, 128 << 9);
    Func_0808a010(20);

    *(s32 *)(actor9 + 24) = 0x13333;
    *(s32 *)(actor9 + 28) = 0x13333;
    actor9[35] = (u8)(actor9[35] | 2);
    *(s32 *)(actor9 + 108) = PUBLISHED_02000098;

    Func_0808a100(8, 4);

    *(s32 *)(actor8 + 68) = 128 << 8;
    *(s32 *)(actor8 + 8) = 0x03120000;
    *(s32 *)(actor8 + 12) = 128 << 14;
    *(s32 *)(actor8 + 16) = 180 << 15;
    *(s32 *)(actor8 + 24) = 128 << 10;
    *(s32 *)(actor8 + 28) = 128 << 10;

    Func_0808a010(10);
    Func_080f9010(183);
    Func_080091f0(128 << 11, 128 << 10, 128 << 9);
    Func_0808a010(20);

    *(s32 *)(actor10 + 8) = *(s32 *)(actor10 + 8) + (224 << 12);
    *(s32 *)(actor10 + 12) = *(s32 *)(actor10 + 12) + 0xfff80000;
    *(u16 *)(*(u8 **)(actor10 + 80) + 30) = (u16)(192 << 8);

    Func_080f9010(107);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);
    Func_0808a1e8(0, 129 << 1, 80);
    Func_080f9010(55);
    Func_080091f0(128 << 9, 192 << 10, 128 << 9);
    Func_0808a1e0(8, 0);
    Func_0808a1e0(0, 0);
    Func_0808a1f0(0, 0x101);
    Func_0808a090(0, 160 << 10, 160 << 9);

    *(u16 *)(actor0 + 100) = 0;
    Func_0808a098(0, Data_0200bdec);

    if (Func_080770c0(0x205) != 0) {
        Func_0808a0f0(1, 0x036e0000, 132 << 18);
        handle = Func_0808a080(1);
        *(u16 *)(handle + 6) = (u16)(160 << 7);
    }

    Func_0808a208(160 << 9, 160 << 6);
    Func_0808a210(0x03120000, -1, 139 << 18, 1);

    Func_0808a010(wait_frames);

    Func_0808a1e0(8, 1);
    Func_0808a090(8, 0x195c2, 0xcae1);

    *(u16 *)(actor8 + 100) = 0;
    Func_0808a098(8, Data_0200bd78);

    do {
        Func_080000c0(1);
    } while (*(s16 *)(actor0 + 100) == 0);

    Func_0808a1f0(0, 0);

    do {
        Func_080000c0(1);
    } while (*(s16 *)(actor8 + 100) == 0);

    Func_0808a1e0(0, 2);

    handle = Func_0808a080(0);
    handle[35] = (u8)(handle[35] | 1);

    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);

    *(s32 *)(actor9 + 8) = 0x03120000;
    *(s32 *)(actor9 + 108) = 0;
    *(s32 *)(actor9 + 16) = 0x026a0000;
    *(s32 *)(actor9 + 12) = 0xffc00000;

    Func_0808a090(8, 0x19999, 0xcccc);

    *(s32 *)(actor8 + 68) = 0x1999;
    *(s32 *)(actor8 + 72) = 0x3333;
    *(s32 *)(actor8 + 40) = 128 << 11;

    Func_0808a0c0(8, 0x312, 151 << 2);
    Func_0808a090(8, 0x33333, 0x19999);
    Func_0808a0b8(8, 0x312, 161 << 2);
    Func_0808a010(15);
    Func_080091f0(160 << 11, 224 << 11, 128 << 9);

    Func_08009180(25, 36, 43, 36, 11, 9);
    Func_080091c0(25, 35, 10, 5, 43, 35);

    Func_0808a0f0(8, 0, 0);
    Func_0808a0f0(9, 0, 0);

    Func_080000d0(PUBLISHED_02002be0, 200 << 4);
    Func_0808a010(80);
    Func_080000d8(PUBLISHED_02002be0);

    Func_0808a010(60);
    Func_080f9010(17);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(120);

    if (Func_080770c0(0x205) != 0) {
        Func_0808a090(1, 128 << 9, 128 << 8);
        Func_0808a0c8(1, 206 << 2, 0x22e);
    }

    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 0x356, 146 << 2);

    if (Func_080770c0(0x205) != 0) {
        Func_0808a100(1, 1);
        Func_0808a1b8(1, 128 << 7, 0);
    }

    Func_0808a1b8(0, 192 << 7, 40);
    Func_0808a1e8(1, 129 << 1, 0);
    Func_0808a1e8(0, 129 << 1, 60);
    Func_0808a210(197 << 18, 0xffc00000, 0x02620000, 1);
    Func_0808a218();
    Func_080f9010(148);
    Func_0808a010(240);

    if (Func_080770c0(0x205) != 0) {
        Func_0808a208(128 << 11, 128 << 8);
        Func_0808a210(0x03560000, 0, 146 << 18, 1);
        Func_0808a218();
        Func_0808a0d0(1, 210 << 2, 138 << 2);
        Func_0808a0d0(1, 0x356, 0x232);
        Func_0808a100(1, 2);

        handle = Func_0808a080(0);
        if (handle != 0) {
            Func_0808a0b8(1, *(s16 *)(handle + 10), *(s16 *)(handle + 18));
        }

        Func_0808a0e8(1);
        Func_0808a0f0(1, 0, 0);
    }

    Func_0808a4f0();
    Func_080770c8(0x908);
}
