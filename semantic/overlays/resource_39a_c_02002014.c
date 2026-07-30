typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 39a overlay per-frame heading step at 0x02002014.
 *
 * Complete owner: `push {r5, lr}` at 0x02002014 and `pop {r5} / pop {r0} /
 * bx r0` at 0x02002082, so nothing is returned; every early exit branches to
 * that same epilogue.  Three pool words follow the return
 * (0x03001ae8, 0x0200a424 and 0xf848f003) and are data - the reconstruction
 * assembly spells the last one as two raw halfwords precisely because it
 * would otherwise decode as a branch.
 *
 * 0x0200a424 is 0x02008000 + 0x2424, in-image data of this overlay under its
 * proven 0x02008000 link base: a 16-entry signed halfword table selected by
 * bits 4-7 of the word at 0x03001ae8, with -1 meaning "no target".
 *
 * Uncertainty: the lower clamp is the pooled constant 0xf848f003.  The value
 * being clamped is a sign-extended 16-bit difference, so that bound can never
 * be reached; it is reproduced faithfully rather than normalised.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

extern s16 Data_0200a424[];
extern u32 Data_03001ae8;

/* Imports. */
void Func_020042f0();
void Func_02004318();
void Func_02004328();

void Func_02002014(u8 *subject)
{
    s16 target;
    s16 heading;
    s32 step;

    /* A countdown halfword at +100; while it is non-zero nothing else runs. */
    if (*(s16 *)(subject + 100) != 0) {
        *(u16 *)(subject + 100) = (u16)(*(u16 *)(subject + 100) - 1);
        return;
    }

    subject[90] = 0;
    target = Data_0200a424[(Data_03001ae8 >> 4) & 15];
    if (target == -1) {
        Func_020042f0(subject, 9);
        return;
    }

    heading = *(s16 *)(subject + 6);
    /* lsls #16 / asrs #16: the difference is taken as a signed halfword. */
    step = (s16)(target - heading);
    /* movs r2,#0x80 / lsls r2,#5 builds the 0x1000 upper bound. */
    if (step > 0x1000) {
        step = 0x1000;
    }
    if (step < (s32)0xf848f003) {
        step = (s32)0xf848f003;
    }
    *(u16 *)(subject + 6) = (u16)(heading + step);
    Func_02004318(subject, 2);
    Func_02004328(subject, 48);
}
