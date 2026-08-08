/*
 * resource_3c4 @ 0x0200259c, 2,636 bytes (0x0200259c-0x02002fe7).
 *
 * The overlay's chapter director: one owner that stages every actor, prop and
 * camera cue for the scene named by the pair of signed halfwords at
 * Data_02000240[224] (the chapter) and [225] (the beat).  It is reached once
 * per beat, does its staging, and returns 0.
 *
 * POOL MAP, derived by a control-flow walk from the prologue (branch targets
 * only; both `mov pc,r3` tables were read and their entries seeded).  1,100
 * bytes are reached without them, 2,408 with; the five gaps are:
 *
 *   0x02002602..0x02002604   2   alignment halfword before the table
 *   0x02002604..0x02002638  52   jump table A, 13 entries
 *   0x02002970..0x020029a8  56   literal pool (14 words)
 *   0x02002a48..0x02002a90  72   jump table B, 18 entries
 *   0x02002b8a..0x02002b8c   2   alignment halfword before the pool
 *   0x02002b8c..0x02002bac  32   literal pool (8 words)
 *   0x02002c8c..0x02002c98  12   literal pool (3 words)
 *
 * 228 pool bytes, 2,408 executable.  Every one of those gaps is confirmed
 * twice over: each word is the target of an `ldr rN,[pc,#k]` reached by the
 * walk, and each table's base is the pool word its `mov pc,r3` dispatch loads.
 * The 12-byte gap at 0x02002c8c is the interesting one — it ends *mid-row*,
 * the `b.n` at 0x02002c8a jumping over exactly three words to resume at
 * 0x02002c98 with an argument register (r1 = 0) that was set at 0x02002c88
 * and is consumed by the call at 0x02002c98, so the gap cannot be a body
 * boundary.
 *
 * LINK BASE 0x02008000, four witnesses inside this owner alone: the two jump
 * table bases 0x0200a604 and 0x0200aa48 equal the tables' own file offsets
 * 0x2604/0x2a48; the callback words 0x02008ec9 and 0x02008e21 are
 * Func_02000ec8 and Func_02000e20 plus the Thumb bit; and 0x02009a99 is
 * Func_02001a98 plus the Thumb bit, whose byte-exact source is tracked in
 * assets/code.  The table entries are even, as `mov pc,rN` does not interwork.
 *
 * CALLS: 211 `bl` sites on the walk, of which five (0x020025f6, 0x0200269e,
 * 0x0200270a, 0x02002776, 0x0200278e) resolve to this owner's own epilogue at
 * 0x02002fda and are long unconditional branches, not calls.  206 real calls
 * remain, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (`bl` stores
 * target_offset - 2); the printed annotations in any disassembly of this
 * region are artefacts.
 *
 * Shared tails are spelled with a label and a `goto`, never duplicated per
 * arm: 0x020026e2 is entered from both halves of the first beat with only its
 * four register arguments differing, 0x02002a1e is entered from three beats
 * with only the actor id differing, and 0x02002daa is entered both from the
 * head of its beat and by falling off the end of it.
 *
 * The epilogue is `movs r0,#0 ; add sp,#8 ; pop {r3} ; mov r8,r3 ;
 * pop {r5,r6,r7} ; pop {r1} ; bx r1` — r1, not r0, carries the return address,
 * so r0 survives and the owner returns the constant 0.
 */
#include "types.h"

typedef struct Actor_0200259c Actor_0200259c;

/* Installed at +108 and run per frame while the beat is on screen. */
typedef s32 (*Watch_0200259c)(void *actor);

struct Actor_0200259c {
    s32 pad0;                  /* +0 */
    s32 pad4;                  /* +4 */
    s32 x;                     /* +8,   12.20 fixed point */
    s32 y;                     /* +12,  12.20 fixed point */
    s32 z;                     /* +16,  12.20 fixed point */
    u8 pad20[15];              /* +20 */
    u8 flags;                  /* +35 */
    u8 pad36[49];              /* +36 */
    u8 hidden;                 /* +85 */
    u8 pad86[3];               /* +86 */
    u8 mode;                   /* +89 */
    u8 pad90[10];              /* +90 */
    s16 pose;                  /* +100 */
    u8 pad102[6];              /* +102 */
    Watch_0200259c watch;      /* +108 */
};

/* Chapter/beat script state, shared with the rest of the overlay set. */
extern s16 Data_02000240[];
/* IWRAM pointer cell; the block it names carries the beat's request word. */
extern u8 *Data_03001ebc;

/* Scene-flag query and clear, and the scene-script kick. */
void Func_02005762();
Actor_0200259c *Func_02005710();
void Func_020056e0();
void Func_020056f4();
void Func_02005704();
void Func_02005714();
Actor_0200259c *Func_0200576c();
void Func_02005740();
void Func_02005754();
void Func_02005764();
void Func_02005774();
void Func_02005784();
void Func_02005794();
void Func_02004ab0();
Actor_0200259c *Func_02005822();
Actor_0200259c *Func_0200582e();
Actor_0200259c *Func_02005838();
Actor_0200259c *Func_020057de();
Actor_0200259c *Func_02005844();
Actor_0200259c *Func_020057ea();
Actor_0200259c *Func_02005850();
Actor_0200259c *Func_02005826();
void Func_020057fe();
void Func_02005820();
void Func_02004ba4();
Actor_0200259c *Func_020058a6();
Actor_0200259c *Func_020058b2();
Actor_0200259c *Func_020058bc();
Actor_0200259c *Func_02005862();
Actor_0200259c *Func_020058c8();
Actor_0200259c *Func_0200586e();
Actor_0200259c *Func_020058d4();
Actor_0200259c *Func_020058e2();
Actor_0200259c *Func_02005888();
Actor_0200259c *Func_020058ee();
Actor_0200259c *Func_02005894();
void Func_02005934();
Actor_0200259c *Func_02005902();
Actor_0200259c *Func_020058a8();
Actor_0200259c *Func_0200590e();
Actor_0200259c *Func_020058b4();
Actor_0200259c *Func_0200596c();
Actor_0200259c *Func_02005974();
Actor_0200259c *Func_020058f2();
void Func_020058c2();
Actor_0200259c *Func_020058e6();
Actor_0200259c *Func_0200595c();
void Func_020059a6();
Actor_0200259c *Func_02005910();
Actor_0200259c *Func_020059be();
Actor_0200259c *Func_02005994();
Actor_0200259c *Func_020059d6();
Actor_0200259c *Func_02005976();
Actor_0200259c *Func_02005a0a();
void Func_020059fa();
Actor_0200259c *Func_02005990();
void Func_02005a0c();
Actor_0200259c *Func_020059a2();
void Func_02005972();
Actor_0200259c *Func_02005996();
Actor_0200259c *Func_02005a0c_b();
void Func_02005a56();
Actor_0200259c *Func_020059c0();
Actor_0200259c *Func_02005a6e();
Actor_0200259c *Func_02005a44();
Actor_0200259c *Func_02005a86();
Actor_0200259c *Func_02005a2c();
Actor_0200259c *Func_02005a2a();
Actor_0200259c *Func_02005ac0();
void Func_02005ab0();
void Func_02004e2a();
Actor_0200259c *Func_02005abc();
Actor_0200259c *Func_02005ac8();
Actor_0200259c *Func_02005ad2();
Actor_0200259c *Func_02005a78();
Actor_0200259c *Func_02005ade();
Actor_0200259c *Func_02005a84();
Actor_0200259c *Func_02005aea();
Actor_0200259c *Func_02005a90();
Actor_0200259c *Func_02005af6();
Actor_0200259c *Func_02005a9c();
Actor_0200259c *Func_02005b02();
Actor_0200259c *Func_02005aa8();
Actor_0200259c *Func_02005b0e();
Actor_0200259c *Func_02005ab4();
Actor_0200259c *Func_02005b1a();
Actor_0200259c *Func_02005b24();
Actor_0200259c *Func_02005b2c();
void Func_02005a58();
void Func_02005b8a();
Actor_0200259c *Func_02005c0a();
Actor_0200259c *Func_02005c12();
Actor_0200259c *Func_02005b90();
Actor_0200259c *Func_02005ba8();
Actor_0200259c *Func_02005c22();
void Func_02005016();
Actor_0200259c *Func_02005c60();
Actor_0200259c *Func_02005c06();
Actor_0200259c *Func_02005bb8();
Actor_0200259c *Func_02005c80();
Actor_0200259c *Func_02005c78();
Actor_0200259c *Func_02005c2e();
Actor_0200259c *Func_02005bd4();
Actor_0200259c *Func_02005c3a();
Actor_0200259c *Func_02005c0e();
Actor_0200259c *Func_02005ca4();
void Func_02005c94();
Actor_0200259c *Func_02005c62();
Actor_0200259c *Func_02005c6a();
Actor_0200259c *Func_02005c10();
Actor_0200259c *Func_02005c86();
Actor_0200259c *Func_02005c90();
Actor_0200259c *Func_02005c84();
Actor_0200259c *Func_02005cca();
Actor_0200259c *Func_02005cd4();
Actor_0200259c *Func_02005cde();
Actor_0200259c *Func_02005ce8();
Actor_0200259c *Func_02005cf0();
Actor_0200259c *Func_02005d04();
Actor_0200259c *Func_02005d14();
Actor_0200259c *Func_02005d26();
Actor_0200259c *Func_02005d34();
Actor_0200259c *Func_02005d40();
Actor_0200259c *Func_02005dac();
Actor_0200259c *Func_02005db4();
Actor_0200259c *Func_02005d5a();
Actor_0200259c *Func_02005d66();
Actor_0200259c *Func_02005d70();
Actor_0200259c *Func_02005d7a();
Actor_0200259c *Func_02005d20();
Actor_0200259c *Func_02005d86();
Actor_0200259c *Func_02005d2c();
Actor_0200259c *Func_02005d92();
Actor_0200259c *Func_02005d46();
Actor_0200259c *Func_02005db6();
Actor_0200259c *Func_02005dc0();
Actor_0200259c *Func_02005ddc();
Actor_0200259c *Func_02005de6();
Actor_0200259c *Func_02005df0();
Actor_0200259c *Func_02005dfc();
Actor_0200259c *Func_02005e40();
Actor_0200259c *Func_02005e16();
Actor_0200259c *Func_02005e22();
Actor_0200259c *Func_02005e2c();
Actor_0200259c *Func_02005e74();
Actor_0200259c *Func_02005e4a();
Actor_0200259c *Func_02005e56();
Actor_0200259c *Func_02005e5e();
Actor_0200259c *Func_02005ea6();
Actor_0200259c *Func_02005e7c();
Actor_0200259c *Func_02005e88();
Actor_0200259c *Func_02005e90();
Actor_0200259c *Func_02005e80();
Actor_0200259c *Func_02005e78();
Actor_0200259c *Func_02005e7e();
Actor_0200259c *Func_02005e84();
Actor_0200259c *Func_0200486a();
Actor_0200259c *Func_02005e8a();
void Func_02005e62();
Actor_0200259c *Func_02005ea8();
void Func_02005e80_b();
Actor_0200259c *Func_02005ece();
Actor_0200259c *Func_02005ea4();
void Func_02004de8();
void Func_02005e42();
Actor_0200259c *Func_02005f0a();
Actor_0200259c *Func_02005ef8();
void Func_02005ed0();
Actor_0200259c *Func_02005f86();
Actor_0200259c *Func_02005f94();
Actor_0200259c *Func_02005fa2();
Actor_0200259c *Func_02005fb0();
Actor_0200259c *Func_02005fbe();
Actor_0200259c *Func_02005fa6();
Actor_0200259c *Func_02005fb6();
Actor_0200259c *Func_02005fc6();
Actor_0200259c *Func_02005fd6();
Actor_0200259c *Func_02006036();
Actor_0200259c *Func_0200603e();
Actor_0200259c *Func_02006046();
Actor_0200259c *Func_0200604e();
Actor_0200259c *Func_02006056();
void Func_02006046_b();
void Func_0200604e_b();
Actor_0200259c *Func_0200601c();
Actor_0200259c *Func_02006024();
Actor_0200259c *Func_02005fca();
Actor_0200259c *Func_02006040();
Actor_0200259c *Func_02006048();
Actor_0200259c *Func_02005fee();
Actor_0200259c *Func_02006064();
Actor_0200259c *Func_0200606c();
Actor_0200259c *Func_02006012();
Actor_0200259c *Func_02006088();
Actor_0200259c *Func_02006090();
Actor_0200259c *Func_020060ac();
Actor_0200259c *Func_020060b4();
Actor_0200259c *Func_0200605a();
Actor_0200259c *Func_02006070();
void Func_02004fb4();

                     

                     

                     

                                

                                

                     

                     

/* Placement services: four register arguments plus two stack words. */
/* Resets the actor record the first argument points at. */

/* Task installer; the first argument is a Thumb entry, not data. */

/* Actor services, all keyed by slot id. */

/* Beat helpers inside this overlay. */

/* Per-frame watches installed through +108 and through Func_080000d0. */
s32 Func_02000ec8(void *actor);
s32 Func_02001a98(void *actor);
void Func_02000e20(void);

s32 Func_0200259c(void)
{
    Actor_0200259c *actor;
    s32 beat;
    s32 a, b, c, d;
    s32 slot;
    s32 x, z;

    *(s32 *)(Data_03001ebc + 448) = 516;

    if (Data_02000240[224] == 0xac || Data_02000240[224] == 0xad) {
        Func_02005762(0);
        Data_02000240[289] = 1;
        Data_02000240[288] = 0xac;
    }

    if (Data_02000240[224] != 0xac) goto other_chapter;

    beat = Data_02000240[225];
    if ((unsigned int)(beat - 1) > 12) goto done;

    switch (beat) {
    case 1:
    case 2:                                     /* 0x02002638 */
        if (Func_02005710(0x982) != 0) {
            Func_020056e0(121, 4, 74, 9, 5, 8);
            Func_020056f4(18, 83, 9, 73, 3, 2);
            Func_02005704(18, 81, 9, 75, 3, 2);
            Func_02005714(18, 83, 9, 77, 3, 2);
            a = 18; b = 83; c = 9; d = 79;
            goto place_trio;
        }
        if (Func_0200576c(0x983) == 0) goto done;
        Func_02005740(121, 13, 74, 9, 5, 8);
        Func_02005754(18, 85, 11, 74, 3, 2);
        Func_02005764(18, 83, 13, 75, 3, 2);
        a = 18; b = 85; c = 11; d = 76;
    place_trio:                                 /* 0x020026e2 */
        Func_02005774(a, b, c, d, 3, 2);
        Func_02005784(18, 83, 11, 78, 3, 2);
        Func_02005794(18, 83, 13, 79, 3, 2);
        goto done;

    case 3:
    case 4:                                     /* 0x0200270e */
        Func_02004ab0();
        Func_02005822(8)->hidden = 0;
        Func_0200582e(9)->hidden = 0;
        Func_020057de(Func_02005838(8), 0);
        Func_020057ea(Func_02005844(9), 0);
        Func_02005850(8)->watch = Func_02000ec8;
        slot = 9;
        goto install_watch;

    case 5:
    case 6:
    case 7:                                     /* 0x0200274e */
        if (Func_02005826(0x982) != 0) Func_020057fe(23, 17, 1, 2, 30, 8);
        if (Func_02005844(0x983) == 0) goto done;
        Func_02005820(23, 17, 1, 2, 32, 10);
        goto done;

    case 8:
    case 9:                                     /* 0x02002792 */
        Func_02004ba4();
        Func_020058a6(10)->hidden = 0;
        Func_020058b2(11)->hidden = 0;
        Func_02005862(Func_020058bc(10), 0);
        Func_0200586e(Func_020058c8(11), 0);
        Func_020058d4(10)->watch = Func_02000ec8;
        slot = 11;
        goto install_watch;

    case 10:
    case 11:                                    /* 0x020027d2 */
        Func_02005888(Func_020058e2(18), 0);
        Func_02005894(Func_020058ee(19), 0);
        Func_02005934(18, 2);
        Func_020058a8(Func_02005902(20), 0);
        Func_020058b4(Func_0200590e(21), 0);
        Func_0200596c(20, 15);
        Func_02005974(21, 15);

        if (Func_020058f2(0x971) != 0) {
            Func_020058c2(59, 8, 49, 8, 1, 3);
            Func_020058e6(51, 8, 1, 1, 49, 8);
            actor = Func_0200595c(18);
            actor->flags |= 2;
            Func_020059a6(18, 3);
            Func_02005910(45, 4, 1, 1, 46, 8);
            Func_020059be(18, 0x02e80000, 0x00880000);
            Func_02005994(18)->y = 0xfff00000;
            Func_020059d6(20, 0x02e80000, 0x00880000);
        }
        if (Func_02005976(512) != 0) {
            Func_02005a0a(20, 0);
            Func_020059fa(20, 5);
        }
        if (Func_02005990(0x202) != 0) Func_02005a0c(19, 2);
        if (Func_020059a2(0x972) != 0) {
            Func_02005972(59, 8, 45, 14, 1, 3);
            Func_02005996(51, 8, 1, 1, 45, 14);
            actor = Func_02005a0c_b(19);
            actor->flags |= 2;
            Func_02005a56(19, 3);
            Func_020059c0(45, 4, 1, 1, 48, 14);
            Func_02005a6e(19, 0x03080000, 0x00e80000);
            Func_02005a44(19)->y = 0xfff00000;
            Func_02005a86(21, 0x03080000, 0x00e80000);
            Func_02005a2c(0x202);
        }
        if (Func_02005a2a(0x201) == 0) goto done;
        Func_02005ac0(21, 0);
        Func_02005ab0(21, 5);
        goto done;

    case 12:
    case 13:                                    /* 0x020029a8 */
        Func_02004e2a();
        Func_02005abc(12)->hidden = 0;
        Func_02005ac8(13)->hidden = 0;
        Func_02005a78(Func_02005ad2(15), 0);
        Func_02005a84(Func_02005ade(16), 0);
        Func_02005a90(Func_02005aea(17), 0);
        Func_02005a9c(Func_02005af6(12), 0);
        Func_02005aa8(Func_02005b02(13), 0);
        Func_02005ab4(Func_02005b0e(14), 0);
        Func_02005b1a(12)->watch = Func_02000ec8;
        Func_02005b24(13)->watch = Func_02000ec8;
        slot = 14;
        goto install_watch;
    }

install_watch:                                  /* 0x02002a1e */
    Func_02005b2c(slot)->watch = Func_02000ec8;
    Func_02005a58(Func_02000e20, 3200);
    goto done;

other_chapter:                                  /* 0x02002a30 */
    beat = Data_02000240[225];
    if ((unsigned int)beat > 17) goto done;

    switch (beat) {
    case 1:
    case 2:
    case 3:                                     /* 0x02002a90 */
        Data_02000240[289] = 1;
        Data_02000240[288] = 176;
        Func_02005b8a(303);
        Func_02005c0a(17, 6);
        Func_02005c12(18, 6);
        if (Func_02005b90(0x974) != 0) Func_02005c0a(17, 0x02d80000, 0x01380000);
        if (Func_02005ba8(0x975) != 0) Func_02005c22(18, 0x02e80000, 0x01380000);
        Func_02005016();
        goto done;

    case 4:
    case 5:                                     /* 0x02002bac */
        if (Func_02005c60(0x109) != 0) goto restart_chapter;
        Func_02005c06(10)->hidden = 0;
        Func_02005c12(11)->hidden = 0;
        Func_02005bb8(10)->y = 0xffd00000;
        Func_02005c80(11)->y = 0xffd00000;
        Func_02005c78(10)->flags |= 2;
        Func_02005c2e(11)->flags |= 2;
        Func_02005bd4(10)->mode &= 0xfe;
        Func_02005c3a(11)->mode &= 0xfe;
        Func_02005c0e(10)->pose = 3;
        Func_02005ca4(11)->pose = 3;
        Func_02005c94(10, 1);
        Func_02005c62(11, 1);
        Func_02005c6a(12)->hidden = 0;
        Func_02005c10(13)->hidden = 0;
        Func_02005c86(14)->hidden = 0;
        Func_02005c84(Func_02005c90(12), 0);
        Func_02005cd4(Func_02005cca(13), 0);
        Func_02005ce8(Func_02005cde(14), 0);
        Func_02005cf0(12)->pose = 0;
        Func_02005d04(13)->pose = 0;
        actor = Func_02005d14(14);
        beat = Data_02000240[225];
        actor->pose = 0;
        if (beat != 5) goto done;

        Func_02005d26(10)->y = 0xffe00000;
        Func_02005d34(11)->y = 0xffc00000;
        Func_02005d40(10)->pose = 2;
        Func_02005dac(11)->pose = 4;
        Func_02005db4(12, 0x00c80000, 0x00980000);
        Func_02005d5a(12)->pose = 11;
        Func_02005d66(12)->watch = Func_02001a98;
        Func_02005d70(12)->flags |= 2;
        Func_02005d7a(13, 0x00c80000, 0x00980000);
        Func_02005d20(13)->pose = 12;
        Func_02005d86(13)->watch = Func_02001a98;
        Func_02005d2c(13)->flags |= 2;
        Func_02005d92(14, 0x00880000, 0x00980000);
        Func_02005d46(14)->pose = 10;
        Func_02005dac(14)->watch = Func_02001a98;
        Func_02005db6(14)->flags |= 2;
        Func_02005dc0(2);
        Func_02005ddc(512);
        Func_02005de6(0x201);
        Func_02005df0(0x202);
    restart_chapter:                            /* 0x02002daa */
        Func_02005dfc(0);
        goto done;

    case 6:
    case 7:                                     /* 0x02002aee */
        Func_02005e40(8, 1);
        Func_02005e16(8)->hidden = 0;
        Func_02005e2c(Func_02005e22(8), 0);
        Func_02005e74(9, 1);
        Func_02005e4a(9, 15);
        Func_02005e5e(Func_02005e56(9), 0);
        Func_02005ea6(9)->hidden = 0;
        if (Func_02005e7c(516) == 0) goto done;
        Func_02005e88(9, 0);
        Func_02005e90(9, 5);
        x = Func_02005e80(9)->x;
        z = Func_02005e78(9)->z >> 20;
        Func_02005e7e(26, 8, 1, 1, x >> 20, z);
        Func_02005e84(9)->watch = Func_02000ec8;
        Func_0200486a(8)->watch = Func_02000ec8;
        goto done;

    case 8:
    case 9:
    case 10:
    case 11:                                    /* 0x02002db2 */
        if (Func_02005e8a(0x982) != 0) Func_02005e62(10, 30, 1, 2, 16, 30);
        if (Func_02005ea8(0x983) != 0) Func_02005e80_b(10, 30, 1, 2, 22, 30);
        Func_02005ece(0x973);
        goto done;

    case 12:                                    /* 0x02002df6 */
        Func_02005ea4(8, 49, 1, 1, 8, 113);
        Func_02004de8();
        Func_02005e42(Func_02000e20, 3200);
        goto done;

    case 13:
    case 14:                                    /* 0x02002e1a */
        Func_02005f0a(1);
        if (Func_02005ef8(0x984) != 0) {
            Func_02005ed0(24, 59, 1, 2, 32, 46);
            Func_02005f86(19, 0x01980000, 0x03180000);
            Func_02005f94(20, 0x01780000, 0x03180000);
            Func_02005fa2(21, 0x01980000, 0x02f80000);
            Func_02005fb0(22, 0x01780000, 0x02f80000);
            Func_02005fbe(23, 0x01880000, 0x03080000);
        }
        Func_02005f94(19)->hidden &= 0xfe;
        Func_02005fa6(20)->hidden &= 0xfe;
        Func_02005fb6(21)->hidden &= 0xfe;
        Func_02005fc6(22)->hidden &= 0xfe;
        Func_02005fd6(23)->hidden &= 0xfe;
        Func_02006036(19, 4);
        Func_0200603e(20, 1);
        Func_02006046(21, 4);
        Func_0200604e(22, 10);
        Func_02006056(23, 0);
        Func_02006046_b(19, 2);
        Func_0200604e_b(23, 2);
        /* Every pin is issued against slot 20, whichever slot was measured. */
        x = Func_0200601c(19)->x;
        z = Func_02006024(19)->z >> 20;
        Func_02005fca(20, 56, 1, 1, x >> 20, z);
        x = Func_02006040(20)->x;
        z = Func_02006048(20)->z >> 20;
        Func_02005fee(20, 56, 1, 1, x >> 20, z);
        x = Func_02006064(21)->x;
        z = Func_0200606c(21)->z >> 20;
        Func_02006012(20, 56, 1, 1, x >> 20, z);
        x = Func_02006088(22)->x;
        z = Func_02006090(22)->z >> 20;
        Func_02006036(20, 56, 1, 1, x >> 20, z);
        x = Func_020060ac(23)->x;
        z = Func_020060b4(23)->z >> 20;
        Func_0200605a(20, 56, 1, 1, x >> 20, z);
        goto done;

    case 17:                                    /* 0x02002fc2 */
        Func_02006070(49, 43, 1, 1, 49, 107);
        Func_02004fb4();
        break;

    default:
        /* Beats 0, 15 and 16 stage nothing. */
        break;
    }

done:                                           /* 0x02002fda */
    return 0;
}
