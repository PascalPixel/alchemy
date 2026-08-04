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
 * tools/overlay_call_targets.ts (target offset = stored displacement + 2),
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
void Func_08009128();
void Func_0808a010();
s32 Func_0808a070();            /* dialogue prompt result */
u8 *Func_0808a080();            /* returns the participant record */
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a200();
void Func_0808a218();
void Func_0808a230();
void Func_0808a360();
void Func_0808a370();
void Func_0808a4f0();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_0200460c(s32, s32, s32);
void Func_02004624(s32, s32, s32);
void Func_0200463c(s32, s32, s32);

void Func_020021c8(void)
{
    u8 *workspace;
    u8 *record;
    u8 *flags;
    s32 answered;

    Func_0808a4f0();
    Func_0808a1e0(0, 1);

    record = Func_0808a080(0);
    record[35] = (u8)(record[35] | 1);

    /* 128 << 14, 144 << 18, 200 << 17, 234 << 18. */
    Func_0808a230(0x200000, 0x2400000, 0x1900000, 0x3a80000);

    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);

    Func_0808a0f0(0, 0xf80000, 0x2d80000);      /* 248 << 16, 182 << 18 */
    Func_0808a0f0(2, 0x1080000, 0x2e80000);     /* 132 << 17, 186 << 18 */
    Func_0808a0f0(1, 0xe80000, 0x2e80000);      /* 232 << 16 */

    record = Func_0808a080(0);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    record = Func_0808a080(1);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);
    record = Func_0808a080(2);
    flags = *(u8 **)(record + 80);
    flags[9] = (u8)((flags[9] & ~12) | 4);

    Func_0808a150(0, 2, 0);
    Func_0200460c(1, 2, 30);
    Func_0808a0f0(24, 0x680000, 0x2b80000);     /* 208 << 15, 174 << 18 */
    Func_0808a0f0(25, 0x780000, 0x2b80000);     /* 240 << 15 */
    Func_0808a150(24, 25, 0);
    Func_0808a200(0, 0);
    Func_0808a218();
    Func_08009128();
    Func_0808a010(30);

    /* 228 << 1 and 224 << 1. */
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 456) = 24;
    *(s32 *)(workspace + 448) = 0x201;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_0808a138(1, 1);
    Func_0808a010(10);
    Func_0808a170(0x12ae);
    Func_020045f4(1, 20);
    Func_0200463c(2, 3, 20);
    Func_020045f4(2, 20);
    Func_02004624(0, 1, 50);
    Func_0808a148(0, 2, 0);
    Func_0200460c(1, 2, 20);
    Func_0200463c(2, 4, 20);
    Func_0808a180(2, 0);
    Func_0808a1e8(0, 258, 0);           /* 129 << 1 */
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(60);
    Func_0200463c(2, 3, 20);
    Func_020045f4(2, 30);
    Func_0808a138(1, 1);
    Func_0808a010(20);
    Func_02004624(0, 1, 20);
    Func_0808a178(1, 0);

    answered = 0;
    if (Func_0808a070(0, 0) != 0) {
        Func_0808a010(20);
        Func_0808a130(2, 2);
        Func_0200463c(1, 4, 30);
        Func_0808a178(1, 0);

        if (Func_0808a070(0, 0) != 0) {
            Func_0808a010(20);
            Func_0808a1e8(2, 258, 0);
            Func_0808a010(60);
            Func_0808a138(2, 2);
            Func_02004624(0, 2, 20);
            Func_0808a178(2, 0);

            if (Func_0808a070(2, 0) != 0) {
                Func_0808a010(20);
                Func_0808a1e8(2, 0x105, 0);
                Func_0808a010(60);
                Func_020045f4(2, 20);
                Func_0200460c(1, 2, 10);
                Func_0808a138(1, 1);
                Func_0808a010(10);
                Func_020045f4(1, 10);
                Func_0200460c(2, 1, 20);
                Func_0808a1e8(2, 0x101, 0);
                Func_0808a010(60);
                Func_0200463c(2, 4, 20);
                Func_020045f4(1, 10);
                Func_02004624(1, 0, 20);
                Func_0200463c(1, 3, 20);
                answered = 1;
                Func_020045f4(1, 20);
            }
        }
    }

    if (answered == 0) {
        Func_0808a170(0x12bc);
        Func_020045f4(1, 20);
        Func_02004624(1, 0, 20);
        Func_020045f4(1, 20);
    }

    Func_0808a1e8(0, 0x105, 0);
    Func_0808a010(60);
    Func_0808a138(1, 1);
    Func_020045f4(1, 10);
    Func_0200463c(1, 3, 10);
    Func_0808a150(1, 2, 0);
    Func_02004624(0, 2, 10);
    Func_0808a180(1, 0);
    Func_0200463c(2, 3, 10);

    Func_0808a0c8(2, 248, 728);         /* 182 << 2 */
    Func_0808a0d0(1, 248, 728);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
}
