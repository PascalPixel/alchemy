#include "types.h"

/*
 * resource_383 owner at 0x02001348, 1608 bytes (0x02001348-0x0200198f):
 * code 0x02001348-0x02001748, a six-word literal pool at 0x0200174c that the
 * body branches over, code 0x02001764-0x02001972, and a five-word literal
 * pool at 0x02001974.
 *
 * The overlay's largest cutscene.  Participants 8, 10, 11 and 12 join the
 * three background layers for a staged arrival: the camera is set up, the
 * first dialogue pair 0x12c5 / 0x12c6 plays, the party walks in, and after a
 * Func_0808a070 prompt the scene either plays the short reaction (no line) or
 * the long one (lines 0x12dc and 0x12d9).  Both paths rejoin for the closing
 * walk, then the story flag 0x854 is posted, the workspace word at +448 is
 * set to 0x200, the RAM byte at 0x0200046b is set to 3 and the scene hands
 * over.
 *
 * Apart from that one prompt the owner is straight-line; the `b` at
 * 0x02001748 merely steps over the interior literal pool.
 *
 * All 169 `bl` sites are placed and reach 30 distinct callees - the inventory
 * row's `calls=163` predates the corrected decoding.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  127 reach the import veneer table at 0x02004cxx-0x02004f2b
 * and are named by the main-image address in the veneer's trailing word; 42
 * reach this overlay's own Func_020045f4, Func_0200460c, Func_02004624 and
 * Func_0200463c, all byte-exact in assets/code.
 *
 * The dialogue ids are carried in r5 as a small cursor: 0x12c5 then +1 for
 * the reply, and later 0x12dc then -3.  That is reproduced literally below.
 *
 * Epilogue is `pop {r5} / pop {r0} / bx r0`, so r0 holds the popped return
 * address and the owner is void.
 *
 * Uncertainty: 0x02000240 + 0x22b = 0x0200046b is written as an absolute
 * EWRAM byte.  It is below the overlay's 0x02008000 link base, so unlike the
 * 0x0200dxxx / 0x0200exxx words elsewhere in this overlay it is a RAM global
 * and not in-image data.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_08015040();
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a250();
void Func_0808a260();
void Func_0808a268();
void Func_080f9010();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_0200460c(s32, s32, s32);
void Func_02004624(s32, s32, s32);
void Func_0200463c(s32, s32, s32);

/* In-image outfit descriptors at file offsets 0x5248 and 0x52ac. */
extern u8 Data_0200d248[];
extern u8 Data_0200d2ac[];

void Func_02001348(void)
{
    u8 *workspace;
    s32 line;

    Func_0808a018();
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a138(0, 3);
    Func_0808a010(20);

    Func_08015040(line, 1);
    Func_0808a170(line);                /* 0x12c6 */
    line = line + 1;
    line = 0x12c5;
    Func_0808a010(30);

    Func_0808a100(8, 1);
    Func_0808a010(60);
    Func_0200463c(8, 3, 40);
    Func_0808a0d0(0, 792, 440);
    Func_0200460c(0, 8, 20);
    Func_0808a0f0(1, 0x3180000, 0x1b80000);
    Func_0808a0f0(2, 0x3180000, 0x1b80000);
    Func_0808a0c8(1, 808, 432);
    Func_0808a0c8(2, 792, 456);
    Func_0808a0e8(1);
    Func_0808a148(1, 8, 0);
    Func_0808a0e8(2);
    Func_0200460c(2, 8, 60);
    Func_0808a1e8(8, 0x102, 0);
    Func_0808a010(60);
    Func_020045f4(8, 20);
    Func_0808a100(2, 3);
    Func_0200463c(1, 3, 30);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_020045f4(8, 20);
    Func_0808a1e8(2, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a010(60);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a010(10);
    Func_020045f4(8, 30);
    Func_0808a1b8(8, 0xd000, 0);
    Func_0808a010(30);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a010(60);
    Func_0808a1b8(8, 0xb000, 0);
    Func_0808a010(40);
    Func_0808a1b8(8, 0xd000, 0);
    Func_0808a010(40);
    Func_0200460c(8, 0, 20);
    Func_0200463c(8, 4, 30);
    Func_020045f4(8, 20);
    Func_0808a138(2, 1);
    Func_020045f4(2, 40);

    Func_0808a0f0(10, 0x2e80000, 0x1980000);
    Func_080f9010(61);
    Func_020045f4(10, 20);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a130(2, 1);
    Func_0808a138(8, 1);
    Func_0808a010(30);
    Func_0808a148(0, 10, 0);
    Func_0808a148(1, 10, 0);
    Func_0808a148(2, 10, 0);
    Func_0200460c(8, 10, 40);

    Func_0808a090(10, 0xcccc, 0x6666);
    Func_0808a090(11, 0x18000, 0xc000);
    Func_0808a090(12, 0x18000, 0xc000);
    Func_0808a0f0(11, 0x2e80000, 0x1980000);
    Func_0808a0f0(12, 0x2e80000, 0x1980000);
    Func_0808a0d0(10, 792, 416);
    Func_0808a1b8(10, 0x3000, 0);
    Func_0808a010(70);
    Func_0808a148(0, 10, 0);
    Func_0808a148(1, 10, 0);
    Func_0808a148(2, 10, 0);
    Func_0808a148(8, 10, 0);
    Func_0808a098(11, Data_0200d248);
    Func_0808a010(40);
    Func_0808a098(12, Data_0200d2ac);
    Func_0808a0a0(12);
    Func_0808a1b8(11, 0x2000, 0);
    Func_0808a1b8(12, 0x2000, 0);
    Func_0808a010(40);
    Func_0200463c(10, 4, 20);
    Func_020045f4(10, 20);
    Func_0808a138(11, 1);
    Func_0808a010(10);
    Func_020045f4(11, 20);
    Func_0808a138(12, 1);
    Func_0808a010(10);
    Func_020045f4(12, 40);
    Func_0808a1e8(2, 0x101, 0);
    Func_0808a010(60);
    Func_0200463c(10, 4, 20);
    Func_020045f4(10, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(60);
    Func_0808a1e8(8, 0x100, 0);
    Func_0808a010(40);
    Func_0808a138(8, 2);
    Func_0808a010(20);
    Func_020045f4(8, 20);
    Func_0808a148(0, 8, 0);
    Func_0808a148(1, 8, 0);
    Func_0200460c(2, 8, 20);
    Func_0200463c(8, 4, 30);
    Func_020045f4(8, 20);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 30);
    Func_020045f4(1, 20);
    Func_0808a1e8(8, 0x102, 0);
    Func_0808a010(60);
    Func_020045f4(8, 20);

    Func_0808a138(10, 1);
    Func_0808a010(10);
    Func_020045f4(10, 20);
    Func_0808a148(0, 10, 0);
    Func_0808a148(1, 10, 0);
    Func_0200460c(2, 10, 20);
    Func_0200463c(11, 3, 20);
    Func_020045f4(11, 20);
    Func_0200463c(12, 4, 20);
    Func_020045f4(12, 20);
    Func_0808a1e8(0, 0x102, 0);
    Func_0808a1e8(1, 0x102, 0);
    Func_0808a1e8(2, 0x102, 0);
    Func_0808a010(60);
    Func_0200463c(10, 3, 20);
    Func_0808a178(10, 0);
    Func_0808a010(50);
    Func_0808a148(2, 0, 0);
    Func_0200460c(1, 0, 30);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(40);
        Func_0808a148(1, 10, 0);
        Func_0808a148(2, 10, 0);
        Func_0808a138(10, 2);
        Func_0808a010(20);
        Func_0808a180(10, 0);
    } else {
        Func_0808a010(40);
        Func_0808a148(1, 10, 0);
        Func_0808a148(2, 10, 0);
        line = 0x12dc;
        Func_0808a170(line);
        Func_0808a138(10, 2);
        Func_0808a010(20);
        Func_0808a180(10, 0);
        line = line - 3;
        Func_0808a170(line);            /* 0x12d9 */
    }

    Func_0808a1e8(11, 0x103, 0);
    Func_0808a010(60);
    Func_020045f4(11, 20);
    Func_0200463c(12, 4, 20);
    Func_020045f4(12, 30);
    Func_02004624(10, 11, 30);
    Func_0808a100(10, 3);
    Func_0200463c(11, 3, 30);
    Func_02004624(10, 12, 30);
    Func_0808a100(10, 3);
    Func_0200463c(12, 3, 40);
    Func_0808a1b8(10, 0x3000, 0);
    Func_0808a1b8(11, 0x3000, 0);
    Func_0808a1b8(12, 0x3000, 0);
    Func_0808a010(20);
    Func_0200463c(10, 4, 20);
    Func_0808a180(10, 0);

    Func_080770c8(0x854);

    /* 224 << 1, then + 64: the workspace word at +448 becomes 0x200. */
    *(s32 *)(workspace + 448) = 512;
    workspace = *(u8 **)0x03001ebc;

    Func_0808a260(0x15, 17);
    Func_0808a268(0x15, 16);

    *(u8 *)(0x02000240 + 0x22b) = 3;

    Func_0808a250(12, 5);
    Func_0808a020();
}
