#include "types.h"

/*
 * resource_383 owner at 0x020021c8, 924 bytes (0x020021c8-0x02002563):
 * code 0x020021c8-0x02002545 and a seven-word literal pool at 0x02002548.
 *
 * The conversation that Func_02001f50 hands off to.  It opens the camera and
 * the three background layers, clears bits 2-3 of the +9 flag byte inside
 * each participant's +80 sub-record (leaving bit 2 set) and sets bit 0 of the
 * byte at +35 of participant 0's record, publishes 24 and 0x201 into the
 * workspace at *(void **)0x03001ebc, then plays the dialogue.
 *
 * The dialogue has an optional branch: three nested Func_0808a070 prompts
 * (each "did the player answer / is the choice still live?") gate a longer
 * exchange, and only the innermost success sets the local flag.  When the
 * flag is still clear the short line 0x12bc is played instead.  Both paths
 * then rejoin for the closing camera moves.
 *
 * All 91 `bl` sites are placed and reach 27 distinct callees - the inventory
 * row's `calls=87` predates the corrected decoding.  Targets come from
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Sixty-three reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; twenty-eight reach this overlay's own Func_020045f4, Func_0200460c,
 * Func_02004624 and Func_0200463c, all byte-exact in assets/code.
 *
 * Epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.
 *
 * Uncertainty: Func_0808a218 and Func_08009128 are reached with no argument
 * register set up by this owner, so they are treated as taking none.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_020070d0();
void Func_02007008();
u8 *Func_02006f56();
void Func_0200705c();
void Func_02006f86();
void Func_02006f90();
void Func_02006f9a();
void Func_02006ff0();
void Func_02006ffe();
void Func_0200700c();
u8 *Func_02006fc2();
u8 *Func_02006fda();
u8 *Func_02006fec();
void Func_02007090();
void Func_0200689a(s32, s32, s32);
void Func_02007068();
void Func_02007076();
void Func_020070c0();
void Func_02007108();
void Func_0200711c();
void Func_02006f88();
void Func_02007006();
void Func_02007182();
void Func_02007196();
void Func_0200702c();
void Func_020070f4();
void Func_0200703a();
void Func_02007118();
void Func_02006900(s32, s32);
void Func_02006952(s32, s32, s32);
void Func_02006912(s32, s32);
void Func_0200694c(s32, s32, s32);
void Func_02007136();
void Func_02006948(s32, s32, s32);
void Func_02006982(s32, s32, s32);
void Func_02007172();
void Func_020071a2();
void Func_020070a8();
void Func_020069b2(s32, s32, s32);
void Func_02006972(s32, s32);
void Func_020070c8();
void Func_020069ba(s32, s32, s32);
void Func_020071ba();
s32 Func_0200711c_b();
void Func_020070ee();
void Func_020071ae();
void Func_02006a00(s32, s32, s32);
void Func_020071e8();
s32 Func_02007148();
void Func_0200711a();
void Func_02007226();
void Func_0200712c();
void Func_020071f4();
void Func_02006a26(s32, s32, s32);
s32 Func_02007186();
void Func_02007158();
void Func_02007262();
void Func_02007168();
void Func_02006a28(s32, s32);
void Func_02006a4a(s32, s32, s32);
void Func_02007242();
void Func_02007188();
void Func_02006a48(s32, s32);
void Func_02006a6a(s32, s32, s32);
void Func_020072a4();
void Func_020071aa();
void Func_02006ab4(s32, s32, s32);
void Func_02006a74(s32, s32);
void Func_02006aae(s32, s32, s32);
void Func_02006ad0(s32, s32, s32);
void Func_02006a92(s32, s32);
void Func_020072bc();
void Func_02006aa4(s32, s32);
void Func_02006ade(s32, s32, s32);
void Func_02006ab6(s32, s32);
void Func_02007308();
void Func_0200720e();
void Func_020072d6();
void Func_02006ad6(s32, s32);
void Func_02006b28(s32, s32, s32);
void Func_02007302();
void Func_02006b24(s32, s32, s32);
void Func_0200732c();
void Func_02006b4e(s32, s32, s32);
void Func_020072ca();
void Func_020072de();
void Func_02007300();
void Func_0200730a();

                                /* dialogue prompt result */
                                /* returns the participant record */

                     

                     

                     

                     

                     

                     

/* This overlay's own routines; byte-exact sources in assets/code. */

                                  

void Func_020021c8(void)
{
    u8 *workspace;
    u8 *record;
    u8 *flags;
    s32 answered;

    Func_020070d0();
    Func_02007008(0, 1);

    record = Func_02006f56(0);
    record[35] = (u8)(record[35] | 1);

    /* 128 << 14, 144 << 18, 200 << 17, 234 << 18. */
    Func_0200705c(0x200000, 0x2400000, 0x1900000, 0x3a80000);

    Func_02006f86(0, 0xcccc, 0x6666);
    Func_02006f90(1, 0xcccc, 0x6666);
    Func_02006f9a(2, 0xcccc, 0x6666);

    Func_02006ff0(0, 0xf80000, 0x2d80000);      /* 248 << 16, 182 << 18 */
    Func_02006ffe(2, 0x1080000, 0x2e80000);     /* 132 << 17, 186 << 18 */
    Func_0200700c(1, 0xe80000, 0x2e80000);      /* 232 << 16 */

    record = Func_02006fc2(0);
    record = Func_02006fda(1);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    record = Func_02006fec(2);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);

    Func_02007090(0, 2, 0);
    Func_0200689a(1, 2, 30);
    Func_02007068(24, 0x680000, 0x2b80000);     /* 208 << 15, 174 << 18 */
    Func_02007076(25, 0x780000, 0x2b80000);     /* 240 << 15 */
    Func_020070c0(24, 25, 0);
    Func_02007108(0, 0);
    Func_0200711c();
    Func_02006f88();
    Func_02007006(30);

    /* 228 << 1 and 224 << 1. */
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 0x201;
    *(s32 *)(workspace + 456) = 24;

    Func_02007182();
    Func_02007196();
    Func_0200702c(40);

    Func_020070f4(1, 1);
    Func_0200703a(10);
    Func_02007118(0x12ae);
    Func_02006900(1, 20);
    Func_02006952(2, 3, 20);
    Func_02006912(2, 20);
    Func_0200694c(0, 1, 50);
    Func_02007136(0, 2, 0);
    Func_02006948(1, 2, 20);
    Func_02006982(2, 4, 20);
    Func_02007172(2, 0);
    Func_02007196(0, 258, 0);           /* 129 << 1 */
    Func_020071a2(1, 258, 0);
    Func_020070a8(60);
    Func_020069b2(2, 3, 20);
    Func_02006972(2, 30);
    Func_02007182(1, 1);
    Func_020070c8(20);
    Func_020069ba(0, 1, 20);
    Func_020071ba(1, 0);

    answered = 0;
    if (Func_0200711c_b(0, 0) != 0) {
        Func_020070ee(20);
        Func_020071ae(2, 2);
        Func_02006a00(1, 4, 30);
        Func_020071e8(1, 0);

        if (Func_02007148(0, 0) != 0) {
            Func_0200711a(20);
            Func_02007226(2, 258, 0);
            Func_0200712c(60);
            Func_020071f4(2, 2);
            Func_02006a26(0, 2, 20);
            Func_02007226(2, 0);

            if (Func_02007186(2, 0) != 0) {
                Func_02007158(20);
                Func_02007262(2, 0x105, 0);
                Func_02007168(60);
                Func_02006a28(2, 20);
                Func_02006a4a(1, 2, 10);
                Func_02007242(1, 1);
                Func_02007188(10);
                Func_02006a48(1, 10);
                Func_02006a6a(2, 1, 20);
                Func_020072a4(2, 0x101, 0);
                Func_020071aa(60);
                Func_02006ab4(2, 4, 20);
                Func_02006a74(1, 10);
                Func_02006aae(1, 0, 20);
                Func_02006ad0(1, 3, 20);
                answered = 1;
                Func_02006a92(1, 20);
            }
        }
    }

    if (answered == 0) {
        Func_020072bc(0x12bc);
        Func_02006aa4(1, 20);
        Func_02006ade(1, 0, 20);
        Func_02006ab6(1, 20);
    }

    Func_02007308(0, 0x105, 0);
    Func_0200720e(60);
    Func_020072d6(1, 1);
    Func_02006ad6(1, 10);
    Func_02006b28(1, 3, 10);
    Func_02007302(1, 2, 0);
    Func_02006b24(0, 2, 10);
    Func_0200732c(1, 0);
    Func_02006b4e(2, 3, 10);

    Func_020072ca(2, 248, 728);         /* 182 << 2 */
    Func_020072de(1, 248, 728);
    Func_02007300(1, 0, 0);
    Func_0200730a(2, 0, 0);
}
