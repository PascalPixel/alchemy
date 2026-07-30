typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_373 owner at 0x020015dc, 5,240 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r9/sl/fp and r8 saves and a
 * 36-byte local frame at 0x020015dc, through the single epilogue at
 * 0x02002a26-0x02002a36, followed by its final literal pool at
 * 0x02002a38-0x02002a53.  The next inventory row begins at 0x02002a54.
 *
 * A long cutscene script: straight-line apart from three `if`s on
 * Func_0808a070 and two counted loops.  Four interior literal pools sit inside
 * the span and are branched over rather than fallen into, at 0x02001a12,
 * 0x02001e58, 0x02002282 and 0x020026dc.  In three of those cases the `b.n`
 * splits a single call - its argument registers are set before the branch and
 * the `bl` itself is on the far side - so the reassembled calls are
 * Func_08009180 (from 0x02001a10), Func_020032b0 (from 0x02001e56) and
 * Func_0808a188 (from 0x020026da).  The fourth, at 0x02002280, jumps to the
 * `cmp r5, #4 / bne` that closes the first counted loop; those two halfwords
 * sit at 0x020022bc, immediately after the pool, and the disassembler
 * mis-splits them because the pool's last word ends on an odd halfword
 * boundary.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue's first call takes no argument
 * register, and nothing here reads r0-r3 on entry, so the owner takes no
 * arguments.
 *
 * Call accounting: 496 `bl` sites in the decoded instruction stream, all
 * resolved with `bun tools/overlay_call_targets.ts resource_373 15dc` - 46
 * distinct targets, 490 import veneers and 6 intra-overlay prologues
 * (Func_0200345c x1, Func_020032b0 x3, Func_02003380 x2).  No `call_via rN`
 * site.  Each of the 496 sites appears exactly once below; the multiset of
 * `Func_...(` calls in this file equals the multiset of resolved targets.
 * (The inventory's `calls=452` counts distinct targets per the usual
 * undercount, not sites.)
 *
 * LINK BASE 0x02008000, as established in resource_373_c_020034c8.c from
 * three Thumb-bit function-pointer witnesses.  The pointer-shaped pool words
 * here - 0x0200e590, 0x0200e614 and 0x0200e5cc - are therefore in-image data
 * at file offsets 0x6590, 0x6614 and 0x65cc, not RAM globals.
 *
 * The owner saves and restores one byte of shared state across itself: the
 * +0x55 flag of record 0 is read into the frame at 0x02001684, cleared, and
 * written back from the frame in the tail at 0x02002a1a.  That is the only
 * value that outlives the body.
 *
 * NOTE (decoding, not a defect to fix): r4 is written at 0x0200220e as scratch
 * holding the constant 4 for the ninth argument of the Func_0808a1d0 call,
 * although the prologue never saves r4.  Same idiom as 0x020034c8 and the
 * resource_371/372 sites recorded in HANDOVER.
 *
 * UNCERTAINTY 1: Func_0808a1b8's third argument is a frame count at most sites
 * and 0 at others; the immediate-versus-tweened split is unresolved, as in the
 * sibling owners.
 * UNCERTAINTY 2: Func_0808a070's result is compared against 0 at 0x02001b8e
 * and against 1 at 0x02001c12 and 0x0200219c.  Both polarities bump the same
 * u16 scene counter at +0x1d8, so the predicate is a yes/no player answer;
 * which answer is which is not settled here.
 * UNCERTAINTY 3: at 0x020028ec and 0x02002906 the s16 integer views at +0x0a
 * and +0x12 of record 0 are used as placement coordinates with +16/-16 nudges.
 * The halfword-view reading is the one HANDOVER records for the 16.16 words at
 * +0x08 and +0x10; the nudge units are pixels on that reading.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc.
 * Two different owners load this same pool word (0x02001a28 and 0x020022a8). */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* In-image data at file offsets 0x6590, 0x6614 and 0x65cc. */
extern u8 Data_0200e590[];
extern u8 Data_0200e614[];
extern u8 Data_0200e5cc[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_08009128();
void Func_08009180();
void Func_080091c0();
void Func_080091e0();
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
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a200();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
u8 *Func_0808a228();
void Func_0808a360();

/* Intra-overlay callees. */
void Func_0200345c();
void Func_020032b0();
void Func_02003380();

void Func_020015dc(void)
{
    u8 *record;             /* r6 - whichever actor record is in hand */
    u8 *savedFlagSlot;      /* [sp,#28] - &record0[0x55] */
    u8 savedFlag;           /* [sp,#32] - its value on entry */
    u8 *handle;
    s32 step;               /* r5 in the two counted loops */

    Func_0808a018();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0. */
    Func_0808a210(-1, -1, -1, 0);

    record = Func_0808a228();
    record[0x55] = 0;
    Func_0808a210(0x017f0000, 0x00a00000, 0x036d0000, 0);

    Func_0808a010(1);
    Func_08009128();
    Func_080091c0(0x31, 0x29, 7, 3, 20, 50);
    Func_08009180(2, 0x66, 0x54, 0x29, 2, 1);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);
    Func_08009180(0, 0x67, 0x52, 0x2a, 1, 1);

    /* Record 0's +0x55 flag is stashed in the frame and cleared for the whole
     * scene; the tail puts it back. */
    record = Func_0808a080(0);
    savedFlagSlot = record + 0x55;
    savedFlag = *savedFlagSlot;
    *savedFlagSlot = 0;

    Func_0808a0f0(0, 0x01970000, 0x02b20000);
    Func_0808a0f0(21, 0x01880000, 0x03800000);
    Func_0808a0f0(1, 0x012a0000, 0x02e00000);
    Func_0808a0f0(5, 0x012a0000, 0x02f80000);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(21, 0xc000, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);

    Func_0200345c(23, 2, 1);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c0) = 0;
    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 32;

    Func_0808a360();
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a098(5, Data_0200e614);
    Func_0808a098(1, Data_0200e5cc);
    Func_0808a098(0, 1);

    /* 128 << 9 into record 0's +0x18 / +0x1c pair; repeated three more times
     * below, once per dialogue beat. */
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;

    Func_0808a1b8(0, 0xb000, 40);
    Func_0808a110(0, 3);
    Func_0808a010(10);
    Func_0808a090(0, 0x4ccc, 0x2666);
    Func_0808a0d0(0, 0x194, 0x34b);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a138(0, 1);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x8000, 40);
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a158(0, 2);
    Func_0808a158(23, 2);
    Func_0808a090(23, 0x4ccc, 0x2666);
    Func_0808a0c0(23, 0x186, 0x340);
    Func_0808a010(80);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(23, 0x192, 0x33c);
    Func_0808a010(80);
    Func_0808a158(0, 0);
    Func_0808a158(23, 0);
    Func_0808a0f0(23, 0x01860000, 0x034a0000);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(200);

    Func_08009180(7, 0x66, 0x54, 0x29, 2, 1);
    Func_0808a098(0, 1);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_0808a100(0, 1);
    Func_0808a010(30);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a0d0(0, 0x179, 0x34b);
    Func_0808a010(10);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a1b8(0, 0, 20);
    Func_0808a158(0, 2);
    Func_0808a158(23, 2);
    Func_0808a090(23, 0x4ccc, 0x2666);
    Func_0808a0c0(23, 0x186, 0x340);
    Func_0808a010(80);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(23, 0x179, 0x33c);
    Func_0808a010(80);
    Func_0808a158(0, 0);
    Func_0808a158(23, 0);
    Func_0808a0f0(23, 0, 0);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(200);

    Func_08009180(6, 0x66, 0x53, 0x29, 1, 1);
    Func_0808a098(0, 1);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_0808a100(0, 1);
    Func_0808a010(30);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a0d0(0, 0x168, 0x357);
    Func_0808a1b8(21, 0xb000, 10);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a1b8(0, 0xd000, 20);
    Func_0808a158(0, 2);
    Func_0808a158(24, 2);
    Func_0808a090(24, 0x4ccc, 0x2666);
    Func_0808a0c0(24, 0x186, 0x340);
    Func_0808a010(80);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(24, 0x168, 0x34a);
    Func_0808a010(80);
    Func_0808a158(0, 0);
    Func_0808a158(24, 0);
    Func_0808a0f0(24, 0, 0);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a010(200);

    /* Split by the interior pool at 0x02001a12. */
    Func_08009180(5, 0x67, 0x52, 0x2a, 1, 1);

    Func_0808a098(0, 1);
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_0808a170(0xf03);
    Func_0808a128(21, 2, 20);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(0, 0x1000, 20);
    Func_020032b0(21, 5, 6, 0);
    Func_0808a090(21, 0x4ccc, 0x2666);
    Func_0808a0d0(21, 0x18d, 0x340);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x4000, 60);
    Func_0808a1b8(21, 0xc000, 60);
    Func_0808a110(21, 3);
    Func_0808a110(21, 3);
    Func_0808a010(10);
    Func_0808a188(21, 0, 20);
    Func_0808a0d0(21, 0x174, 0x340);
    Func_0808a010(20);
    Func_0808a1b8(21, 0x4000, 40);
    Func_0808a1b8(21, 0x8000, 40);
    Func_0808a110(21, 3);
    Func_0808a110(21, 3);
    Func_0808a010(10);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a110(21, 3);
    Func_0808a010(10);
    Func_0808a188(21, 0, 20);
    Func_0808a138(0, 2);
    Func_0808a010(40);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a178(21, 0);

    /* First question: the counter runs on the zero answer. */
    if (Func_0808a070(0, 0) == 0) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a170(0xf0a);
    Func_0808a0d0(21, 0x182, 0x349);
    Func_0808a010(10);
    Func_0808a1b8(21, 0xd000, 60);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a178(21, 0);

    /* Second question: the counter runs on the one answer. */
    if (Func_0808a070(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(21, 0xd000, 60);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a170(0xf0e);
    Func_0808a188(21, 0, 20);
    Func_0808a0d0(21, 0x182, 0x339);
    Func_0808a010(10);
    Func_0808a110(21, 4);
    Func_0808a010(60);
    Func_0808a188(21, 0, 60);
    Func_0808a1b8(21, 0x5000, 10);
    Func_0808a188(21, 0, 20);
    Func_0808a0d0(21, 0x174, 0x340);
    Func_0808a1b8(21, 0x5000, 20);
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a1e8(0, 0x102, 60);
    Func_0808a138(21, 2);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a010(30);
    Func_0808a110(21, 3);
    Func_0808a010(20);
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
    Func_0808a1b8(0, 0x1000, 30);
    Func_0808a128(5, 4, 0);
    Func_0808a0d0(5, 0x188, 0x34b);
    Func_0808a1b8(5, 0x9000, 0);
    Func_0808a1b8(21, 0x3000, 0);
    Func_0808a1b8(0, 0xd000, 40);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a110(5, 3);
    Func_0808a010(20);
    Func_0808a188(5, 0, 20);
    Func_0808a100(21, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);

    /* Split by the interior pool at 0x02001e58. */
    Func_020032b0(1, 10, 11, 0);

    Func_0808a090(5, 0x4ccc, 0x2666);
    Func_0808a090(1, 0x4ccc, 0x2666);
    Func_0808a0c8(1, 0x188, 0x34b);

    /* Clear bit 0 of actor 5's +0x5a flag, then set it again a beat later. */
    handle = Func_0808a080(5) + 0x5a;
    *handle = (u8)(*handle & 0xfe);

    Func_0808a0d0(5, 0x198, 0x34b);
    Func_0808a010(1);

    handle = Func_0808a080(5) + 0x5a;
    *handle = (u8)(*handle | 1);

    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a0e8(1);
    Func_0808a100(1, 1);
    Func_0808a1b8(1, 0x8000, 30);
    Func_0808a128(21, 4, 30);
    Func_0808a188(21, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(30);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a128(0, 2, 30);
    Func_0808a1f0(0, 0x102);
    Func_0808a010(60);
    Func_0808a1b8(21, 0x3000, 40);
    Func_0808a138(1, 2);
    Func_0808a188(1, 0, 20);
    Func_0808a1e8(21, 0x101, 80);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a1e8(0, 0x102, 80);
    Func_0808a1b8(21, 0x3000, 0);
    Func_0808a1b8(0, 0xd000, 40);
    Func_0808a188(21, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(100);
    Func_0808a150(5, 1, 30);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(10);
    Func_0808a1e8(21, 0x105, 60);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(5, 0x8000, 0);
    Func_0808a1b8(1, 0x8000, 30);
    Func_0808a138(5, 2);
    Func_0808a010(30);
    Func_0808a188(5, 0, 20);
    Func_0808a130(21, 2);
    Func_0808a010(30);
    Func_0808a188(21, 0, 20);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(30);
    Func_0808a110(21, 4);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a110(21, 3);
    Func_0808a010(20);
    Func_0808a188(21, 0, 20);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a138(21, 2);
    Func_0808a010(30);
    Func_0808a188(21, 0, 20);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0x01750000, 0x00a00000, 0x03450000, 1);
    Func_0808a0d0(21, 0x16c, 0x330);
    Func_0808a1b8(0, 0xd000, 0);
    Func_0808a1b8(21, 0x3000, 30);
    Func_0808a188(21, 0, 40);
    Func_0808a150(5, 1, 30);
    Func_0808a130(1, 2);
    Func_0808a138(5, 2);
    Func_0808a010(30);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(5, 0x8000, 30);
    Func_0808a1b8(21, 0x5000, 30);
    Func_0808a188(21, 0, 30);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a110(21, 4);
    Func_0808a010(10);
    Func_0808a178(21, 0);

    /* Third question: same one-answer polarity as the second. */
    if (Func_0808a070(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0808a010(40);
    Func_0808a188(21, 0, 20);
    Func_0808a170(0xf27);
    Func_0808a1e8(21, 0x103, 0);
    Func_0808a138(21, 3);
    Func_0808a010(30);
    Func_0808a188(21, 0, 20);
    Func_0808a128(21, 4, 0);
    Func_0808a138(21, 3);
    Func_0808a100(21, 7);
    Func_0808a010(5);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_0808a1d0(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, 0);

    record = Func_0808a080(21);
    /* The +0x50 handle's +0x26 byte, the field resource_373_c_02003380.c
     * models as Resource373Handle::field26. */
    *(*(u8 **)(record + 0x50) + 0x26) = 0;
    record[0x5a] = (u8)(record[0x5a] & 0xfe);

    Func_0808a090(21, 0x30000, 0x18000);
    Func_0808a0d0(21, 0x16c, 0x32f);
    Func_0808a010(4);

    /* Four frames of a constant slide: +0x18000 on the 16.16 word at +0x10 and
     * -0x1999 on the one at +0x1c. */
    for (step = 0; step != 4; step++) {
        *(s32 *)(record + 16) = *(s32 *)(record + 16) + 0x18000;
        *(s32 *)(record + 28) = *(s32 *)(record + 28) + (s32)0xffffe667;
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
    Func_080091f0(0, 0x40000, 0x10000);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);
    Func_080091e0(Func_0808a080(1), 0);
    Func_0808a1b8(0, 0xd000, 10);
    Func_0808a130(1, 3);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a100(21, 8);

    *(s32 *)(record + 28) = 0x8000;
    Func_0808a0f0(21, 0x016c0000, 0x032b0000);

    /* Five frames of the reverse slide on +0x1c. */
    for (step = 0; step != 5; step++) {
        *(s32 *)(record + 28) = *(s32 *)(record + 28) + 0x1999;
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
    Func_0808a010(30);
    Func_0808a1b8(21, 0x4000, 30);
    Func_0808a138(21, 2);
    Func_0808a010(30);

    record = Func_0808a080(21);
    record[0x23] = (u8)(record[0x23] & 0xfe);

    Func_0808a188(21, 0, 80);
    Func_0808a1e8(21, 0x101, 80);
    Func_0808a1b8(21, 0, 60);
    Func_0808a138(21, 3);
    Func_0808a188(21, 0, 20);
    Func_0808a1f0(21, 0x102);
    Func_0808a010(80);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a188(1, 0, 20);
    Func_0808a1b8(1, 0x8000, 30);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a138(1, 3);
    Func_080091e0(Func_0808a080(1), 1);
    Func_0808a128(1, 6, 0);
    Func_0808a100(1, 1);
    Func_0808a090(1, 0x40000, 0x20000);

    record = Func_0808a080(1);
    record[0x5a] = (u8)(record[0x5a] & 0xfe);

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
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(5, 0xb000, 0);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(21, 0, 60);
    Func_0808a1b8(21, 0x4000, 60);
    Func_0808a110(21, 4);
    Func_0808a010(60);
    Func_0808a1b8(21, 0, 80);
    Func_0808a1e8(21, 0x105, 80);

    /* Split by the interior pool at 0x020026dc. */
    Func_0808a188(21, 0, 60);

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a110(21, 4);
    Func_0808a010(30);
    Func_0808a188(21, 0, 70);
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

    record = Func_0808a080(1);
    record[0x5a] = (u8)(record[0x5a] | 1);

    record = Func_0808a080(5);
    record[0x5a] = (u8)(record[0x5a] | 1);

    record = Func_0808a080(0);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a1b8(0, 0, 0);

    /* The s16 integer views at +0x0a and +0x12 of record 0's 16.16 X and Z. */
    Func_0808a0c8(5, *(s16 *)(record + 10) + 16, *(s16 *)(record + 18));
    Func_0808a0d0(1, *(s16 *)(record + 10) + 16, *(s16 *)(record + 18) - 16);

    Func_0808a0e8(1);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(40);
    Func_0808a0d0(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0f0(5, 0, 0);
    Func_0808a0d0(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_0808a0f0(1, 0, 0);
    Func_0808a050(1, 5);
    Func_0808a210(0x01790000, 0x00a00000, 0x03770000, 1);

    Func_02003380(0, 13, 10, 0);

    Func_0808a0d0(0, 0x178, 0x390);
    Func_0808a1b8(0, 0xc000, 0);

    handle = Func_0808a080(21) + 0x5a;
    *handle = (u8)(*handle | 1);

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
    Func_080770c8(0x202);
    Func_080770d0(0x12f);

    /* Put record 0's +0x55 flag back the way the scene found it. */
    *savedFlagSlot = savedFlag;

    /* Common exit; no argument register is set. */
    Func_0808a020();
}
