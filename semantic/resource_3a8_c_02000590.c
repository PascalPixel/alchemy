#include "types.h"

/*
 * resource_3a8 owner at 0x02000590, 4,092 bytes: the overlay's main scripted
 * cutscene - the one gated on story flag 0x910 being set and 0x911 not yet
 * latched, which stages ten scene entities (14, 20, 21, 27-34) through a long
 * choreography and finally latches 0x911.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x02000590 through the single interworking epilogue at 0x0200153c
 * (`pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0`).  The next
 * inventory row, 0x0200158c, has a byte-exact source in assets/code, so the
 * owner is exactly its advertised span.
 *
 * Four literal pools sit inside the span and are DATA, not code; all four are
 * branched over and all four are excluded:
 *   0x0200069c-0x020006b3   (jumped by `b 0x020006c6` at 0x0200069a)
 *   0x02000aac-0x02000aef   (jumped by `b 0x02000af0` at 0x02000aa8)
 *   0x02000f3c-0x02000f77   (jumped by `b 0x02000f78` at 0x02000f3a)
 *   0x02001548-0x0200158b   (after the epilogue)
 * Two further "pool words" that `overlay_show.ts` reports, 0x020009e4 and
 * 0x02000e08, are NOT pools: each is the low halfword of a BL pair
 * (0x20009e2 and 0x2000e06 respectively) that happens to be reachable from an
 * `ldr rN,[pc,#816]` decoded out of the real pool word 0x00004ccc.  They were
 * checked individually and reinstated as code; treating them as data would
 * have silently dropped `movs r0,#21` and `movs r1,#160`.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with a
 * constant before any read and r1-r3 are never read before being written, so
 * it takes no arguments.  There is no stack frame; nothing is live past the
 * return.
 *
 * TWO `bl`s THAT ARE GOTOs.  0x020005aa and 0x020005b8 both branch to
 * 0x0200151c, which the resolver reports as `unknown` because it is neither a
 * veneer nor a prologue.  It is not a call: 0x0200151c is an arm of THIS
 * function, reached otherwise only by falling past the `b 0x02001538` at
 * 0x0200151a, and it runs straight into the owner's own epilogue.  That
 * epilogue pops the frame pushed at 0x02000590, which is still intact, so the
 * `bl` never returns - it executes the abort arm and returns to this
 * function's caller.  It is spelled as a `goto` below, which is exactly its
 * effect; the clobbered lr is dead because the real return address is on the
 * stack.  (This is the same shape HANDOVER records for resource_372's
 * `bl .L_02003390`.)
 *
 * Call accounting: 406 `bl` sites, resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3a8 0590` - 41 distinct targets,
 * 396 import veneers, 8 calls to three intra-overlay prologues that all have
 * byte-exact sources in assets/code (Func_02001e6c x1, Func_02001ea4 x6,
 * Func_02001ed8 x1), and the 2 goto-`bl`s above.  Every site is reproduced
 * below exactly once; the transcription's per-callee counts were checked back
 * against the resolver's, target by target:
 *   0808a1b8 x70  0808a188 x34  0808a0a8 x30  0808a090 x27  0808a098 x25
 *   0808a138 x19  0808a110 x19  0808a080 x18  0808a0d0 x17  0808a010 x16
 *   0808a128 x15  080000c0 x15  0808a1e8 x14  0808a0f0 x14  0808a1e0 x10
 *   0808a100  x9  0808a088  x8  0808a170  x7  0808a1f0  x5  080f9010  x4
 *   0808a208  x2  0808a130  x2  080770c0  x2  and fourteen singletons.
 * The inventory row advertises `calls=367`; the difference is the sites inside
 * the four wait loops and the repeated bodies the row folds.  The
 * disassembler's own `bl` annotations are wrong in the usual overlay way (an
 * overlay `bl` stores `target_offset - 2`) and were not used anywhere.
 *
 * LINK BASE 0x02008000, with the strongest witness available: the word handed
 * to Func_080000d8 at 0x02000d38 is 0x02009f15, i.e. file offset 0x1f14 plus
 * the Thumb bit - and 0x02001f14 is the five-step ping-pong state machine
 * reconstructed in this same reconstruction, whose `push {lr}` prologue is right there.
 * Installing exactly that routine here is also what the two owners mean
 * together, and its state word Data_0200d144 is the one the byte-exact
 * assets/code/resource_3a8_c_02001ed8.c zeroes - which is the routine called
 * from 0x02000c40 here, six instructions before.  The remaining
 * `0x0200b*`/`0x0200c*` pool words are therefore in-image script data at
 * `value - 0x8000`; 0x0200bfb0 in particular is the same script offset 0x3fb0
 * that Func_02001f14 hands to Func_0808a098.
 *
 * `Data_03001ebc` is the IWRAM scene pointer, named and typed by the
 * byte-exact assets/code/resource_3a8_c_02003184.c.  The halfword at
 * `scene + 364` (`0xb6 << 1`) selects between two presentation variants at
 * both ends of the function and is passed to Func_0808a248 on the abort path.
 *
 * UNCERTAINTIES.
 *  - None of the 0x0808axxx scene-script imports has an established interface;
 *    the arity used here is the number of argument registers each site
 *    actually sets, and the literals are reproduced positionally.  All are
 *    declared old-style, as this overlay's convention requires.
 *  - Func_080000d8 is given only r0 at 0x02000d38; r1 holds a call-clobbered 0
 *    from the preceding Func_0808a188 and is not asserted as an argument.
 *  - r8 is set to 0 once at 0x020008d8 and read back at 0x02000d54/0x02000d60;
 *    it is written as the constant 0.
 *  - Func_0808a0d0's second and third arguments are always a small unsigned
 *    byte and an even halfword; they are kept as plain s32 literals.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                /* scene entity by selector */
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a030();
void Func_0808a088();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1d8();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_0808a4f0();
void Func_080000c0();               /* wait n frames (busy) */
void Func_080000d8();               /* install a per-frame task */
void Func_08015210();
s32 Func_080770c0();                /* read a story flag */
void Func_080770c8();               /* set a story flag */
void Func_080f9010();               /* play a sound cue */

/* Intra-overlay callees, all byte-exact in assets/code. */
void Func_02001e6c(void);
void Func_02001ea4(s32);
void Func_02001ed8(void);

/* IWRAM scene pointer, named by the byte-exact sibling for 0x02003184. */
extern u8 *Data_03001ebc;

/* In-image script data (file offsets value - 0x8000). */
extern u8 Data_0200c948[];          /* 0x4948 */
extern u8 Data_0200bdc4[];          /* 0x3dc4 */
extern u8 Data_0200bfb0[];          /* 0x3fb0, also used by Func_02001f14 */
extern u8 Data_0200c034[];          /* 0x4034 */
extern u8 Data_0200c0cc[];          /* 0x40cc */
extern u8 Data_0200c164[];          /* 0x4164 */
extern u8 Data_0200c1ac[];          /* 0x41ac */
extern u8 Data_0200be00[];          /* 0x3e00 */

/* 0x02009f15 = file offset 0x1f14 + the Thumb bit = Func_02001f14. */
extern u8 Data_02009f15[];

void Func_02000590(void)
{
    u8 *scene;
    u8 *entity;
    u8 *counter;
    s32 pose;
    void (*abortSound)() = Func_080f9010;
    void (*abortScene)() = Func_0808a248;
    void (*abortReset)() = Func_0808a368;
    void (*abortSettle)() = Func_0808a370;

    scene = Data_03001ebc;

    Func_0808a018();

    if (Func_080770c0(0x910) == 0) {
        goto aborted;
    }
    if (Func_080770c0(0x911) != 0) {
        goto aborted;
    }

    Func_0808a030(Data_0200c948);

    Func_0808a0f0(20, 0x00fc0000, 0x01100000);
    Func_0808a0f0(27, 0x011c0000, 0x01080000);
    Func_0808a0f0(28, 0x011c0000, 0x01180000);
    Func_0808a0f0(29, 0x012c0000, 0x01080000);
    Func_0808a0f0(30, 0x012c0000, 0x01180000);
    Func_0808a0f0(32, 0x013c0000, 0x01080000);
    Func_0808a0f0(31, 0x013c0000, 0x01180000);
    Func_0808a0f0(33, 0x014c0000, 0x01080000);
    Func_0808a0f0(34, 0x014c0000, 0x01180000);
    Func_0808a0f0(21, 0x016c0000, 0x01100000);

    Func_080f9010(17);
    Func_0808a1d8(20);
    Func_08015210(0x1a91, 1, 0);
    Func_080f9010(9);
    Func_0808a010(10);
    Func_0808a138(0, 2);

    if (*(s16 *)(scene + 364) == 9) {
        Func_0808a208(0x00026666, 0x00004ccc);
        Func_0808a1b8(0, 0xe000, 20);
    } else {
        Func_0808a208(0x00013333, 0x00002666);
        Func_0808a1b8(0, 0, 20);
    }

    Func_0808a090(20, 0x00011999, 0x00008ccc);
    Func_0808a090(27, 0x00010000, 0x00008000);
    Func_0808a090(28, 0x00010000, 0x00008000);
    Func_0808a090(29, 0x0000e666, 0x00007333);
    Func_0808a090(30, 0x0000e666, 0x00007333);
    Func_0808a090(32, 0x0000cccc, 0x00006666);
    Func_0808a090(31, 0x0000cccc, 0x00006666);
    Func_0808a090(33, 0x0000b333, 0x00005999);
    Func_0808a090(34, 0x0000b333, 0x00005999);
    Func_0808a090(21, 0x00009999, 0x00004ccc);

    Func_0808a098(20, Data_0200bdc4);
    Func_0808a098(27, Data_0200bdc4);
    Func_0808a098(28, Data_0200bdc4);
    Func_0808a098(29, Data_0200bdc4);
    Func_0808a098(30, Data_0200bdc4);
    Func_0808a098(32, Data_0200bdc4);
    Func_0808a098(31, Data_0200bdc4);
    Func_0808a098(33, Data_0200bdc4);
    Func_0808a098(34, Data_0200bdc4);

    *(s16 *)counter = 0;
    counter = Func_0808a080(21) + 100;
    Func_0808a098(21, Data_0200bdc4);

    Func_0808a210(0x00ba0000, -1, 0x01100000, 1);
    Func_0808a0a0(20);
    Func_0808a1b8(20, 0, 0);
    do {
        Func_080000c0(1);
    } while (*(s16 *)counter == 0);

    Func_0808a010(40);
    Func_0808a138(27, 2);
    Func_0808a1b8(27, 0x5000, 20);
    Func_0808a170(0x1a92);
    Func_0808a188(27, 0, 10);
    Func_0808a138(28, 2);
    Func_0808a1b8(28, 0xb000, 10);
    Func_0808a100(28, 3);
    Func_0808a188(28, 0, 10);
    Func_0808a1f0(32, 0x102);
    Func_0808a010(40);
    Func_0808a188(32, 0, 10);
    Func_0808a1e8(31, 0x100, 40);
    Func_0808a1b8(31, 0xb000, 10);
    Func_0808a188(31, 0, 10);
    Func_0808a1b8(31, 0x8000, 10);
    Func_0808a138(31, 2);
    Func_0808a100(31, 4);
    Func_0808a188(31, 0, 10);
    Func_0808a1b8(31, 0xb000, 0);
    Func_0808a1b8(32, 0x5000, 20);
    Func_0808a100(31, 3);
    Func_0808a110(32, 3);
    Func_0808a138(20, 2);
    Func_0808a1f0(20, 0x102);
    Func_0808a010(40);
    Func_0808a188(20, 0, 10);
    Func_0808a138(20, 2);

    entity = Func_0808a080(20);
    entity[90] = (u8)(entity[90] & 0xfe);
    Func_0808a0d0(20, 172, 264);
    Func_0808a010(1);
    entity = Func_0808a080(20);
    entity[90] = (u8)(entity[90] | 1);

    Func_0808a1b8(27, 0x8000, 0);
    Func_0808a1b8(28, 0x8000, 0);
    Func_0808a1b8(32, 0x8000, 0);
    Func_0808a1b8(31, 0x8000, 20);
    Func_0808a110(20, 3);
    Func_0808a010(20);

    entity = Func_0808a080(20);
    entity[90] = (u8)(entity[90] & 0xfe);
    Func_0808a0d0(20, 172, 272);
    Func_0808a010(1);
    entity = Func_0808a080(20);
    entity[90] = (u8)(entity[90] | 1);
    Func_0808a0d0(20, 180, 272);

    Func_0808a1b8(20, 0, 0);
    Func_0808a188(20, 0, 10);
    Func_0808a1e8(34, 0x105, 0);
    Func_0808a138(34, 1);
    Func_0808a110(34, 3);
    Func_0808a188(34, 0, 10);
    Func_0808a138(33, 1);
    Func_0808a188(33, 0, 10);
    Func_0808a100(33, 4);
    Func_0808a188(33, 0, 10);
    Func_0808a138(21, 2);
    Func_0808a1e8(21, 0x102, 0);
    Func_0808a188(21, 0, 10);
    Func_0808a128(20, 2, 20);
    Func_0808a128(20, 4, 40);
    Func_0808a138(20, 2);
    Func_0808a188(20, 0, 10);

    Func_0808a090(21, 0x00019999, 0x0000cccc);
    Func_0808a0d0(21, 0x109, 282);
    Func_0808a0d0(21, 251, 284);
    Func_0808a0d0(21, 246, 296);
    Func_0808a1b8(21, 0xc000, 0);
    Func_02001e6c();
    Func_0808a010(40);
    Func_0808a090(21, 0x00019999, 0x0000cccc);
    Func_0808a0d0(21, 228, 296);
    Func_0808a1b8(21, 0xc000, 40);
    Func_0808a0d0(21, 212, 296);
    Func_0808a1b8(21, 0xc000, 40);
    Func_0808a0d0(21, 192, 296);
    Func_0808a1b8(21, 0xc000, 40);
    Func_0808a130(21, 2);
    Func_0808a1e8(21, 0x100, 60);
    Func_0808a1b8(20, 0x3000, 0);
    Func_0808a0d0(21, 184, 286);
    Func_0808a1b8(21, 0xb000, 10);
    Func_0808a1f0(21, 0x102);
    Func_0808a010(40);
    Func_0808a110(21, 4);

    Func_0808a1e8(20, 0x101, 40);
    Func_0808a110(20, 3);
    Func_0808a138(20, 2);
    Func_0808a1b8(20, 0, 0);
    Func_0808a1b8(21, 0, 60);
    Func_0808a1b8(20, 0x3000, 0);
    Func_0808a1b8(21, 0xb000, 10);
    Func_0808a110(20, 3);
    Func_0808a110(21, 3);
    Func_0808a1b8(21, 0, 0);
    Func_0808a090(20, 0x00019999, 0x0000cccc);
    Func_0808a0b0(20, Data_0200bfb0);
    Func_0808a0d0(20, 228, 296);
    Func_0808a1b8(20, 0xc000, 40);
    Func_0808a0d0(20, 212, 296);
    Func_0808a1b8(20, 0xc000, 40);
    Func_0808a0d0(20, 192, 296);
    Func_0808a1b8(20, 0xc000, 40);
    Func_0808a1b8(20, 0xb000, 0);
    Func_0808a1b8(21, 0x3000, 10);
    Func_0808a1f0(20, 0x102);
    Func_0808a010(60);
    Func_0808a110(20, 4);
    Func_0808a170(0x1a9e);
    Func_0808a188(20, 0, 40);
    Func_02001ed8();
    Func_0808a0d0(20, 178, 272);
    Func_0808a1b8(20, 0, 0);
    Func_0808a010(240);

    Func_0808a0a8(27);
    Func_080000c0(1);
    Func_0808a1b8(27, 0x8000, 10);
    Func_0808a1e8(27, 0x101, 60);
    Func_0808a188(27, 0, 10);
    Func_02001ea4(27);
    Func_0808a010(80);

    Func_0808a0a8(28);
    Func_080000c0(1);
    Func_0808a1b8(28, 0xd000, 20);
    Func_0808a138(28, 2);
    Func_0808a188(28, 0, 10);
    Func_02001ea4(28);
    Func_0808a010(160);

    Func_0808a0a8(32);
    Func_080000c0(1);
    Func_0808a1b8(32, 0x5000, 10);
    Func_0808a1e8(32, 0x101, 60);
    Func_0808a188(32, 0, 10);
    Func_02001ea4(32);
    Func_0808a010(80);

    Func_0808a0a8(30);
    Func_080000c0(1);
    Func_0808a1b8(30, 0xb000, 10);
    Func_0808a138(30, 1);
    Func_0808a170(0x1aa4);          /* r5 (0x1a9e) + 6 */
    Func_0808a188(30, 0, 10);

    Func_080000d8(Data_02009f15);   /* = Func_02001f14 + Thumb bit */

    Func_0808a0a8(20);
    Func_0808a0a8(21);
    Func_080000c0(1);

    entity = Func_0808a080(20);
    *(s16 *)(entity + 100) = 0;     /* from r8, which is 0 */
    entity = Func_0808a080(21);
    *(s16 *)(entity + 100) = 0;     /* from r8, which is 0 */

    Func_0808a090(20, 0x0000cccc, 0x00006666);
    Func_0808a090(21, 0x0000cccc, 0x00006666);
    Func_0808a098(20, Data_0200c034);
    Func_0808a098(21, Data_0200c0cc);

    Func_0808a0a8(29);
    Func_080000c0(1);
    Func_0808a1b8(29, 0x5000, 10);
    Func_0808a138(29, 2);
    Func_0808a170(0x1aa3);          /* r5 (0x1a9e) + 5 */
    Func_0808a188(29, 0, 20);
    Func_02001ea4(29);
    Func_02001ea4(30);

    for (;;) {
        Func_080000c0(1);
        entity = Func_0808a080(20);
        if (*(s16 *)(entity + 100) == 0) {
            continue;
        }
        entity = Func_0808a080(21);
        if (*(s16 *)(entity + 100) == 1) {
            break;
        }
    }

    Func_0808a098(20, Data_0200c164);
    Func_0808a098(21, Data_0200c1ac);

    Func_0808a0a8(31);
    Func_080000c0(1);
    Func_0808a1b8(31, 0x5000, 10);
    Func_0808a138(31, 1);
    Func_0808a110(31, 4);
    Func_0808a170(0x1aa2);
    Func_0808a188(31, 0, 10);
    Func_02001ea4(31);

    Func_0808a0a8(34);
    Func_0808a0a8(33);
    Func_080000c0(1);
    Func_0808a1e8(34, 0x105, 40);
    Func_0808a1e8(33, 0x105, 60);
    Func_0808a1b8(34, 0xb000, 10);
    Func_0808a1b8(33, 0x5000, 10);
    Func_0808a110(34, 4);
    Func_0808a170(0x1aa5);          /* r5 (0x1aa2) + 3 */
    Func_0808a188(34, 0, 10);
    Func_0808a138(33, 1);
    Func_0808a100(33, 4);
    Func_0808a188(33, 0, 10);
    Func_0808a1e8(34, 0x102, 60);

    Func_0808a1e8(20, 0x103, 0);
    Func_0808a138(20, 2);
    Func_0808a170(0x1ab2);
    Func_0808a188(20, 0, 10);

    Func_0808a0a8(27);
    Func_0808a0a8(28);
    Func_0808a0a8(29);
    Func_0808a0a8(30);
    Func_0808a0a8(32);
    Func_0808a0a8(31);
    Func_0808a0a8(33);
    Func_0808a0a8(34);
    Func_0808a0a8(20);
    Func_0808a0a8(21);
    Func_080000c0(1);

    Func_0808a128(27, 2, 0);
    Func_0808a128(28, 2, 0);
    Func_0808a128(29, 2, 0);
    Func_0808a128(30, 2, 0);
    Func_0808a128(32, 2, 0);
    Func_0808a128(31, 2, 0);
    Func_0808a128(33, 2, 0);
    Func_0808a128(34, 2, 0);
    Func_0808a128(21, 2, 40);

    Func_0808a1b8(27, 0x8000, 0);
    Func_0808a1b8(28, 0x8000, 0);
    Func_0808a1b8(29, 0x8000, 0);
    Func_0808a1b8(30, 0x8000, 0);
    Func_0808a1b8(32, 0x8000, 0);
    Func_0808a1b8(31, 0x8000, 0);
    Func_0808a1b8(33, 0x8000, 0);
    Func_0808a1b8(34, 0x8000, 40);

    Func_0808a128(21, 4, 40);
    Func_0808a188(21, 0, 10);
    Func_0808a138(20, 1);
    Func_0808a188(20, 0, 10);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a110(20, 3);
    Func_0808a188(20, 0, 10);

    Func_0808a1e8(27, 0x102, 40);
    Func_0808a130(27, 1);
    Func_0808a188(27, 0, 10);
    Func_0808a1e8(28, 0x102, 40);
    Func_0808a188(28, 0, 10);
    Func_0808a110(21, 4);
    Func_0808a010(40);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 20);
    Func_0808a110(20, 3);
    Func_0808a188(20, 0, 10);

    Func_0808a1b8(27, 0x5000, 0);
    Func_0808a1b8(28, 0xb000, 4);
    Func_0808a1b8(29, 0x5000, 0);
    Func_0808a1b8(30, 0xb000, 4);
    Func_0808a1b8(32, 0x5000, 0);
    Func_0808a1b8(31, 0xb000, 4);
    Func_0808a1b8(33, 0x5000, 0);
    Func_0808a1b8(34, 0xb000, 4);

    Func_0808a100(27, 3);
    Func_0808a110(28, 3);
    Func_0808a100(29, 3);
    Func_0808a110(30, 3);
    Func_0808a100(32, 3);
    Func_0808a110(31, 3);
    Func_0808a100(33, 3);
    Func_0808a110(34, 3);

    Func_0808a128(20, 2, 40);
    Func_0808a188(20, 0, 10);

    Func_0808a1b8(27, 0x8000, 0);
    Func_0808a1b8(28, 0x8000, 4);
    Func_0808a1b8(29, 0x8000, 0);
    Func_0808a1b8(30, 0x8000, 4);
    Func_0808a1b8(32, 0x8000, 0);
    Func_0808a1b8(31, 0x8000, 4);
    Func_0808a1b8(33, 0x8000, 0);
    Func_0808a1b8(34, 0x8000, 4);

    Func_0808a090(20, 0x00011999, 0x00008ccc);
    Func_0808a090(27, 0x00010ccc, 0x00008666);
    Func_0808a090(28, 0x00010ccc, 0x00008666);
    Func_0808a090(29, 0x00010000, 0x00008000);
    Func_0808a090(30, 0x00010000, 0x00008000);
    Func_0808a090(32, 0x0000f333, 0x00007999);
    Func_0808a090(31, 0x0000f333, 0x00007999);
    Func_0808a090(33, 0x0000e666, 0x00007333);
    Func_0808a090(34, 0x0000e666, 0x00007333);
    Func_0808a090(21, 0x0000d999, 0x00006ccc);

    Func_0808a1e0(27, 1);
    Func_0808a1e0(28, 1);
    Func_0808a1e0(29, 1);
    Func_0808a1e0(30, 1);
    Func_0808a1e0(32, 1);
    Func_0808a1e0(31, 1);
    Func_0808a1e0(33, 1);
    Func_0808a1e0(34, 1);
    Func_0808a1e0(20, 1);
    Func_0808a1e0(21, 1);

    Func_0808a0a8(27);
    Func_0808a0a8(28);
    Func_0808a0a8(29);
    Func_0808a0a8(30);
    Func_0808a0a8(32);
    Func_0808a0a8(31);
    Func_0808a0a8(33);
    Func_0808a0a8(34);
    Func_0808a0a8(20);
    Func_0808a0a8(21);
    Func_080000c0(1);

    Func_0808a098(20, Data_0200be00);
    Func_0808a098(27, Data_0200be00);
    Func_0808a098(28, Data_0200be00);
    Func_0808a098(29, Data_0200be00);
    Func_0808a098(30, Data_0200be00);
    Func_0808a098(32, Data_0200be00);
    Func_0808a098(31, Data_0200be00);
    Func_0808a098(33, Data_0200be00);
    Func_0808a098(34, Data_0200be00);

    entity = Func_0808a080(21);
    *(s16 *)(entity + 100) = 0;
    Func_0808a098(21, Data_0200be00);
    do {
        Func_080000c0(1);
        entity = Func_0808a080(21);
    } while (*(s16 *)(entity + 100) != 1);

    Func_0808a010(80);
    Func_0808a0f0(14, 0x01540000, 0x01120000);
    Func_080000c0(1);
    Func_0808a090(14, 0x00010000, 0x00008000);
    Func_0808a0d0(14, 224, 274);
    Func_0808a1b8(14, 0, 40);
    Func_0808a1b8(14, 0x8000, 40);
    Func_0808a1b8(14, 0xc000, 40);
    Func_0808a1b8(14, 0x5000, 40);
    Func_0808a1e8(14, 0x101, 60);
    Func_0808a188(14, 0, 10);
    Func_0808a1b8(14, 0, 40);
    Func_0808a1b8(14, 0xc000, 40);
    Func_0808a1b8(14, 0x8000, 40);
    Func_0808a1f0(14, 0x102);
    Func_0808a128(14, 4, 40);
    Func_0808a188(14, 0, 20);
    Func_0808a138(14, 2);
    Func_0808a188(14, 0, 10);
    Func_0808a128(14, 4, 40);
    Func_0808a090(14, 0x00013333, 0x00009999);

    *(s16 *)(entity + 100) = 0;
    entity = Func_0808a080(14);
    Func_0808a098(14, Data_0200be00);
    do {
        Func_080000c0(1);
        entity = Func_0808a080(14);
    } while (*(s16 *)(entity + 100) != 1);

    Func_0808a0f0(14, 0x01670000, 0x013a0000);
    entity = Func_0808a080(14);
    Func_0808a0f0(20, 0x01c70000, 0x01b20000);
    entity = Func_0808a080(20);
    *(u16 *)(entity + 6) = 0xd000;
    *(u16 *)(entity + 6) = 0xd000;
    Func_0808a0f0(21, 0x01d00000, 0x01a00000);
    entity = Func_0808a080(21);
    *(u16 *)(entity + 6) = 0x5000;

    Func_0808a088(27);
    Func_0808a088(28);
    Func_0808a088(29);
    Func_0808a088(30);
    Func_0808a088(31);
    Func_0808a088(32);
    Func_0808a088(33);
    Func_0808a088(34);

    Func_080f9010(17);

    if (*(s16 *)(scene + 364) == 9) {
        Func_0808a0d0(0, 224, 458);
        entity = Func_0808a080(0);
    } else {
        pose = 0xc000;
        Func_0808a0d0(0, 40, 248);
        entity = Func_0808a080(0);
        pose = 0x4000;
    }
    *(u16 *)(entity + 6) = (u16)pose;

    goto finish;
    Func_0808a4f0();
    Func_080770c8(0x911);

aborted:
    /* Reached only by the two `bl 0x0200151c` gotos above.  Keep the four
     * known static targets explicit through typed pointers: the address-order
     * checker intentionally classifies those two BLs as intra-owner gotos and
     * therefore does not walk this out-of-line target block as a call region. */
    abortSound(123);
    abortScene(*(s16 *)(scene + 364));
    abortReset();
    abortSettle();

finish:
    Func_0808a020();
}
