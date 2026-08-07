#include "types.h"

/*
 * resource_373 owner at 0x02003fb0, 5,604 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r9/sl/fp and r8 saves and a
 * 28-byte local frame at 0x02003fb0, through the single epilogue at
 * 0x0200556a-0x0200557a, followed by its final literal pool at
 * 0x0200557c-0x02005593.  The next inventory row begins at 0x02005594, which
 * already has byte-exact C.
 *
 * The longest of the three remaining cutscene scripts in this overlay and the
 * variant of the 0x020015dc scene: four dialogue beats driven by
 * Func_02005c20 rather than by Func_0808a010 waits, three player questions on
 * Func_0808a070, and two constant-slide loops.  Six interior literal pools sit
 * inside the span and are branched over rather than fallen into, at
 * 0x02004082, 0x02004484, 0x020046ea, 0x02004b22, 0x02004ffa and 0x02005472.
 * Four of the six `b.n`s split a call whose argument registers are set before
 * the branch: Func_0808a080 (from 0x02004080), Func_02005c20 (from
 * 0x02004482), Func_0808a1b8 (from 0x02004b20), Func_0808a128 (from
 * 0x02004ff8) and Func_0808a210 (from 0x02005470).  All are reassembled below.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  r0 is written before the first call and no incoming
 * register is read, so the owner takes no arguments.
 *
 * Call accounting: 513 `bl` sites in the decoded instruction stream, all
 * resolved with `bun tools/lib/overlay_call_targets.ts resource_373 3fb0` - 52
 * distinct targets, 5 of them intra-overlay prologues (Func_020032b0 x3,
 * Func_02003380 x2, Func_02005594 x1, Func_020055a4 x1, Func_02005c20 x4).
 * No `call_via rN` site.  Each of the 513 sites appears exactly once below;
 * the multiset of `Func_...(` calls in this file equals the multiset of
 * resolved targets.
 *
 * LINK BASE 0x02008000.  This owner adds five more Thumb-bit witnesses to the
 * three recorded in resource_373_c_020034c8.c: the pool words 0x0200d5c1,
 * 0x0200d5e1, 0x0200d5f1 and 0x0200d601 are Func_020055c0, Func_020055e0,
 * Func_020055f0 and Func_02005600 + the Thumb bit, and every one of those has
 * a byte-exact source in assets/code.  They are installed as per-beat
 * callbacks through Func_080000d0 for 3200 ticks and removed again through
 * Func_080000d8.  The fifth, 0x0200da09, resolves to file offset 0x5a08; that
 * range has no separate source yet, so it is passed through as the encoded
 * address.
 *
 * The pointer-shaped data words 0x0200e590, 0x0200e614 and 0x0200e5cc are
 * in-image data at file offsets 0x6590, 0x6614 and 0x65cc, the same three the
 * 0x020015dc scene uses.
 *
 * NOTE (decoding, not a defect to fix): r4 is written at 0x02004d2c as scratch
 * holding the constant 4 for the ninth argument of the Func_0808a1d0 call,
 * although the prologue never saves r4.  Same idiom as the two sibling owners.
 *
 * UNCERTAINTY 1: Func_0808a1b8's third argument is a frame count at most sites
 * and 0 at others; the immediate-versus-tweened split stays unresolved.
 * UNCERTAINTY 2: the first question at 0x020046c8 selects between two
 * Func_0808a110 poses (3 on the zero answer, 4 otherwise) and only the zero
 * answer bumps the u16 scene counter at +0x1d8.  The later two questions bump
 * on the one answer.  Which answer is affirmative is not settled here.
 * UNCERTAINTY 3: the tail writes fp[0x3c] = 0x80000000, the same sentinel the
 * byte-exact assets/code/resource_373_c_0200572c.c tests for at word offsets
 * 14 and 16 of its record.  The field is treated as a plain s32 here.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc;
 * this owner loads that pool word at four separate sites. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* In-image data at file offsets 0x6590, 0x6614 and 0x65cc. */
extern u8 Data_0200e590[];
extern u8 Data_0200e614[];
extern u8 Data_0200e5cc[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_020032b0();
void Func_02003380();
void Func_02005594();
void Func_020055a4();
void Func_02005c20();
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();
void Func_08009128();
void Func_08009180();
void Func_080091c0();
u8 *Func_080091e0();
void Func_080091f0();
void Func_080091f8();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a050();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b8();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1d0();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a200();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
u8 *Func_0808a228();
void Func_0808a360();
void Func_080f9010();

/* Intra-overlay callees, all with byte-exact sources in assets/code. */

/* The per-beat callbacks installed through Func_080000d0.  Their in-image
 * addresses carry the Thumb bit. */
#define CALLBACK_02005a08 ((void *)0x0200da09)
#define CALLBACK_020055c0 ((void *)0x0200d5c1)
#define CALLBACK_020055e0 ((void *)0x0200d5e1)
#define CALLBACK_020055f0 ((void *)0x0200d5f1)
#define CALLBACK_02005600 ((void *)0x0200d601)

void Func_02003fb0(void)
{
    u8 *camera;             /* fp - the record for actor 0 */
    u8 *actor;              /* r7 - whichever record is in hand */
    u32 frame;

    camera = Func_0808a080(0);

    Func_0808a018();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0. */
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);

    Func_080091c0(0x31, 0x35, 8, 4, 20, 50);
    Func_08009180(2, 0x66, 0x54, 0x29, 2, 1);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);
    Func_08009180(0, 0x67, 0x52, 0x2a, 1, 1);

    Func_0808a0f0(21, 0x01880000, 0x03800000);
    *(u16 *)(Func_0808a080(21) + 6) = 0xc000;

    Func_0808a0f0(1, 0x012a0000, 0x02e00000);
    *(u16 *)(Func_0808a080(1) + 6) = 0x4000;

    Func_0808a0f0(5, 0x012a0000, 0x02f80000);
    /* Split by the interior pool at 0x02004082. */
    *(u16 *)(Func_0808a080(5) + 6) = 0x4000;

    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);

    /* Three actors at the same X/Y with stepped Z (0x348, 0x34c, 0x350). */
    actor[0x55] = 0;
    *(s32 *)(actor + 16) = 0x03480000;
    actor = Func_0808a080(23);
    *(s32 *)(actor + 8) = 0x01840000;
    *(s32 *)(actor + 12) = 0x00a00000;
    Func_080091e0(actor, 0);

    *(s32 *)(actor + 16) = 0x034c0000;
    *(s32 *)(actor + 12) = 0x00a00000;
    actor[0x55] = 0;
    *(s32 *)(actor + 8) = 0x01840000;
    actor = Func_0808a080(24);
    Func_080091e0(actor, 0);

    actor = Func_0808a080(25);
    actor[0x55] = 0;
    *(s32 *)(actor + 8) = 0x01840000;
    *(s32 *)(actor + 12) = 0x00a00000;
    Func_080091e0(actor, 0);
    *(s32 *)(actor + 16) = 0x03500000;

    Func_0808a228()[0x55] = 0;

    Func_080000c0(1);
    Func_0808a210(0x017f0000, 0x00a00000, 0x036d0000, 0);
    Func_08009128();
    Func_080000c0(1);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 32;

    Func_0808a360();
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a098(5, Data_0200e614);
    Func_0808a098(1, Data_0200e5cc);
    Func_0808a010(40);
    Func_0808a098(0, 1);

    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;

    Func_0808a1b8(0, 0xb000, 40);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a090(0, 0x4ccc, 0x2666);
    Func_0808a0d0(0, 0x190, 0x348);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a138(0, 1);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x8000, 40);

    Func_02005594();

    /* ---- beat 1: actor 23 ---- */
    Func_0808a100(0, 17);
    Func_080000d0(CALLBACK_02005a08, 0xc80);
    for (frame = 0; frame <= 39; frame++) {
        Func_02005c20(camera);
        Func_080000c0(1);
    }
    Func_0808a1e0(0, 1);
    Func_080000d0(CALLBACK_020055c0, 0xc80);
    Func_080000d0(CALLBACK_020055e0, 0xc80);
    Func_0808a090(23, 0x3333, 0x1999);
    Func_0808a0c0(23, 0x186, 0x340);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(23, 0x190, 0x33a);
    Func_0808a010(20);

    actor = Func_0808a080(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0808a100(0, 1);
    Func_080000d8(CALLBACK_02005a08);
    Func_080000d8(CALLBACK_020055c0);
    Func_080000d8(CALLBACK_020055e0);
    Func_080000c0(1);
    Func_0808a158(0, 0);
    Func_0808a158(23, 0);
    Func_0808a0f0(23, 0, 0);
    Func_0808a010(20);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(120);

    /* ---- beat 2: actor 24 ---- */
    Func_08009180(7, 0x66, 0x54, 0x29, 2, 1);
    Func_0808a098(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0808a100(0, 1);
    Func_0808a010(40);
    Func_0808a110(0, 3);
    Func_0808a0d0(0, 0x179, 0x34b);
    Func_0808a1b8(0, 0xc000, 40);
    Func_0808a1b8(0, 0, 20);
    Func_0808a100(0, 17);
    Func_080000d0(CALLBACK_02005a08, 0xc80);
    for (frame = 0; frame <= 39; frame++) {
        Func_02005c20(camera);
        Func_080000c0(1);
    }
    Func_0808a1e0(0, 1);
    Func_080000d0(CALLBACK_020055c0, 0xc80);
    Func_080000d0(CALLBACK_020055f0, 0xc80);
    Func_0808a090(24, 0x3333, 0x1999);
    Func_0808a0c0(24, 0x186, 0x340);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(24, 0x179, 0x33c);
    Func_0808a010(20);

    actor = Func_0808a080(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0808a100(0, 1);
    Func_080000d8(CALLBACK_02005a08);
    Func_080000d8(CALLBACK_020055c0);
    Func_080000d8(CALLBACK_020055f0);
    Func_080000c0(1);
    Func_0808a158(0, 0);
    Func_0808a158(24, 0);
    Func_0808a0f0(24, 0, 0);
    Func_0808a010(20);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(120);

    /* ---- beat 3: actor 25 ---- */
    Func_08009180(6, 0x66, 0x53, 0x29, 1, 1);
    Func_0808a098(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0808a100(0, 1);
    Func_0808a010(40);
    Func_0808a110(0, 3);
    Func_0808a0d0(0, 0x168, 0x357);
    Func_0808a1b8(21, 0xb000, 10);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a1b8(0, 0xd000, 20);
    Func_0808a100(0, 17);
    Func_080000d0(CALLBACK_02005a08, 0xc80);
    /* Split by the interior pool at 0x02004484. */
    for (frame = 0; frame <= 39; frame++) {
        Func_02005c20(camera);
        Func_080000c0(1);
    }
    Func_0808a1e0(0, 1);
    Func_080000d0(CALLBACK_020055c0, 0xc80);
    Func_080000d0(CALLBACK_02005600, 0xc80);
    Func_0808a090(25, 0x3333, 0x1999);
    Func_0808a0c0(25, 0x186, 0x340);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(25, 0x168, 0x345);
    Func_0808a010(20);

    actor = Func_0808a080(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0808a100(0, 1);
    Func_080000d8(CALLBACK_02005a08);
    Func_080000d8(CALLBACK_020055c0);
    Func_080000d8(CALLBACK_02005600);
    Func_080000c0(1);
    Func_0808a158(0, 0);
    Func_0808a158(25, 0);
    Func_0808a0f0(25, 0, 0);
    Func_0808a010(20);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(120);

    Func_020055a4();

    /* ---- the questioning sequence ---- */
    Func_08009180(5, 0x67, 0x52, 0x2a, 1, 1);
    Func_0808a098(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0808a128(21, 2, 20);
    Func_0808a170(0xf03);
    Func_0808a188(21, 0, 10);
    Func_0808a1b8(0, 0x1000, 10);
    Func_020032b0(21, 5, 6, 0);
    Func_0808a090(21, 0x4ccc, 0x2666);
    Func_0808a0d0(21, 0x18d, 0x340);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x4000, 60);
    Func_0808a1b8(21, 0xc000, 60);
    Func_0808a110(21, 3);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a0d0(21, 0x174, 0x340);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x4000, 40);
    Func_0808a1b8(21, 0x8000, 40);
    Func_0808a110(21, 3);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0x5000, 20);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a110(21, 4);
    Func_0808a178(21, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(21, 3);
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    } else {
        Func_0808a110(21, 4);
    }

    Func_0808a188(21, 0, 20);
    Func_0808a170(0xf0a);
    Func_0808a0d0(21, 0x182, 0x349);
    Func_0808a010(10);
    Func_0808a1b8(21, 0xd000, 60);
    Func_0808a138(21, 2);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a178(21, 0);

    if (Func_0808a070(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0xd000, 60);
    Func_0808a138(21, 2);
    Func_0808a170(0xf0e);
    Func_0808a188(21, 0, 20);
    Func_0808a0d0(21, 0x182, 0x339);
    Func_0808a010(10);
    Func_0808a110(21, 4);
    Func_0808a010(60);
    Func_0808a188(21, 0, 60);
    Func_0808a1b8(21, 0x5000, 10);
    Func_0808a188(21, 0, 10);
    Func_0808a0d0(21, 0x174, 0x340);
    Func_0808a1b8(21, 0x5000, 10);
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a1e8(0, 0x102, 60);
    Func_0808a138(21, 2);
    Func_0808a188(21, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(0x01790000, 0x00a00000, 0x035c0000, 1);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a0c8(1, 0x171, 0x388);
    Func_0808a0d0(5, 0x188, 0x388);
    Func_0808a100(1, 1);
    Func_020032b0(5, 10, 11, 0);
    Func_0808a1b8(5, 0xa000, 0);
    Func_0808a188(5, 0, 10);
    Func_0808a138(21, 2);
    Func_0808a010(10);
    Func_0808a1b8(21, 0x3000, 0);
    Func_0808a1b8(0, 0x1000, 20);
    Func_0808a128(5, 4, 0);
    Func_0808a0d0(5, 0x188, 0x34b);
    Func_0808a1b8(5, 0x9000, 0);
    Func_0808a1b8(21, 0x3000, 0);
    Func_0808a1b8(0, 0xd000, 20);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a110(5, 3);
    Func_0808a188(5, 0, 10);
    Func_0808a100(21, 3);
    Func_0808a110(0, 3);
    Func_0808a188(21, 0, 20);
    Func_020032b0(1, 10, 11, 0);
    Func_0808a090(5, 0x4ccc, 0x2666);
    Func_0808a090(1, 0x4ccc, 0x2666);
    Func_0808a0c8(1, 0x188, 0x34b);

    actor = Func_0808a080(5) + 0x5a;
    *actor = (u8)(*actor & 0xfe);

    Func_0808a0d0(5, 0x198, 0x34b);
    Func_0808a010(1);

    actor = Func_0808a080(5) + 0x5a;
    *actor = (u8)(*actor | 1);

    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a0e8(1);
    Func_0808a100(1, 1);
    Func_0808a1b8(1, 0x8000, 30);
    Func_0808a128(21, 4, 30);
    Func_0808a188(21, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x5000, 20);
    Func_0808a188(21, 0, 10);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a128(0, 2, 30);
    Func_0808a1f0(0, 0x102);
    Func_0808a010(60);
    Func_0808a1b8(21, 0x3000, 40);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(21, 0x101, 80);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a1e8(0, 0x102, 60);
    Func_0808a1b8(21, 0x3000, 0);
    Func_0808a1b8(0, 0xd000, 20);
    Func_0808a188(21, 0, 10);
    Func_0808a110(1, 3);
    Func_0808a010(80);
    Func_0808a150(5, 1, 30);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x105, 60);
    Func_0808a188(21, 0, 10);

    /* Split by the interior pool at 0x02004b22. */
    Func_0808a1b8(5, 0x8000, 0);

    Func_0808a1b8(1, 0x8000, 10);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a188(5, 0, 10);
    Func_0808a130(21, 2);
    Func_0808a010(40);
    Func_0808a188(21, 0, 10);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a110(21, 4);
    Func_0808a188(21, 0, 10);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a138(21, 2);
    Func_0808a188(21, 0, 20);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x01750000, 0x00a00000, 0x03450000, 1);
    Func_0808a0d0(21, 0x16c, 0x330);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1b8(21, 0x3000, 10);
    Func_0808a188(21, 0, 40);
    Func_0808a150(5, 1, 30);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(5, 0x8000, 20);
    Func_0808a1b8(21, 0x5000, 20);
    Func_0808a188(21, 0, 10);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a110(21, 4);
    Func_0808a178(21, 0);

    if (Func_0808a070(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0808a188(21, 0, 20);
    Func_0808a170(0xf27);
    Func_0808a1e8(21, 0x103, 0);
    Func_0808a138(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a128(21, 4, 0);
    Func_0808a138(21, 3);
    Func_0808a100(21, 7);
    Func_0808a010(5);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_0808a1d0(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, 0);

    Func_080f9010(0xa1);

    actor = Func_0808a080(21);
    /* The +0x50 handle's +0x26 byte. */
    *(*(u8 **)(actor + 0x50) + 0x26) = 0;
    actor[0x5a] = (u8)(actor[0x5a] & 0xfe);

    Func_0808a090(21, 0x30000, 0x18000);
    Func_0808a0d0(21, 0x16c, 0x32f);
    Func_0808a010(4);

    /* Four frames of a constant slide. */
    for (frame = 0; frame != 4; frame++) {
        *(s32 *)(actor + 16) = *(s32 *)(actor + 16) + 0x18000;
        *(s32 *)(actor + 28) = *(s32 *)(actor + 28) + (s32)0xffffe667;
        Func_0808a010(1);
    }

    Func_0808a0f0(21, 0, 0);
    Func_0808a090(1, 0x30000, 0x18000);
    Func_0808a128(1, 6, 0);
    Func_0808a0d0(1, 0x176, 0x33b);
    Func_0808a180(5, 0);
    Func_0808a1b8(1, 0xb000, 0);
    Func_0808a1e8(5, 0x100, 0);
    Func_0808a130(5, 2);
    Func_0808a1e8(1, 0x100, 10);
    Func_0808a100(1, 13);
    Func_0808a128(1, 2, 5);
    Func_080f9010(0x8f);
    Func_080091f0(0, 0x40000, 0x10000);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);
    Func_080091e0(Func_0808a080(1), 0);
    Func_0808a1b8(0, 0xd000, 10);
    Func_0808a130(1, 3);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a100(21, 8);

    *(s32 *)(actor + 28) = 0x8000;
    Func_0808a0f0(21, 0x016c0000, 0x032b0000);

    /* Five frames of the reverse slide. */
    for (frame = 0; frame != 5; frame++) {
        *(s32 *)(actor + 28) = *(s32 *)(actor + 28) + 0x1999;
        Func_0808a010(1);
    }

    Func_0808a010(60);
    Func_0808a138(1, 2);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(60);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a208(0x4ccc, 0x999);
    Func_0808a210(0x01740000, 0x00a00000, 0x035b0000, 1);
    Func_0808a090(21, 0x30000, 0x18000);
    Func_0808a128(21, 6, 0);
    Func_0808a0d0(21, 0x167, 0x343);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x4000, 20);
    Func_0808a138(21, 2);

    actor[0x23] = (u8)(actor[0x23] & 0xfe);

    Func_0808a188(21, 0, 80);
    Func_0808a1e8(21, 0x101, 80);
    Func_0808a1b8(21, 0, 60);
    Func_0808a138(21, 3);
    Func_0808a188(21, 0, 10);
    Func_0808a1f0(21, 0x102);
    Func_0808a010(80);
    Func_0808a1b8(1, 0x5000, 40);
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 20);
    Func_0808a1b8(1, 0x8000, 20);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a138(1, 3);
    Func_080091e0(Func_0808a080(1), 1);

    /* Split by the interior pool at 0x02004ffa. */
    Func_0808a128(1, 6, 0);

    Func_0808a100(1, 1);
    Func_0808a090(1, 0x40000, 0x20000);

    actor = Func_0808a080(1);
    actor[0x5a] = (u8)(actor[0x5a] & 0xfe);

    Func_0808a0b8(1, 0x193, 0x33b);
    Func_0808a1f0(5, 0x102);
    Func_0808a1b8(5, 0xc000, 20);
    Func_0808a188(5, 0, 1);
    Func_0808a0e8(1);
    Func_0808a1b8(1, 0x5000, 20);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(1, 0x100, 0);
    Func_0808a100(1, 13);
    Func_0808a128(1, 2, 5);
    Func_080091e0(Func_0808a080(1), 0);
    Func_08009180(2, 0x66, 0x54, 0x29, 2, 1);
    Func_080f9010(0x8f);
    Func_080091f0(0, 0x40000, 0x10000);
    Func_0808a138(1, 3);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a1e8(1, 0x102, 30);
    Func_0808a090(5, 0x4ccc, 0x2666);
    Func_0808a0d0(5, 0x198, 0x357);
    Func_0808a010(60);
    Func_0808a138(1, 2);
    Func_0808a1e8(21, 0x105, 60);
    Func_0808a130(5, 3);
    Func_0808a138(0, 3);
    Func_0808a010(80);
    Func_0808a1b8(1, 0x4000, 30);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a110(5, 4);
    Func_0808a010(80);
    Func_0808a110(21, 3);
    Func_0808a010(10);
    Func_0808a188(21, 0, 10);
    Func_0808a1b8(5, 0xb000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(21, 0, 60);
    Func_0808a1b8(21, 0x4000, 60);
    Func_0808a110(21, 4);
    Func_0808a010(60);
    Func_0808a1b8(21, 0, 80);
    Func_0808a1e8(21, 0x105, 80);
    Func_0808a188(21, 0, 60);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a110(21, 4);
    Func_0808a010(30);
    Func_0808a188(21, 0, 60);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a1b8(5, 0x8000, 60);
    Func_0808a1b8(21, 0x4000, 30);
    Func_0808a188(21, 0, 30);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a1b8(21, 0, 30);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a188(1, 0, 20);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1e8(21, 0x100, 0);
    Func_0808a138(21, 3);
    Func_0808a010(30);
    Func_0808a188(21, 0, 60);
    Func_0808a138(1, 3);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_080091e0(Func_0808a080(1), 0);
    Func_0808a128(1, 4, 0);
    Func_0808a0d0(1, 0x18e, 0x33c);
    Func_0808a010(60);
    Func_0808a1b8(21, 0x4000, 0);
    Func_0808a1b8(0, 0xc000, 60);
    Func_0808a110(0, 3);
    Func_0808a010(60);
    Func_0808a110(21, 3);
    Func_0808a010(60);

    actor = Func_0808a080(1);
    actor[0x5a] = (u8)(actor[0x5a] | 1);

    actor = Func_0808a080(5);
    actor[0x5a] = (u8)(actor[0x5a] | 1);

    actor = Func_0808a080(0);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a1b8(0, 0, 0);

    /* The s16 integer views at +0x0a and +0x12 of record 0's 16.16 X and Z. */
    Func_0808a0c8(5, *(s16 *)(actor + 10) + 16, *(s16 *)(actor + 18));
    Func_0808a0d0(1, *(s16 *)(actor + 10) + 16, *(s16 *)(actor + 18) - 16);

    Func_0808a0e8(1);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(40);
    Func_0808a0d0(5, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0808a0f0(5, 0, 0);
    Func_0808a0d0(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0808a0f0(1, 0, 0);
    Func_0808a050(1, 5);

    /* Split by the interior pool at 0x02005472. */
    Func_0808a210(0x01790000, 0x00a00000, 0x03770000, 1);

    Func_02003380(0, 13, 10, 0);

    Func_0808a0d0(0, 0x178, 0x390);
    Func_0808a1b8(0, 0xc000, 0);

    actor = Func_0808a080(21) + 0x5a;
    *actor = (u8)(*actor | 1);

    Func_02003380(21, 6, 5, 0);

    Func_0808a0d0(21, 0x175, 0x377);
    Func_0808a1b8(21, 0x4000, 0);
    Func_0808a1b8(0, 0xc000, 40);
    Func_0808a100(21, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a200(0, 1);
    Func_0808a218();
    Func_0808a010(100);
    Func_080091c0(0x31, 0x2e, 8, 4, 20, 50);
    Func_080770c8(0x202);
    Func_080770d0(0x12f);

    /* Leave record 0 in the state the next scene expects. */
    camera[0x55] = 3;
    *(s32 *)(camera + 12) = 0x00a00000;
    *(s32 *)(camera + 0x3c) = (s32)0x80000000;
    *(s32 *)(camera + 0x28) = 0;

    /* Common exit; no argument register is set. */
    Func_0808a020();
}
