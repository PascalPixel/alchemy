#include "types.h"

/*
 * resource_3c6 cutscene script at 0x020007c4, 2,250 bytes
 * (0x020007c4-0x0200108d).
 *
 * Complete owner: `push {r5, r6, lr}` plus the 8-byte outgoing-argument frame
 * (`sub sp, #8`) at 0x020007c4, through the single interworking epilogue
 * `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02001086.  The epilogue
 * pops the return address into r0, so the owner is void.  Nothing reads
 * r0-r3 before they are first written, so it takes no arguments.
 *
 * Three literal pools sit inside the span.  The pool map was derived by a
 * control-flow walk from the prologue, not from a referenced-words listing;
 * each pool is proved unreachable as code by the branch that steps over it:
 *   0x02000880-0x0200088b  (0x09ba, 0x288e, 0x03001ebc) - the `bne` at
 *                          0x02000850 jumps to 0x0200088c and the `b.n` at
 *                          0x0200087e jumps to 0x020008b8, so nothing falls in;
 *   0x02000c10-0x02000c1b  (0x03001ebc, 0x6666, 0xcccc) - same shape, `bne` at
 *                          0x02000be0 to 0x02000c1c, `b.n` at 0x02000c0e to
 *                          0x02000c48;
 *   0x02000ff0-0x0200100b  (0x03001ebc, 0x09bf, 0x28a5, 0xcccc, 0x6666,
 *                          0x13333, 0x9999) - `beq` at 0x02000fde and `b.n` at
 *                          0x02000fee both to 0x0200100c.
 * No call argument is in flight across any of the three, but the walk is what
 * establishes that.
 *
 * LINK BASE 0x02008000 (witnesses recorded in resource_3c6_c_02000078.c).
 * None of the thirteen pool words falls in 0x0200_8xxx-0x0200_bxxx: they are
 * cue/dialogue ids, 16.16 constants, and three copies of 0x03001ebc, the
 * overlay's IWRAM workspace pointer cell - the same cell the byte-exact
 * resource_3c6_c_020001f0.c reads.
 *
 * Control flow: six conditionals, no loops.  Two of them (0x0200084e and
 * 0x02000bde) are the skip-beat shape: both arms run the same four calls and
 * both bump the u16 skip counter at workspace + 472, differing only in where
 * the bump sits relative to the last call.  They are written as two arms
 * anyway, because each arm's four `bl` sites are distinct sites.
 *
 * Call accounting: 250 `bl` sites, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3c6 07c4` - 32 distinct targets,
 * 249 import veneers plus one intra-overlay prologue call to Func_02001090;
 * no `call_via` site.  The multiset of `Func_...(` calls below equals that
 * multiset per target.  Every site is written exactly once: no site is reached
 * from two control paths, and no `bl` targets this owner's own epilogue.
 * (The inventory's `calls=230` is the usual undercount and is not the check.)
 *
 * UNCERTAINTY 1: the very first argument register is not set for the
 * Func_0808a018 call at 0x020007ce - r0 still holds whatever Func_080770c8
 * left there.  The sibling script Func_02000218 passes an explicit mode to the
 * same import, so this is written argument-less rather than inventing a value.
 * UNCERTAINTY 2: at 0x02000996 r3 is loaded with 2 as the value stored into
 * record + 0x55 and is still live at the `bl` at 0x020009a0.  That is the
 * documented phantom-argument idiom, so Func_0808a1e0 takes two arguments.
 * The same applies to the three Func_0808a0b8 sites, where r3 = 18 is the
 * second `ldrsh` offset.
 * UNCERTAINTY 3: Func_080091c8's fifth and sixth arguments are passed on the
 * stack from r5 and r6, which are set once (4 and 18) and reused by all four
 * sites.  Their meaning is not established here.
 */

/* Imports, reached through this overlay's veneer table.  Old-style
 * declarations: their interfaces vary by call site across the overlay.
 * Func_080770c0 is used in a condition, so it needs a real return type. */
void Func_080091c8();           /* six-argument scene presentation request */
s32 Func_080770c0();            /* query an audio/sequence cue */
void Func_080770c8();           /* start an audio/sequence cue */
void Func_080f9010();           /* audio/sequence cue by id */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* begin the scripted sequence */
void Func_0808a020();           /* end the scripted sequence */
void Func_0808a060();           /* scene resource request */
s32 Func_0808a070();            /* scene-state query */
u8 *Func_0808a080();            /* scene record by slot selector, or 0 */
void Func_0808a090();           /* per-slot reorientation request */
void Func_0808a0b8();           /* move a slot to (x, z) */
void Func_0808a0d0();           /* timed per-slot adjustment */
void Func_0808a0e0();           /* timed per-slot adjustment */
void Func_0808a0e8();           /* per-slot release */
void Func_0808a0f0();           /* place an entity at (x, z) */
void Func_0808a100();           /* per-record scene request */
void Func_0808a110();           /* scene-presentation request */
void Func_0808a128();           /* per-slot pairing request with a duration */
void Func_0808a138();           /* select an entity presentation */
void Func_0808a148();           /* per-slot pairing request */
void Func_0808a150();           /* per-slot pairing request */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a178();           /* dialogue-line variant with a mode word */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a1b8();           /* face the slot along a heading */
void Func_0808a1e0();           /* per-slot presentation select */
void Func_0808a1e8();           /* play animation id for n frames */
void Func_0808a570();           /* signed per-slot adjustment */
void Func_0808a578();           /* signed per-slot adjustment */
void Func_0808a580();           /* signed per-slot adjustment */

/* Intra-overlay call, resolved to the prologue at file offset 0x1090. */
void Func_02001090();

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

/* The u16 at workspace + 472 is the skip-beat counter: it is bumped on both
 * arms of every scene-variant test in this overlay. */
#define SKIP_BEATS (*(u16 *)(Data_03001ebc + 472))

void Func_020007c4(void)
{
    u8 *record;
    s16 *entity;

    Func_080770c8(2490);   /* 07ca */
    Func_0808a018();   /* 07ce */
    Func_0808a170(0x288e);   /* 07d4 */
    Func_0808a0d0(0, 104, 376);   /* 07e0 */
    Func_0808a1b8(0, 0, 0);   /* 07ea */
    Func_0808a570(1, -32, 0, 0);   /* 07f8 */
    Func_0808a570(3, -16, 16, 0xe000);   /* 0808 */
    Func_0808a570(2, 0, 16, 0xc000);   /* 0816 */
    Func_0808a0e8(1);   /* 081c */
    Func_0808a010(30);   /* 0822 */
    Func_0808a178(1, 0);   /* 082a */
    Func_0808a010(10);   /* 0830 */
    Func_0808a1b8(0, 0x8000, 0);   /* 083c */
    Func_0808a010(10);   /* 0842 */
    if (Func_0808a070(0, 0) == 0) {   /* 084a, tested at 0200084e */
        Func_0808a010(20);   /* 0854 */
        Func_0808a110(3, 4);   /* 085c */
        Func_0808a010(20);   /* 0862 */
        Func_0808a180(3, 0);   /* 086a */
        SKIP_BEATS++;
    } else {
        Func_0808a010(20);   /* 088e */
        Func_0808a110(3, 4);   /* 0896 */
        Func_0808a010(20);   /* 089c */
        SKIP_BEATS++;
        Func_0808a180(3, 0);   /* 08b4 */
    }

    Func_0808a010(10);   /* 08ba */
    Func_0808a1e8(2, 264, 40);   /* 08c6 */
    Func_0808a180(2, 0);   /* 08ce */
    Func_0808a010(10);   /* 08d4 */
    Func_0808a100(0, 3);   /* 08dc */
    Func_0808a010(40);   /* 08e2 */
    Func_0808a100(1, 3);   /* 08ea */
    Func_0808a100(2, 3);   /* 08f2 */
    Func_0808a110(3, 3);   /* 08fa */
    Func_0808a010(30);   /* 0900 */
    Func_0808a580(0, -16, 0);   /* 090c */
    Func_0808a578(2, 0, -16);   /* 0918 */
    Func_0808a578(3, 0, -8);   /* 0924 */
    Func_0808a580(0, 0, -16);   /* 0930 */
    Func_0808a100(3, 1);   /* 0938 */
    Func_0808a100(2, 1);   /* 0940 */
    Func_0808a1b8(0, 0xc000, 0);   /* 094c */
    Func_0808a1b8(1, 0xc000, 0);   /* 0958 */
    Func_0808a1b8(3, 0xc000, 0);   /* 0964 */
    Func_0808a1b8(2, 0xc000, 0);   /* 0970 */
    Func_0808a1b8(22, 0x4000, 0);   /* 097c */
    Func_0808a010(80);   /* 0982 */
    Func_0808a090(22, 0xcccc, 0x6666);   /* 098c */
    record = Func_0808a080(22);
    record[0x55] = 2;   /* 0992 */
    Func_0808a1e0(22, 2);   /* 09a0 */
    Func_080091c8(34, 0, 1, 2, 4, 18);   /* 09b4 */
    Func_080f9010(158);   /* 09ba */
    Func_0808a010(20);   /* 09c0 */
    Func_0808a0f0(22, 0x480000, 0x1380000);   /* 09ce */
    Func_0808a010(20);   /* 09d4 */
    Func_0808a580(22, 0, 16);   /* 09de */
    Func_080091c8(32, 0, 1, 2, 4, 18);   /* 09ee */
    Func_080f9010(159);   /* 09f4 */
    Func_0808a010(20);   /* 09fa */
    Func_0808a180(22, 0);   /* 0a02 */
    Func_0808a010(10);   /* 0a08 */
    Func_0808a100(0, 3);   /* 0a10 */
    Func_0808a100(1, 3);   /* 0a18 */
    Func_0808a100(2, 3);   /* 0a20 */
    Func_0808a110(3, 3);   /* 0a28 */
    Func_0808a010(40);   /* 0a2e */
    Func_0808a580(22, 16, 0);   /* 0a38 */
    Func_0808a1b8(22, 0x4000, 0);   /* 0a44 */
    Func_0808a010(20);   /* 0a4a */
    Func_0808a010(10);   /* 0a50 */
    Func_0808a110(22, 3);   /* 0a58 */
    Func_0808a010(30);   /* 0a5e */
    Func_0808a180(22, 0);   /* 0a66 */
    Func_0808a010(10);   /* 0a6c */
    Func_0808a110(3, 3);   /* 0a74 */
    Func_0808a010(30);   /* 0a7a */
    Func_0808a180(3, 0);   /* 0a82 */
    Func_0808a010(10);   /* 0a88 */
    Func_0808a138(22, 2);   /* 0a90 */
    Func_0808a010(20);   /* 0a96 */
    Func_0808a180(22, 0);   /* 0a9e */
    Func_0808a010(10);   /* 0aa4 */
    Func_0808a138(1, 2);   /* 0aac */
    Func_0808a010(20);   /* 0ab2 */
    Func_0808a180(1, 0);   /* 0aba */
    Func_0808a010(10);   /* 0ac0 */
    Func_0808a1e8(22, 258, 40);   /* 0acc */
    Func_0808a180(22, 0);   /* 0ad4 */
    Func_0808a010(10);   /* 0ada */
    Func_0808a1e8(2, 256, 40);   /* 0ae6 */
    Func_0808a180(2, 0);   /* 0aee */
    Func_0808a010(10);   /* 0af4 */
    Func_0808a110(22, 3);   /* 0afc */
    Func_0808a010(30);   /* 0b02 */
    Func_0808a180(22, 0);   /* 0b0a */
    Func_0808a010(10);   /* 0b10 */
    Func_0808a150(1, 0, 0);   /* 0b1a */
    Func_0808a150(3, 2, 0);   /* 0b24 */
    Func_0808a010(60);   /* 0b2a */
    Func_0808a1b8(0, 0xc000, 0);   /* 0b36 */
    Func_0808a1b8(1, 0xc000, 0);   /* 0b42 */
    Func_0808a1b8(2, 0xc000, 0);   /* 0b4e */
    Func_0808a1b8(3, 0xc000, 0);   /* 0b5a */
    Func_0808a010(40);   /* 0b60 */
    Func_0808a010(10);   /* 0b66 */
    Func_0808a110(22, 3);   /* 0b6e */
    Func_0808a010(30);   /* 0b74 */
    Func_0808a180(22, 0);   /* 0b7c */
    Func_0808a010(10);   /* 0b82 */
    Func_0808a1e8(3, 258, 40);   /* 0b8e */
    Func_0808a180(3, 0);   /* 0b96 */
    Func_0808a010(10);   /* 0b9c */
    Func_0808a110(22, 3);   /* 0ba4 */
    Func_0808a010(20);   /* 0baa */
    Func_0808a180(22, 0);   /* 0bb2 */
    Func_0808a010(10);   /* 0bb8 */
    Func_0808a1b8(1, 0xe000, 0);   /* 0bc4 */
    Func_0808a010(20);   /* 0bca */
    Func_0808a178(1, 0);   /* 0bd2 */
    if (Func_0808a070(0, 0) == 0) {   /* 0bda, tested at 02000bde */
        Func_0808a010(20);   /* 0be4 */
        Func_0808a110(22, 4);   /* 0bec */
        Func_0808a010(20);   /* 0bf2 */
        Func_0808a180(22, 0);   /* 0bfa */
        SKIP_BEATS++;
    } else {
        Func_0808a010(20);   /* 0c1e */
        Func_0808a110(22, 4);   /* 0c26 */
        Func_0808a010(20);   /* 0c2c */
        SKIP_BEATS++;
        Func_0808a180(22, 0);   /* 0c44 */
    }

    Func_0808a010(10);   /* 0c4a */
    Func_0808a1e8(2, 258, 40);   /* 0c56 */
    Func_0808a180(2, 0);   /* 0c5e */
    Func_0808a010(10);   /* 0c64 */
    Func_0808a1b8(1, 0xc000, 0);   /* 0c70 */
    Func_0808a1e8(22, 262, 50);   /* 0c7c */
    Func_0808a180(22, 0);   /* 0c84 */
    Func_0808a010(20);   /* 0c8a */
    Func_0808a0f0(25, 0x580000, 0x14c0000);   /* 0c98 */
    Func_0808a180(-1, 0);   /* 0ca2 */
    Func_0808a010(10);   /* 0ca8 */
    Func_0808a1e8(3, 256, 40);   /* 0cb4 */
    Func_0808a128(3, 4, 13);   /* 0cbe */
    Func_0808a128(3, 4, 30);   /* 0cc8 */
    Func_0808a180(3, 0);   /* 0cd0 */
    Func_0808a010(10);   /* 0cd6 */
    Func_0808a110(22, 3);   /* 0cde */
    Func_0808a010(30);   /* 0ce4 */
    Func_0808a180(22, 0);   /* 0cec */
    if (Func_080770c0(0x9bf) != 0) {   /* 0cf2, tested at 02000cf6 */
        Func_02001090();   /* 0cfa */
    }

    Func_0808a170(0x28a5);   /* 0d00 */
    Func_0808a010(10);   /* 0d06 */
    Func_0808a138(22, 2);   /* 0d0e */
    Func_0808a010(20);   /* 0d14 */
    Func_0808a180(22, 0);   /* 0d1c */
    Func_0808a010(10);   /* 0d22 */
    Func_0808a110(22, 3);   /* 0d2a */
    Func_0808a010(30);   /* 0d30 */
    Func_0808a180(22, 0);   /* 0d38 */
    Func_0808a010(10);   /* 0d3e */
    Func_0808a1e8(1, 256, 40);   /* 0d4a */
    Func_0808a180(1, 0);   /* 0d52 */
    Func_0808a010(10);   /* 0d58 */
    Func_0808a138(22, 2);   /* 0d60 */
    Func_0808a010(20);   /* 0d66 */
    Func_0808a180(22, 0);   /* 0d6e */
    Func_0808a010(20);   /* 0d74 */
    Func_0808a110(2, 3);   /* 0d7c */
    Func_0808a010(30);   /* 0d82 */
    Func_0808a180(2, 0);   /* 0d8a */
    Func_0808a010(10);   /* 0d90 */
    Func_0808a110(22, 3);   /* 0d98 */
    Func_0808a010(30);   /* 0d9e */
    Func_0808a090(25, 0xcccc, 0x6666);   /* 0da8 */
    Func_0808a0e0(25, 0, 16);   /* 0db2 */
    Func_0808a580(22, 0, 16);   /* 0dbc */
    Func_0808a010(30);   /* 0dc2 */
    Func_0808a0f0(25, 0, 0);   /* 0dcc */
    SKIP_BEATS++;
    Func_0808a060(242, 0);   /* 0de4 */
    Func_0808a010(10);   /* 0dea */
    record = Func_0808a080(22);
    record[0x5a] &= (u8)~1;   /* 0df0 */
    Func_0808a580(22, 0, -16);   /* 0e06 */
    record = Func_0808a080(22);
    record[0x5a] |= 1;   /* 0e0c */
    Func_0808a1b8(22, 0x4100, 0);   /* 0e22 */
    Func_0808a010(30);   /* 0e28 */
    Func_0808a010(10);   /* 0e2e */
    Func_0808a1e8(22, 256, 40);   /* 0e3a */
    Func_0808a180(22, 0);   /* 0e42 */
    Func_0808a010(10);   /* 0e48 */
    Func_0808a110(22, 3);   /* 0e50 */
    Func_0808a010(30);   /* 0e56 */
    Func_0808a180(22, 0);   /* 0e5e */
    Func_0808a010(20);   /* 0e64 */
    Func_0808a100(0, 3);   /* 0e6c */
    Func_0808a100(1, 3);   /* 0e74 */
    Func_0808a100(2, 3);   /* 0e7c */
    Func_0808a110(3, 3);   /* 0e84 */
    Func_0808a010(50);   /* 0e8a */
    Func_0808a010(10);   /* 0e90 */
    Func_0808a110(22, 3);   /* 0e98 */
    Func_0808a010(30);   /* 0e9e */
    Func_0808a580(22, -16, 0);   /* 0eaa */
    Func_0808a1b8(22, 0xc000, 0);   /* 0eb6 */
    Func_0808a010(20);   /* 0ebc */
    Func_080091c8(34, 0, 1, 2, 4, 18);   /* 0ed0 */
    Func_080f9010(158);   /* 0ed6 */
    Func_0808a010(10);   /* 0edc */
    Func_0808a580(22, 0, -16);   /* 0ee8 */
    Func_0808a0f0(22, 0, 0);   /* 0ef2 */
    Func_0808a010(10);   /* 0ef8 */
    Func_080091c8(32, 0, 1, 2, 4, 18);   /* 0f08 */
    Func_080f9010(159);   /* 0f0e */
    Func_0808a010(50);   /* 0f14 */
    Func_0808a1b8(0, 0x4000, 0);   /* 0f20 */
    Func_0808a148(1, 0, 0);   /* 0f2a */
    Func_0808a148(2, 0, 0);   /* 0f34 */
    Func_0808a010(20);   /* 0f3a */
    Func_0808a180(1, 0);   /* 0f42 */
    Func_0808a010(10);   /* 0f48 */
    Func_0808a138(3, 2);   /* 0f50 */
    Func_0808a010(20);   /* 0f56 */
    Func_0808a180(3, 0);   /* 0f5e */
    Func_0808a010(10);   /* 0f64 */
    Func_0808a110(2, 3);   /* 0f6c */
    Func_0808a010(30);   /* 0f72 */
    Func_0808a180(2, 0);   /* 0f7a */
    Func_0808a010(10);   /* 0f80 */
    Func_0808a100(0, 3);   /* 0f88 */
    Func_0808a010(40);   /* 0f8e */
    Func_0808a100(1, 3);   /* 0f96 */
    Func_0808a100(2, 3);   /* 0f9e */
    Func_0808a110(3, 3);   /* 0fa6 */
    Func_0808a010(30);   /* 0fac */
    Func_0808a090(1, 0x13333, 0x9999);   /* 0fb6 */
    Func_0808a090(3, 0x13333, 0x9999);   /* 0fc0 */
    Func_0808a090(2, 0x13333, 0x9999);   /* 0fca */
    Func_0808a100(1, 2);   /* 0fd2 */

    entity = (s16 *)Func_0808a080(0);   /* 0fd8 */
    if (entity != 0) {
        /* entity[5] and entity[9] are the +0x0a and +0x12 halfwords, the
         * integer parts of the 16.16 words at +0x08 and +0x10. */
        Func_0808a0b8(1, entity[5], entity[9]);   /* 0fea */
    }

    Func_0808a0e8(1);   /* 100e */
    Func_0808a0f0(1, 0, 0);   /* 1018 */
    Func_0808a100(3, 2);   /* 1020 */
    entity = (s16 *)Func_0808a080(0);   /* 1026 */
    if (entity != 0) {
        Func_0808a0b8(3, entity[5], entity[9]);   /* 1038 */
    }

    Func_0808a0e8(3);   /* 103e */
    Func_0808a0f0(3, 0, 0);   /* 1048 */
    Func_0808a100(2, 2);   /* 1050 */
    entity = (s16 *)Func_0808a080(0);   /* 1056 */
    if (entity != 0) {
        Func_0808a0b8(2, entity[5], entity[9]);   /* 1068 */
    }

    Func_0808a0e8(2);   /* 106e */
    Func_0808a0f0(2, 0, 0);   /* 1078 */
    Func_0808a010(10);   /* 107e */
    Func_0808a020();   /* 1082 */
}
