#include "types.h"

/*
 * Resource 37f, owner at 0x020007d0 (260 bytes of code, prologue
 * `push {r5, r6, r7, lr}` + the r8/r9/sl spill + `sub sp, #12` at 0x020007d0
 * through the interworking return at 0x020008d4, plus a four-word literal pool
 * at 0x020008e4-0x020008f3).  r0 is the popped return address, so `void`.
 *
 * All 18 call sites were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`
 * (18 = the inventory's `calls` field): 8 import veneers, 1 call to the
 * byte-exact Func_0200079c, and 9 calls to this overlay's own scene routines.
 * The last nine are the two-arm scene selection at the tail, and they are
 * exactly the eight-member sibling family at 0x0200056c..0x02000754 plus
 * Func_0200079c.
 *
 * `Data_0200a65c` is the packed direction table documented for this family:
 * 16 entries indexed by `heading >> 12`, X step in the HIGH half and Z step in
 * the LOW half.  It is used twice and in two ways — first sign-extended to
 * integer tile steps (`>> 16` arithmetic, and a 16-bit sign extension of the
 * low half), then promoted to 16.16 by masking with 0xffff0000 and by shifting
 * left 16, never by multiplying.  The pool word is even, so under this
 * overlay's 0x02008000 link base it is in-image data at file offset 0x265c.
 *
 * Uncertainties:
 *  - Func_08009080 is called twice.  At the second site r2 has just been set
 *    to 0, but only in order to serve the four `str` instructions around it,
 *    so it is recorded as a two-argument call at both sites rather than as a
 *    three-argument call once.  This is the documented "offset register reads
 *    as a phantom last argument" shape.
 *  - Func_080091d8's result is only tested with `bgt`, so it is declared s32.
 *  - The 0x3333 written to slot+48/+52 and actor+48/+52 is a plain pool
 *    constant, not an address.
 */

extern u32 Data_0200a65c[];
extern s16 Data_02000240[];

s32 Func_020023ac();
s32 *Func_02000fa6();
s32 Func_020023aa();
void Func_02002386();
void Func_02002376();
void Func_02002524();
void Func_020023b4();
void Func_020023c6();
void Func_020023d4();
void Func_020023d2();
void Func_02000e20(void);
void Func_02000e64(void);
void Func_02000eaa(void);
void Func_02000ef6(void);
void Func_02000f42(void);
void Func_02000f8e(void);
void Func_02000fda(void);
void Func_02001026(void);

                    

                     

                         

                         

                         

void Func_020007d0(void)
{
    u8 *actor;
    s32 *slot;
    u32 step;
    s32 pos[3];
    s16 substep;

    actor = (u8 *)Func_020023ac(0);

    step = Data_0200a65c[*(u16 *)(actor + 6) >> 12];

    /* Integer tile coordinates: the s16 views at +10 and +18 are the integer
     * parts of the 16.16 words at +8 and +16. */
    slot = Func_02000fa6(
        (*(s16 *)(actor + 10) + (s32)((s32)step >> 16)) >> 4,
        (*(s16 *)(actor + 18) + (s32)(s16)step) >> 4);
    if (slot == 0) {
        return;
    }

    *(u8 *)((u8 *)slot + 34) = 2;

    pos[0] = slot[2] + (s32)(step & 0xffff0000);
    pos[1] = slot[3];
    pos[2] = slot[4] + (s32)(step << 16);

    if (Func_020023aa(slot, pos) > 0) {
        return;
    }

    Func_02002386(actor, 8);
    Func_02002376(15);
    Func_02002524(185);

    Func_020023b4(slot, pos[0], pos[1], pos[2]);
    slot[12] = 0x3333;
    slot[13] = 0x3333;

    Func_020023c6(actor, pos[0], pos[1], pos[2]);
    *(s32 *)(actor + 48) = 0x3333;
    *(s32 *)(actor + 52) = 0x3333;

    Func_020023d4(slot);

    slot[2] = pos[0];
    Func_020023d2(actor, 1);
    slot[4] = pos[2];
    slot[9] = 0;
    slot[11] = 0;

    substep = Data_02000240[225];
    if (substep >= 11 && substep <= 13) {
        Func_02000e20();
        Func_02000e64();
    } else if (substep >= 14 && substep <= 16) {
        Func_02000eaa();
        Func_02000ef6();
        Func_02000f42();
        Func_02000f8e();
        Func_02000fda();
        Func_02001026();
    }
}
