typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_38f owner at 0x020008ec, 7,356 bytes: code 0x020008ec-0x02002589
 * with nine interior literal pools that are branched over and are data, not
 * code (0x02000cba-0x02000ce3, 0x02000f8a-0x02000faf, 0x020011ea-0x02001213,
 * 0x020015bc-0x020015e7, 0x020018e8-0x020018f3, 0x02001a62-0x02001a67,
 * 0x02001e6a-0x02001e93, 0x02002378-0x0200239b, 0x0200258a-0x020025a7).  The
 * next inventory row starts at 0x020025a8 with its own `push {r5, lr}` and has
 * a byte-exact source, so the owner is exactly its advertised span.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 / push {r6, r7}`;
 * single epilogue at 0x0200257e `pop {r3, r5} / mov r8, r3 / mov sl, r5 /
 * pop {r5, r6, r7} / pop {r0} / bx r0`.  r0 holds the popped return address,
 * so the owner returns nothing, and no incoming argument register is read
 * before the first branch, so it takes none - the caller's r0-r3 reach
 * Func_0808a018 unchanged.  There is no stack frame: this owner passes at most
 * three arguments anywhere.
 *
 * Call accounting: 732 `bl` sites, 47 distinct targets, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 08ec` - 43 import veneers
 * and four intra-overlay prologues (Func_020008ac, Func_020008d8,
 * Func_020025a8, Func_020025c0), each of which has a byte-exact source in
 * assets/code whose signature is used here.  Every site appears below exactly
 * once.  (The inventory row advertises `calls=667`; the difference is the
 * sites inside the two `Func_080000c0(1)` spin loops and the repeated bodies
 * the row folds.)  The disassembler's own `bl` annotations are wrong in the
 * usual overlay way - an overlay `bl` stores `target_offset - 2` - and were
 * not used anywhere.
 *
 * LINK BASE, confirmed again and independently of the jump-table witness in
 * semantic/overlays/resource_38f_c_02000304.c: three pool words in this owner
 * are a known function start plus the Thumb bit under the 0x02008000 base.
 * 0x0200a609 - 0x8000 = 0x2609 = Func_02002608 + 1, 0x0200a7ad = 0x27ad =
 * Func_020027ac + 1, and 0x02008801 = 0x0801 = Func_02000800 + 1.  All three
 * are handed to Func_080000d0 / Func_080000d8 in the argument position that
 * takes a per-frame task, and all three of those functions are genuine
 * overlay owners (the first and third byte-exact in assets/code, the second
 * reconstructed as semantic/overlays/resource_38f_c_020027ac.c).  So they are
 * function pointers, not data addresses, and every Data_0200a/b* word below is
 * likewise in-image at `value - 0x8000`.
 *
 * That also settles what the three Data_0200b69x globals are.
 * Func_080000d0(Func_020027ac, 3200) starts the emitter this file's sibling
 * reconstructs; this owner then writes its state directly - Data_0200b690 = 0
 * (ramp), Data_0200b68c = 0x800000 (height, 128.0 in 16.16), Data_0200b694 = 1
 * (phase) - steps the phase to 2 and then 3, and spins on
 * `while (Data_0200b694 != 0) Func_080000c0(1)` until the emitter's phase-3
 * arm resets it.  The two are one mechanism read from both ends.
 *
 * Structure: one long scripted cutscene bracketed by Func_0808a018 /
 * Func_0808a020, driving actors 0/1/2/3 plus the fixed channels 17-22.
 * Data_0200b69c, set once from Func_080770c0(3) at entry, gates every
 * appearance of actor 3; each place where it is false instead bumps the scene
 * counter at scene+0x1d8, the same halfword resource_373's scripted owners
 * advance.  Three Func_0808a070 queries choose between alternative
 * continuations, the last of which splits the tail into two whole variants.
 *
 * Import shapes are the ones resource_373 and resource_39f settled for the
 * same family: Func_0808a010(frames) waits, Func_0808a080(selector) returns
 * the actor record (+8/+12/+16 the 16.16 X/Y/Z, +10/+18 their s16 integer
 * views), Func_0808a090(selector, sx, sz) sets a 16.16 scale pair,
 * Func_0808a0f0(selector, x, z) a 16.16 position, Func_0808a0b8 the integer
 * one, Func_080770c8(id) is the one-argument service.
 *
 * UNCERTAINTIES, recorded rather than guessed:
 *  - Func_0808a1b8/Func_020025c0's second argument is always a multiple of
 *    0x2000 built by shifting a small immediate; the values are reproduced
 *    exactly and read as 16.16 angles (0x10000 = one full turn would make
 *    0x4000 a quarter turn), but nothing here proves the unit.
 *  - Func_0808a330's first argument is 0x7fff, 0x10000, 0x00405210 or
 *    0x00406218 at different sites.  The last two are not addresses on this
 *    machine and are passed with a mode selector of 1, so they are packed
 *    constants; they are left as literals.
 *  - The +0x23 bit 0 and +0x5a bit 0 flips on the actor record are done with
 *    `& 0xfe` / `| 1` on a byte; which state each represents is not
 *    reconstructed.
 *  - Func_0808a218 does not appear here; Func_08009128, Func_0808a360,
 *    Func_0808a370 and Func_0808a4f0 are each reached with no argument
 *    register written since the preceding call, so they are spelled with no
 *    arguments.
 *  - At 0x02001e66 the original loads r0 = 0 and branches to the join at
 *    0x02001e94, where r0 is the first argument of Func_0808a130.  That is
 *    written out as Func_0808a130(0, 2) rather than preserved as a branch.
 */

/* The scene block's counter halfword, through the IWRAM pointer at
 * 0x03001ebc; 236 << 1 = 0x1d8 in the original. */
#define ADVANCE_SCENE_COUNTER() do {                                      \
    u16 *scene_counter_ = (u16 *)(*(u8 **)0x03001ebc + 0x1d8);           \
    *scene_counter_ = *scene_counter_ + 1;                                \
} while (0)

/* In-image data under the 0x02008000 link base. */
extern u8 Data_0200abd4[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ac3c[];
extern u8 Data_0200ac70[];
extern u8 Data_0200acfc[];
extern u8 Data_0200ad20[];
extern u8 Data_0200ad7c[];
extern s32 Data_0200b68c;
extern s32 Data_0200b690;
extern s32 Data_0200b694;
extern s32 Data_0200b698;
extern s32 Data_0200b69c;
extern s32 Data_0200b6a0;

/* Intra-overlay callees; signatures from their byte-exact sources. */
void Func_020008ac(s32, s32);
void Func_020008d8(void);
void Func_020025a8(s32, s32);
void Func_020025c0(s32, s32, s32);
void Func_02000800(void);
void Func_02002608(void);
void Func_020027ac(void);

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0a8();
void Func_0808a0b0();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a210();
void Func_0808a330();
void Func_0808a348();
void Func_0808a360();
void Func_0808a370();
void Func_0808a4f0();
void Func_08009128();
void Func_080091e0();
s32 Func_080770c0();
void Func_080770c8();
void Func_080f9010();
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();

/* `ldrb / and 0xfe / strb` and `ldrb / orr 1 / strb` on the record byte.
 * These were once four macros; they are written out at every use because each
 * use is a SEPARATE Func_0808a080 call site in the assembly and the per-target
 * call multiset is taken over sites. */

void Func_020008ec(void) {
    u8 *record;
    s32 *has_actor;

    Func_0808a018();
    has_actor = &Data_0200b69c;
    *has_actor = Func_080770c0(3);

    Func_080091e0(Func_0808a080(19), 0);
    Func_080091e0(Func_0808a080(20), 0);
    Func_080091e0(Func_0808a080(21), 0);
    Func_080091e0(Func_0808a080(22), 0);

    /* 208 << 15 and 128 << 17. */
    Func_0808a210(0x00680000, -1, 0x01000000, 0);
    Func_08009128();
    /* 184 << 13 and 247 << 16. */
    Func_0808a0f0(0, 0x00170000, 0x00f70000);
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();

    Func_0808a090(0, 0xcccc, 0x6666); /* 0.8 / 0.4 */
    Func_0808a0d0(0, 121, 238);
    Func_0808a090(1, 0x9999, 0x4ccc); /* 0.6 / 0.3 */
    Func_0808a090(2, 0x9999, 0x4ccc);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Func_0808a098(1, Data_0200abd4);
    Func_0808a098(2, Data_0200ac08);

    if (Data_0200b69c != 0) {
        Func_0808a090(3, 0x9999, 0x4ccc);
        record = Func_0808a080(0);
        if (record != 0) {
            Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
        }
        Func_0808a098(3, Data_0200ac3c);
    }

    Func_0808a0a0(2);
    Func_0808a1b8(0, 0xe000, 20);
    Func_0808a1b8(2, 0xe000, 20);
    Func_0808a1b8(1, 0x2000, 20);
    if (Data_0200b69c != 0) {
        Func_0808a1b8(3, 0x2000, 0);
    }
    Func_0808a1b8(0, 0x2000, 20);
    Func_0808a1b8(2, 0x2000, 20);
    Func_0808a1b8(1, 0xe000, 60);
    if (Data_0200b69c != 0) {
        Func_0808a1b8(3, 0xe000, 0);
    }
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a148(2, 0, 0);
    Func_0808a148(1, 0, 0);
    if (Data_0200b69c != 0) {
        Func_0808a148(3, 0, 0);
    }

    Func_0808a010(40);
    Func_0808a130(1, 1);
    Func_0808a170(0x1473);
    Func_020025a8(1, 20);
    Func_0808a148(0, 1, 0);
    Func_0808a110(0, 3);
    Func_0808a010(30);

    Func_0808a0d0(2, 72, 286);
    Func_0808a0d0(2, 72, 302);
    Func_0808a0d0(2, 88, 310);
    Func_0808a138(2, 1);
    Func_0808a148(2, 0, 0);
    Func_0808a010(20);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    if (Data_0200b69c != 0) {
        Func_0808a148(3, 2, 0);
    }
    Func_0808a010(30);
    Func_0808a100(1, 3);
    if (Data_0200b69c != 0) {
        Func_0808a100(3, 3);
    }
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(30);
    Func_0808a1b8(2, 0, 0);
    Func_0808a010(20);
    Func_020008ac(2, 9);
    Func_0808a010(40);
    Func_020008d8();

    Func_0808a1e8(2, 256, 40);
    Func_0808a090(2, 0x10000, 0x8000); /* 1.0 / 0.5 */
    {
        u8 *r_ = Func_0808a080(2);
        r_[0x5a] = r_[0x5a] & 0xfe;
    }
    Func_0808a0d0(2, 80, 310);
    Func_0808a010(1);
    {
        u8 *r_ = Func_0808a080(2);
        r_[0x5a] = r_[0x5a] | 1;
    }
    Func_0808a1e8(1, 258, 40);
    Func_020025a8(1, 20);
    Func_0808a1e8(2, 258, 40);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_0808a148(3, 0, 0);
    }
    Func_0808a150(2, 1, 60);
    if (Data_0200b69c != 0) {
        Func_0808a1b8(3, 0xc000, 0);
    }
    Func_0808a1b8(1, 0xc000, 0);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a130(1, 1);
    Func_020025a8(1, 20);
    Func_0808a090(2, 0x8000, 0x4000); /* 0.5 / 0.25 */
    {
        u8 *r_ = Func_0808a080(2);
        r_[0x5a] = r_[0x5a] & 0xfe;
    }
    Func_0808a0d0(2, 72, 286);
    Func_0808a010(1);
    {
        u8 *r_ = Func_0808a080(2);
        r_[0x5a] = r_[0x5a] | 1;
    }
    Func_0808a098(2, Data_0200ac08);

    if (Data_0200b69c != 0) {
        Func_0808a1e8(3, 0x105, 0);
        Func_0808a010(60);
        Func_020025a8(3, 20);
        Func_0808a100(3, 3);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    Func_0808a148(2, 0, 0);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a1f0(1, 258);
    Func_0808a010(10);
    Func_020025c0(1, 0x2000, 10);
    Func_020025c0(0, 0xa000, 10);
    Func_0808a178(1, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a100(1, 3);
    } else {
        Func_0808a110(1, 4);
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_4000;
        s32 angle_6000;
        s32 angle_2000;
        s32 angle_a000;
        s32 angle_c000;

        angle_2000 = 0x2000;
        Func_020025a8(1, 40);
        Func_020025c0(2, angle_2000, 40);
        Func_020025c0(2, 0x8000, 20);
        angle_4000 = 0x4000;
        Func_020025c0(2, angle_4000, 40);
        Func_0808a1e8(2, 0x101, 0);
        Func_0808a010(60);
        Func_0808a1b8(1, angle_4000, 0);
        angle_6000 = 0x6000;
        Func_020025c0(0, angle_6000, 60);
        Func_020025c0(3, angle_2000, 10);
        Func_0808a1b8(1, angle_2000, 0);
        angle_a000 = 0xa000;
        Func_020025c0(0, angle_a000, 10);
        Func_0808a1e8(1, 0x101, 0);
        Func_0808a1e8(0, 0x101, 0);
        Func_0808a010(40);
        Func_0808a1b8(1, angle_4000, 0);
        Func_020025c0(0, angle_6000, 10);
        Func_0808a138(1, 2);
        Func_0808a170(0x147b);
        Func_020025a8(1, 10);
        angle_c000 = 0xc000;
        Func_020025c0(2, angle_c000, 20);
        Func_0808a110(2, 3);
        Func_020025a8(2, 20);
        Func_020025c0(1, 0, 20);
        Func_020025c0(0, angle_a000, 40);
        Func_020025c0(1, angle_4000, 20);
        Func_020025c0(0, angle_6000, 30);
        Func_020025c0(1, angle_6000, 20);
        Func_020025c0(0, 0xe000, 30);
        Func_0808a138(2, 2);
        Func_0808a1e8(2, 256, 0);
        Func_0808a010(40);
        Func_0808a1b8(1, angle_4000, 0);
        Func_020025c0(0, angle_6000, 20);
        Func_020025c0(2, angle_c000, 10);
    }

    Func_080f9010(17);
    Func_080f9010(206);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(1);
    Func_080000c0(1);

    Data_0200b6a0 = 1;
    Func_080000d0(Func_02002608, 3200);
    Func_080000c0(20);
    Func_0808a330(0x00405210, 1);
    Func_0808a330(0x10000, 2);
    Func_0808a348(120);
    Func_080000c0(60);

    Func_0808a098(0, Data_0200ac70);
    Func_0808a098(1, Data_0200ac70);
    Func_0808a098(2, Data_0200ac70);
    Func_0808a098(3, Data_0200ac70);
    Func_0808a010(100);
    Func_020025a8(1, 20);
    Func_020025a8(2, 40);

    if (Data_0200b69c != 0) {
        Func_0808a010(40);
        Func_0808a1f0(3, 258);
        Func_0808a010(40);
        Func_020025a8(3, 40);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    Func_0808a010(20);

    if (Data_0200b69c != 0) {
        s32 scale_20000;

        record = Func_0808a080(3);
        scale_20000 = 0x20000;
        *(s32 *)(record + 0x28) = scale_20000;
        Func_0808a010(10);
        Func_0808a090(3, scale_20000, scale_20000);
        Func_0808a0e0(3, -2, 0);
        Func_0808a098(3, Data_0200acfc);
        Func_080091e0(Func_0808a080(3), 0);
        Func_0808a100(3, 19);
        Func_0808a010(10);
    }

    {
        s32 scale_20000;

        record = Func_0808a080(0);
        scale_20000 = 0x20000;
        *(s32 *)(record + 0x28) = scale_20000;
        Func_0808a010(10);
        Func_0808a090(0, scale_20000, scale_20000);
        Func_0808a098(0, Data_0200acfc);
        Func_080091e0(Func_0808a080(0), 0);
        Func_0808a100(0, 19);
        Func_0808a010(20);

        record = Func_0808a080(1);
        *(s32 *)(record + 0x28) = scale_20000;
        Func_0808a010(10);
        Func_0808a090(1, scale_20000, scale_20000);
        Func_0808a098(1, Data_0200acfc);
        Func_080091e0(Func_0808a080(1), 0);
        Func_0808a100(1, 19);
        Func_0808a010(40);

        record = Func_0808a080(2);
        *(s32 *)(record + 0x28) = scale_20000;
        Func_0808a010(10);
        Func_0808a098(2, Data_0200acfc);
        Func_080091e0(Func_0808a080(2), 0);
        Func_0808a100(2, 19);
    }

    Data_0200b6a0 = 0;
    Func_0808a010(160);
    Func_080000d8(Func_02002608);
    Func_0808a010(120);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    Data_0200b690 = 0;
    Data_0200b68c = 0x00800000; /* 128.0 in 16.16 */
    Data_0200b694 = 1;
    Func_080000d0(Func_020027ac, 3200);
    Func_0808a010(180);
    Func_080f9010(21);
    Func_020025a8(1, 80);
    Func_020025a8(2, 40);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a1f0(2, 258);
    Func_0808a1f0(3, 258);
    Func_0808a010(60);
    Func_020025a8(2, 20);

    Data_0200b694 = 2;
    Func_0808a130(2, 2);
    Func_0808a010(20);
    Func_0808a130(1, 1);
    Func_0808a010(40);
    Func_0808a130(0, 2);
    Func_0808a130(3, 1);
    Func_0808a010(20);
    Func_0808a130(2, 3);
    Func_0808a010(40);
    Func_0808a130(0, 1);
    Func_0808a010(20);
    Func_0808a130(1, 2);
    Func_0808a010(20);
    Func_0808a130(3, 2);
    Func_0808a1f0(1, 258);
    Func_020025a8(1, 20);

    if (Data_0200b69c != 0) {
        Func_0808a1f0(3, 258);
        Func_020025a8(3, 10);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        u8 *actor_data;
        s32 zero;

        zero = 0;
        Data_0200b694 = 3;
        {
            u8 *r_ = Func_0808a080(0);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_0808a080(1);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_0808a080(2);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        {
            u8 *r_ = Func_0808a080(3);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_0808a1e0(0, 3);
        Func_0808a1e0(1, 3);
        Func_0808a1e0(2, 3);
        Func_0808a1e0(3, 3);

        Data_0200b698 = zero;
        Func_080000d0(Func_02000800, 3200);
        Func_080f9010(220);

        {
            u8 *r_ = Func_0808a080(19);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_0808a1e0(19, 2);
        Func_0808a0f0(19, 0x00780000, 0x00f80000);
        actor_data = Data_0200ad20;
        Func_0808a098(19, actor_data);

        {
            u8 *r_ = Func_0808a080(20);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_0808a1e0(20, 2);
        Func_0808a0f0(20, 0x00640000, 0x01120000);
        Func_0808a098(20, actor_data);

        if (Data_0200b69c != 0) {
            {
                u8 *r_ = Func_0808a080(21);
                r_[0x23] = r_[0x23] & 0xfe;
            }
            Func_0808a1e0(21, 2);
            Func_0808a0f0(21, 0x004a0000, 0x00fe0000);
            Func_0808a098(21, actor_data);
        }

        {
            u8 *r_ = Func_0808a080(22);
            r_[0x23] = r_[0x23] & 0xfe;
        }
        Func_0808a1e0(22, 2);
        Func_0808a0f0(22, 0x005e0000, 0x00e10000);
        Func_0808a098(22, actor_data);
    }

    /* Wait for the emitter's phase-3 arm to clear the phase. */
    while (Data_0200b694 != 0) {
        Func_080000c0(1);
    }

    Func_0808a010(300);
    Func_080000d8(Func_020027ac);
    Func_0808a010(120);
    Func_080f9010(17);
    Func_0808a330(0x10000, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    Func_0808a0a8(19);
    Func_0808a0a8(20);
    if (Data_0200b69c != 0) {
        Func_0808a0a8(21);
    }
    Func_0808a0a8(22);
    Func_080000c0(1);
    Func_0808a098(19, Data_0200ad7c);
    Func_0808a098(20, Data_0200ad7c);
    if (Data_0200b69c != 0) {
        Func_0808a098(21, Data_0200ad7c);
    }
    Func_0808a0b0(22, Data_0200ad7c);

    Func_0808a010(80);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a178(1, 0);
    Func_0808a0f0(17, 0x00570000, 0x008b0000);
    Func_0808a0f0(18, 0x00570000, 0x008b0000);
    Func_080000c0(1);
    if (Func_0808a070(17, 0) == 1) {
        ADVANCE_SCENE_COUNTER();
    }
    Func_0808a138(0, 1);
    Func_0808a010(20);
    Func_0808a138(2, 2);
    Func_020025a8(2, 20);

    if (Data_0200b69c != 0) {
        Func_0808a138(3, 2);
        Func_0808a010(10);
        Func_0808a170(0x1488);
        Func_020025a8(3, 40);
    }

    Func_0808a130(1, 1);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(80);
    Func_0808a138(2, 2);
    Func_0808a170(0x1489);
    Func_020025a8(2, 40);
    Func_0808a138(1, 3);
    Func_0808a010(40);
    Func_0808a1e0(1, 2);
    {
        u8 *r_ = Func_0808a080(1);
        r_[0x23] = r_[0x23] | 1;
    }
    Func_080091e0(Func_0808a080(1), 1);
    Func_0808a128(1, 6, 0);
    Func_0808a100(1, 1);
    Func_020025c0(1, 0x4000, 60);
    Func_020025a8(1, 20);
    {
        s32 angle_2000;
        s32 angle_6000;

        Func_0808a130(1, 2);
        Func_020025a8(1, 10);
        angle_2000 = 0x2000;
        Func_0808a138(0, 3);
        Func_020025c0(1, angle_2000, 20);
        Func_0808a1e8(1, 0x101, 0);
        angle_6000 = 0x6000;
        Func_0808a010(40);
        Func_020025c0(1, angle_6000, 40);
        Func_020025c0(1, angle_2000, 20);
        Func_020025c0(1, angle_6000, 20);
        Func_020025c0(1, angle_2000, 10);
    }
    Func_0808a128(1, 2, 0);
    Func_0808a010(40);
    Func_0808a128(1, 2, 0);
    Func_0808a010(10);
    Func_0808a128(1, 4, 0);
    Func_0808a010(20);
    Func_020025a8(1, 20);

    if (Data_0200b69c != 0) {
        Func_0808a1e8(3, 256, 0);
        Func_0808a010(60);
        Func_0808a138(3, 2);
        Func_0808a010(80);
        Func_0808a1e0(3, 2);
        {
            u8 *r_ = Func_0808a080(3);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_080091e0(Func_0808a080(3), 1);
        Func_0808a128(3, 4, 0);
        Func_0808a0e0(3, -2, 0);
        Func_0808a100(3, 1);
        Func_020025c0(3, 0xe000, 60);
        Func_0808a138(3, 2);
        Func_0808a010(20);
        Func_020025a8(3, 20);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_2000;
        s32 angle_4000;
        s32 angle_6000;

        angle_4000 = 0x80;
        Func_0808a128(1, 2, 0);
        angle_4000 <<= 7;
        angle_2000 = 0x80;
        angle_2000 <<= 6;
        Func_020025c0(1, angle_4000, 20);
        Func_0808a110(1, 3);
        Func_020025c0(1, angle_2000, 10);
        Func_020025a8(1, 20);
        Func_0808a110(1, 3);
        Func_0808a010(10);
        Func_0808a138(2, 1);
        Func_0808a010(40);
        Func_0808a138(2, 2);
        Func_0808a010(20);
        Func_0808a1e0(2, 2);
        {
            u8 *r_ = Func_0808a080(2);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_080091e0(Func_0808a080(2), 1);
        Func_0808a128(2, 4, 0);
        Func_0808a100(2, 1);
        Func_0808a1b8(2, 0xc000, 0);
        Func_0808a138(0, 2);
        Func_0808a010(10);
        Func_0808a1e0(0, 2);
        {
            u8 *r_ = Func_0808a080(0);
            r_[0x23] = r_[0x23] | 1;
        }
        Func_080091e0(Func_0808a080(0), 1);
        angle_6000 = 0x6000;
        Func_0808a128(0, 4, 0);
        Func_0808a100(0, 1);
        Func_020025c0(0, angle_6000, 60);
        Func_0808a1e8(0, 0x105, 0);
        Func_0808a1e8(2, 0x105, 0);
        Func_0808a010(60);
        Func_020025c0(0, 0xa000, 20);
        Func_0808a110(1, 3);
        Func_0808a110(0, 3);
        Func_020025c0(0, angle_6000, 10);
        Func_020025c0(1, angle_4000, 10);
        Func_0808a1b8(2, 0xc000, 0);
        Func_0808a110(2, 3);
        Func_020025a8(2, 20);
        Func_0808a100(0, 3);
        Func_0808a100(3, 3);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        Func_0808a138(2, 1);
        Func_0808a010(20);
        Func_0808a178(2, 0);
        Func_0808a100(2, 3);
        Func_0808a1b8(2, 0xe000, 0);
        Func_0808a1b8(1, angle_2000, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a100(2, 3);
            Func_0808a110(1, 3);
            ADVANCE_SCENE_COUNTER();
        } else {
            Func_0808a138(1, 2);
            Func_020025c0(1, angle_2000, 10);
            Func_0808a130(1, 2);
            Func_0808a180(1, 0);
        }
    }

    Func_020025c0(1, 0x4000, 10);
    Func_0808a110(1, 4);
    Func_020025a8(1, 20);
    Func_020025c0(2, 0xc000, 10);
    Func_0808a110(2, 3);
    Func_020025a8(2, 10);

    if (Data_0200b69c != 0) {
        Func_0808a138(3, 2);
        Func_020025c0(3, 0, 20);
        Func_020025c0(3, 0x2000, 10);
        Func_0808a100(3, 4);
        Func_020025a8(3, 10);
    } else {
        ADVANCE_SCENE_COUNTER();
    }

    {
        s32 angle_2000;
        s32 angle_4000;
        s32 angle_6000;
        s32 angle_a000;

        angle_2000 = 0x2000;
        angle_a000 = 0xa000;
        Func_0808a1b8(1, angle_2000, 0);
        Func_020025c0(0, angle_a000, 10);
        Func_0808a100(0, 3);
        Func_0808a110(1, 3);
        Func_0808a010(20);
        angle_4000 = 0x80;
        angle_6000 = 0x6000;
        Func_0808a1b8(0, angle_6000, 0);
        angle_4000 <<= 7;
        Func_020025c0(1, angle_4000, 10);
        Func_0808a110(2, 4);
        Func_0808a010(20);
        Func_0808a1e8(0, 258, 0);
        Func_0808a1e8(1, 258, 80);
        Func_020025c0(2, 0xe000, 10);
        Func_0808a130(2, 2);
        Func_020025a8(2, 20);
        Func_0808a1b8(1, 0x2000, 0);
        Func_020025c0(0, angle_a000, 40);
        Func_0808a1b8(1, angle_4000, 0);
        Func_020025c0(0, 0x6000, 10);
        Func_020025c0(2, 0xc000, 10);
        Func_0808a110(2, 3);
        Func_020025a8(2, 10);
        Func_0808a1f0(1, 258);
        Func_0808a010(40);
        Func_020025a8(1, 20);
        Func_0808a110(2, 3);
        Func_0808a010(20);
        Func_0808a1f0(1, 258);
        Func_0808a010(40);
        Func_020025a8(1, 20);
        Func_0808a100(2, 3);
        Func_020025a8(2, 10);
        Func_0808a138(1, 2);
        Func_020025c0(1, 0x2000, 10);
        Func_0808a178(1, 0);
        Func_0808a1b8(0, angle_a000, 0);
    }

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(1, 3);
    } else {
        Func_0808a010(20);
        Func_0808a138(1, 2);
        Func_0808a010(40);
        ADVANCE_SCENE_COUNTER();
    }

    Func_0808a180(1, 0);
    Func_080f9010(21);
    Func_0808a330(0x00406218, 1);
    Func_0808a348(60);
    Func_080000c0(60);

    Data_0200b690 = 0;
    Data_0200b68c = 0x00800000;
    Data_0200b694 = 1;
    Func_080000d0(Func_020027ac, 3200);
    Func_0808a010(80);
    {
        s32 angle_c000;

        Func_0808a130(0, 2);
        Func_0808a130(1, 2);
        Func_0808a130(3, 2);
        Func_0808a138(2, 2);
        angle_c000 = 0xc000;
        Func_0808a010(60);
        Func_020025c0(2, angle_c000, 10);
        Func_0808a170(0x149d);
        Func_020025a8(2, 10);
        Func_020025c0(1, angle_c000, 10);
        Func_020025c0(0, angle_c000, 10);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, angle_c000, 10);
        }
    }

    {
        u8 *r_ = Func_0808a080(0);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_0808a080(1);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_0808a080(2);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    {
        u8 *r_ = Func_0808a080(3);
        r_[0x23] = r_[0x23] & 0xfe;
    }
    Func_0808a1e0(0, 3);
    Func_0808a1e0(1, 3);
    Func_0808a1e0(2, 3);
    Func_0808a1e0(3, 3);

    Data_0200b694 = 2;
    Func_080f9010(220);
    Func_0808a0f0(19, 0x00780000, 0x00f80000);
    Func_0808a098(19, Data_0200ad20);
    Func_0808a0f0(20, 0x00640000, 0x01120000);
    Func_0808a098(20, Data_0200ad20);
    if (Data_0200b69c != 0) {
        Func_0808a0f0(21, 0x004a0000, 0x00fe0000);
        Func_0808a098(21, Data_0200ad20);
    }
    Func_0808a0f0(22, 0x005e0000, 0x00e10000);
    Func_0808a098(22, Data_0200ad20);
    Func_0808a010(120);

    Data_0200b694 = 3;
    while (Data_0200b694 != 0) {
        Func_080000c0(1);
    }

    Func_020025a8(17, 80);
    Func_020025a8(18, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a1e8(3, 0x101, 0);
    Func_0808a010(60);
    Func_020025a8(18, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_020025a8(18, 10);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(3, 3);
    Func_0808a110(2, 3);
    Func_020025a8(18, 10);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 0);
    Func_0808a1e8(2, 256, 0);
    Func_0808a1e8(3, 256, 0);
    Func_0808a010(40);
    Func_020025a8(17, 10);
    Func_0808a1b8(0, 0x8000, 0);
    {
        s32 angle_c000 = 0xc0;
        s32 angle_e000;

        angle_c000 <<= 8;

        Func_0808a1b8(1, 0x4000, 0);
        Func_0808a1b8(3, 0, 0);
        Func_020025c0(2, angle_c000, 40);
        Func_020025a8(18, 10);
        Func_0808a1f0(0, 258);
        Func_0808a1f0(1, 258);
        Func_0808a1f0(2, 258);
        Func_0808a1f0(3, 258);
        Func_0808a1b8(0, angle_c000, 0);
        Func_0808a1b8(1, angle_c000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, angle_c000, 80);
        Func_020025a8(18, 10);
        Func_0808a1b8(0, 0x8000, 0);
        Func_0808a1b8(1, 0x4000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, 0, 40);
        Func_020025a8(17, 10);
        Func_0808a1b8(0, angle_c000, 0);
        Func_0808a1b8(1, angle_c000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, angle_c000, 10);
        Func_0808a100(0, 4);
        Func_0808a100(1, 4);
        Func_0808a100(3, 4);
        Func_0808a110(2, 4);
        Func_0808a010(60);
        Func_020025a8(18, 10);
        Func_0808a100(0, 3);
        Func_0808a100(1, 3);
        Func_0808a100(3, 3);
        Func_0808a110(2, 3);
        Func_020025a8(18, 20);
        Func_0808a1b8(0, 0x8000, 0);
        Func_0808a1b8(1, 0x4000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, 0, 20);
        Func_020025a8(18, 10);

        Func_0808a130(0, 2);
        Func_0808a130(1, 2);
        Func_0808a130(3, 2);
        Func_0808a138(2, 2);
        Func_0808a1b8(0, angle_c000, 0);
        Func_0808a1b8(1, angle_c000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_0808a1b8(3, angle_c000, 0);
        Func_020025a8(18, 20);
        Func_0808a1b8(0, 0x8000, 0);
        Func_0808a1b8(1, 0x4000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, 0, 20);
        Func_020025a8(17, 20);
        Func_0808a1f0(0, 258);
        Func_0808a1f0(1, 258);
        Func_0808a1f0(3, 258);
        Func_0808a1f0(2, 258);
        Func_0808a010(40);
        Func_020025a8(18, 10);
        Func_0808a1b8(0, angle_c000, 0);
        Func_0808a1b8(1, angle_c000, 0);
        Func_0808a1b8(2, angle_c000, 0);
        Func_020025c0(3, angle_c000, 10);
        Func_020025a8(18, 10);
        Func_0808a100(0, 3);
        Func_0808a100(1, 3);
        Func_0808a100(3, 3);
        Func_0808a110(2, 3);
        Func_0808a010(60);
        Func_0808a180(18, 0);
        Func_0808a180(17, 0);

        Func_080000d8(Func_020027ac);
        Func_0808a010(80);
        Func_0808a330(0x10000, 1);
        Func_0808a348(60);
        Func_080000c0(80);
        {
            s32 *has_actor_late;

            Func_0808a0a8(19);
            Func_0808a0a8(20);
            has_actor_late = &Data_0200b69c;
            Func_0808a0a8(21);
            Func_0808a0a8(22);
            Func_080000c0(1);
            Func_0808a098(19, Data_0200ad7c);
            Func_0808a098(20, Data_0200ad7c);
            if (*has_actor_late != 0) {
                Func_0808a098(21, Data_0200ad7c);
            }
            Func_0808a0b0(22, Data_0200ad7c);
        }
        Func_0808a010(20);

        Func_0808a1e0(0, 2);
        Func_0808a1e0(1, 2);
        Func_0808a1e0(2, 2);
        Func_0808a1e0(3, 2);
        {
            u8 *r_ = Func_0808a080(0);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_0808a080(1);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_0808a080(2);
            r_[0x23] = r_[0x23] | 1;
        }
        {
            u8 *r_ = Func_0808a080(3);
            r_[0x23] = r_[0x23] | 1;
        }

        angle_e000 = 0xe000;
        Func_0808a138(2, 2);
        Func_020025c0(2, angle_e000, 10);
        Func_0808a178(2, 0);
        Func_0808a1b8(1, 0x2000, 0);
        Func_0808a1b8(3, 0, 0);

        if (Func_0808a070(0, 0) == 0) {
            Func_0808a138(1, 2);
            Func_0808a010(10);
            Func_0808a178(1, 0);
            if (Func_0808a070(0, 0) == 0) {
                Func_020025c0(3, 0, 20);
                Func_0808a1e8(1, 0x101, 0);
                Func_0808a1e8(2, 0x101, 0);
                Func_0808a1e8(3, 0x101, 0);
                Func_0808a010(40);
                Func_020025c0(1, 0x4000, 20);
                Func_020025a8(1, 10);
                Func_020025c0(2, angle_c000, 20);
                Func_020025c0(2, angle_e000, 20);
                Func_0808a110(2, 3);
                Func_020025a8(2, 20);
                Func_020025c0(1, 0x2000, 20);
            } else {
                Func_020025c0(3, 0, 20);
                Func_0808a1e8(1, 258, 0);
                Func_0808a1e8(2, 258, 0);
                Func_0808a1e8(3, 258, 40);
                Func_020025c0(1, 0x4000, 20);
                Func_0808a170(0x14b4);
                Func_020025a8(1, 20);
                Func_0808a110(2, 3);
                Func_020025a8(2, 20);
            }
            Func_0808a100(3, 3);
            Func_0808a110(1, 3);
            Func_0808a110(2, 3);
        } else {
            Func_0808a010(20);
            Func_0808a110(1, 3);
            Func_0808a010(10);
            Func_0808a170(0x14b6);
            Func_020025a8(1, 10);
            Func_0808a1b8(1, 0x4000, 0);
            Func_020025c0(0, 0x6000, 20);
            Func_0808a100(1, 3);
            Func_0808a110(0, 3);
            Func_0808a010(10);
            Func_0808a110(2, 4);
            Func_0808a178(2, 0);

            if (Func_0808a070(0, 0) == 0) {
                Func_0808a010(20);
                Func_0808a1e8(2, 0x103, 0);
                Func_0808a010(40);
                Func_020025c0(2, angle_e000, 10);
                Func_020025a8(2, 10);
                if (Data_0200b69c != 0) {
                    Func_020025c0(3, 0, 10);
                    Func_0808a130(3, 3);
                    Func_020025a8(3, 20);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_0808a1f0(0, 258);
                Func_0808a1f0(1, 258);
                Func_0808a010(40);
                Func_0808a138(1, 2);
                Func_020025a8(1, 20);
                Func_0808a1e8(1, 0x105, 0);
                Func_0808a010(120);
                Func_020025a8(2, 40);
                if (Data_0200b69c != 0) {
                    Func_020025c0(3, 0x2000, 10);
                    Func_0808a110(3, 4);
                    Func_020025a8(3, 10);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_0808a010(60);
                Func_0808a138(2, 2);
                if (Data_0200b69c != 0) {
                    Func_020025c0(2, 0xa000, 40);
                    Func_020025c0(2, 0xe000, 20);
                }
                Func_020025a8(2, 10);
                Func_0808a130(0, 2);
                Func_0808a138(1, 2);
                Func_0808a010(40);
                Func_020025a8(2, 20);
                Func_0808a110(0, 3);
                Func_0808a110(1, 3);
                Func_0808a010(20);
                Func_0808a100(3, 3);
                Func_0808a110(2, 3);
            } else {
                Func_0808a1e8(2, 0x105, 0);
                Func_0808a010(40);
                Func_0808a110(2, 3);
                Func_0808a170(0x14bf);
                Func_020025a8(2, 20);
                if (Data_0200b69c != 0) {
                    Func_020025c0(3, 0, 10);
                    Func_0808a130(3, 1);
                    Func_020025a8(3, 20);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_0808a1f0(1, 258);
                Func_0808a1f0(0, 258);
                Func_0808a010(40);
                Func_0808a138(1, 2);
                Func_020025a8(1, 20);
                Func_0808a1e8(2, 0x105, 0);
                Func_0808a010(80);
                Func_020025a8(2, 40);
                if (Data_0200b69c != 0) {
                    Func_020025c0(3, 0x2000, 20);
                    Func_0808a100(3, 4);
                    Func_020025a8(3, 40);
                } else {
                    ADVANCE_SCENE_COUNTER();
                }
                Func_0808a138(2, 2);
                Func_0808a010(20);
                Func_020025a8(2, 20);
                Func_0808a130(1, 2);
                Func_0808a138(0, 2);
                Func_0808a010(40);
                Func_020025a8(2, 20);
            }
        }
    }

    Func_080f9010(17);
    Func_0808a090(1, 0x13333, 0x9999); /* 1.2 / 0.6 */
    Func_0808a090(2, 0x13333, 0x9999);
    Func_0808a090(3, 0x13333, 0x9999);

    Func_0808a100(1, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(2, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0808a0e8(2);
    Func_0808a0f0(2, 0, 0);

    Func_0808a0f0(17, 0, 0);
    Func_0808a0f0(18, 0, 0);

    if (Data_0200b69c != 0) {
        Func_0808a100(3, 2);
        record = Func_0808a080(0);
        if (record != 0) {
            Func_0808a0b8(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0808a0e8(3);
        Func_0808a0f0(3, 0, 0);
    }

    Func_080770c8(0x843);
    Func_0808a4f0();
    Func_0808a020();
}
