#include "types.h"

/*
 * resource_383 owner at 0x02000de4, 1380 bytes (0x02000de4-0x02001347):
 * code 0x02000de4-0x02001206, a three-word literal pool at 0x02001208 that
 * the body branches over, code 0x02001214-0x0200133e, and a two-word literal
 * pool at 0x02001340.
 *
 * The long cutscene that Func_02000b48 plays.  It is entirely straight-line -
 * there is not one conditional branch in the whole owner, only the single
 * unconditional `b` at 0x02001206 that steps over the interior pool - so it
 * reads as a script: pose, walk, camera, line, wait, repeated for
 * participants 0, 1 and 2.  The dialogue line ids are all formed from the
 * base 0x125f held in r8 (0x125f, 0x1264, 0x1265, 0x126c, 0x126d, 0x1275).
 *
 * Between the beats it toggles bit 0 of the byte at +90 of participants 0 and
 * 1 (clear, then set) and clears the byte at +91 of participant 2.
 *
 * All 151 `bl` sites are placed and reach 25 distinct callees - the inventory
 * row's `calls=143` predates the corrected decoding.  Targets come from
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  111 reach the import veneer table at 0x02004cxx-0x02004f2b
 * and are named by the main-image address in the veneer's trailing word; 40
 * reach this overlay's own Func_020045f4, Func_0200460c, Func_02004624,
 * Func_0200463c, Func_02004658 and Func_02004684, all byte-exact in
 * assets/code.
 *
 * Epilogue is `pop {r3} / mov r8, r3 / pop {r5, r6} / pop {r0} / bx r0`, so
 * r0 holds the popped return address and the owner is void.
 *
 * Uncertainties: the result of Func_0808a070 at 0x02000f84 is never tested -
 * it is called purely for its effect, unlike the same import elsewhere in
 * this overlay.  Func_02004684 and Func_0808a480 are reached with no argument
 * register set up, so they are treated as taking none.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_08015040();
void Func_0808a010();
void Func_0808a048();
s32 Func_0808a070();
u8 *Func_0808a080();            /* returns the participant record */
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a100();
void Func_0808a128();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a480();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_0200460c(s32, s32, s32);
void Func_02004624(s32, s32, s32);
void Func_0200463c(s32, s32, s32);
void Func_02004658(s32, s32);
void Func_02004684(void);

void Func_02000de4(void)
{
    u8 *record;
    s32 line;

    /* The dialogue base held in r8 for the whole owner. */
    line = 0x125f;

    Func_0808a010(20);
    record = Func_0808a080(2);
    record[91] = 0;
    Func_0808a128(2, 4, 0);
    Func_0808a010(40);

    Func_0808a170(line);                /* 0x125f */
    Func_0808a010(20);
    Func_020045f4(2, 20);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(50);
    Func_020045f4(2, 30);
    Func_0808a0d0(2, 376, 392);         /* 188 << 1, 196 << 1 */
    Func_02004658(2, 0);
    Func_0808a010(40);
    Func_02004684();
    Func_0808a138(0, 1);

    record = Func_0808a080(0);
    record[90] = (u8)(record[90] & 0xfe);
    Func_0808a0d0(0, 384, 424);         /* 192 << 1, 212 << 1 */
    Func_0808a010(1);
    record = Func_0808a080(0);
    record[90] = (u8)(record[90] | 1);
    Func_0808a010(30);

    Func_0808a180(2, 0);
    Func_0808a1f0(0, 258, 0);           /* 129 << 1 */
    Func_0808a1f0(1, 258, 0);
    Func_0808a010(60);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 30);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(60);
    Func_0808a138(2, 1);
    Func_0808a010(10);
    Func_0200463c(2, 3, 20);
    Func_0808a180(2, 0);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0200460c(1, 0, 30);
    Func_0808a138(0, 2);
    Func_0808a010(10);
    Func_0200460c(0, 1, 40);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(60);
    Func_0808a1e8(2, 0x102, 0);
    Func_0808a010(60);
    Func_0808a178(2, 0);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);

    /* Called for effect; the result is not tested here. */
    Func_0808a070(0, 0);

    Func_0808a010(30);
    Func_0200463c(2, 3, 10);
    Func_0808a0d0(2, 384, 408);         /* 204 << 1 */
    Func_0808a010(10);
    Func_08015040(line + 5, 1);         /* 0x1264 */
    Func_0808a170(line + 6);            /* 0x1265 */
    Func_0808a150(2, 1, 0);
    Func_0200460c(0, 1, 20);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(60);
    Func_0808a138(1, 1);

    record = Func_0808a080(1);
    record[90] = (u8)(record[90] & 0xfe);
    Func_0808a0d0(1, 352, 408);         /* 176 << 1 */
    Func_0808a010(1);
    record = Func_0808a080(1);
    record[90] = (u8)(record[90] | 1);
    Func_0808a010(10);

    Func_0808a0c8(2, 368, 408);         /* 184 << 1 */
    Func_0808a010(20);
    Func_0808a0d0(0, 368, 424);
    Func_0808a148(0, 1, 0);
    Func_0808a0e8(2);
    Func_02004658(2, 1);
    Func_0200463c(1, 4, 10);
    Func_0808a180(1, 0);
    Func_02004684();
    Func_02004624(2, 0, 30);
    Func_0808a180(2, 0);
    Func_0808a138(0, 1);
    Func_0808a010(30);
    Func_0200463c(0, 3, 30);
    Func_0200463c(2, 3, 10);
    Func_0808a180(2, 0);
    Func_0808a138(1, 2);
    Func_0808a148(1, 0, 0);
    Func_0808a180(1, 0);
    Func_0808a148(0, 1, 0);
    Func_0200463c(0, 3, 40);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 30);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(60);
    Func_0200463c(2, 3, 20);
    Func_0808a180(2, 0);
    Func_0808a1e8(1, 0x103, 0);
    Func_0808a010(60);
    Func_0200460c(1, 0, 10);
    Func_0808a180(1, 0);
    Func_0808a1e8(2, 0x100, 0);
    Func_0808a010(60);
    Func_0808a180(2, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(60);
    Func_08015040(line + 13, 1);        /* 0x126c */
    Func_0808a170(line + 14);           /* 0x126d */
    Func_0808a150(2, 0, 0);
    Func_0808a148(1, 0, 0);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a010(60);
    Func_02004658(2, 0);
    Func_0808a480();
    Func_020045f4(1, 30);
    Func_02004684();
    Func_0808a150(2, 1, 0);
    Func_0200463c(2, 3, 10);
    Func_020045f4(2, 20);
    Func_0808a150(2, 0, 0);
    Func_0200463c(0, 3, 20);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_0200463c(1, 4, 10);
    Func_020045f4(1, 30);

    Func_0808a148(2, 1, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(30);
    Func_02004624(0, 1, 10);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 30);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a148(2, 0, 0);
    Func_0808a1e8(2, 0x106, 0);         /* 131 << 1 */
    Func_0808a010(60);
    Func_020045f4(2, 10);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 20);
    Func_0200463c(2, 4, 20);
    Func_020045f4(2, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(60);
    Func_0200463c(2, 3, 10);
    Func_020045f4(2, 30);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a010(60);
    Func_0200463c(2, 4, 10);
    Func_020045f4(2, 30);
    Func_0200463c(2, 3, 10);
    Func_0808a100(0, 3);
    Func_0200463c(1, 3, 20);
    Func_0808a048(2, 1);
    Func_0808a170(line + 22);           /* 0x1275 */
    Func_0808a138(2, 1);
    Func_0808a180(2, 0);
}
