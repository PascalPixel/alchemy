#include "types.h"

/*
 * resource_391 owner at 0x02002974, 356 bytes: code 0x02002974-0x02002aa7 and
 * the twelve-word literal pool at 0x02002aa8-0x02002ad7.  A three-phase particle
 * emitter.
 *
 * TRANSPOSED from semantic/overlays/resource_38f_c_020027ac.c.  The two owners
 * are the same routine shared verbatim: over all 178 halfwords they differ in
 * 11 places - 7 BL halfwords (one of the seven BL pairs happens to encode an
 * identical half) and the low halves of the four pool words naming this
 * overlay's own data.  Nothing structural differs.
 *
 * What was changed:
 *  - the four in-image data addresses, all shifted by a constant -0x308:
 *    0x0200b694 -> 0x0200b38c (file offset 0x338c), 0x0200b690 -> 0x0200b388
 *    (0x3388), 0x0200b68c -> 0x0200b384 (0x3384) and 0x0200b610 -> 0x0200b308
 *    (0x3308), under the 0x02008000 link base.  Note that 0x3384/0x3388/0x338c
 *    sit at and just past the end of the assembled reconstruction (0x3384 bytes),
 *    exactly as 0x368c/0x3690/0x3694 do in resource_38f (0x368c bytes) - the
 *    same three-word block relative to the image end in both overlays, which is
 *    itself a check on the substitution.  These overlays are writable EWRAM and
 *    the trailing zero-initialised words are not spelled out in the .s.
 *    The two IWRAM pool words 0x03001e40 and 0x03001e70 are byte-identical and
 *    were not touched.
 *  - the calls, re-resolved with 'bun tools/overlay_call_targets.ts
 *    resource_391 2974': the same 7 sites over the same 5 distinct imports, in
 *    the same site order - Func_080000f8 three times (the random source),
 *    Func_080090c8 (spawn), Func_080f9010 (cue), Func_08009080 and
 *    Func_08009098.  Import names are therefore unchanged.
 *
 * Epilogue 'pop {r5, r6, r7} / pop {r0} / bx r0': r0 holds the popped return
 * address, so the owner returns nothing.  No incoming argument register is read
 * either, so it takes none.
 *
 * The object ABI is inherited from the byte-exact sibling
 * assets/code/resource_38f_c_02002608.c: Func_080090c8(kind, x, y, z) returns
 * the object or 0, +0x50 is the sub-record whose +0x09 bitfield gets
 * '(v & ~12) | 4' and whose +0x26 is cleared, +0x23 gets 'v & 0xfe', and +0x55
 * is cleared.  +0x08/+0x0c/+0x10 are the 16.16 X/Y/Z triple.
 *
 * Behaviour: phases 1 and 2 ramp the spread by 50 up to 14999 / 29999 and, once
 * the height is past a phase ceiling, walk it down by 0x4000 per frame; phase 3
 * walks it down unconditionally until it drops below -128.0, at which point the
 * phase counter is reset to 0.  Then, every eighth frame, one particle is
 * spawned at a randomised offset from the reference record's X/Z, with a cue
 * fired every 64th frame.
 *
 * UNCERTAINTIES (all inherited):
 *  - In phase 3 the reset 'str r5, [r2, #0]' writes through r2, which still
 *    holds the address of the PHASE word from the switch head - phase 3 never
 *    reloads r2.  That is deliberate in the original and is reproduced here; it
 *    is the one place where reading the disassembly casually would give the
 *    height word instead.
 *  - the spread word is used both as the ramp counter and as the multiplier of
 *    the random value in phase 1/2, so it is a spread width in 16.16.  The
 *    '<< 8' on a raw random word in the phase-0 path gives a 0..0xffff00 spread
 *    instead; the two paths are not the same scale, and that asymmetry is in the
 *    original.
 *  - Data_0200b308 is the in-image block handed to Func_08009098; its shape is
 *    not reconstructed, so it stays an opaque byte array.
 */

struct Sub {
    u8 pad00[9];
    u8 f09;
    u8 pad0a[28];
    u8 f26;
};

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[3];
    u8 f23;
    u8 pad24[0x2c];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

/* In-image data under the 0x02008000 link base. */
extern s32 Data_0200b38c;
extern s32 Data_0200b388;
extern s32 Data_0200b384;
extern u8 Data_0200b308[];

/* IWRAM. */
#define FRAME_COUNTER (*(volatile s32 *)0x03001e40)
#define REFERENCE_RECORD (**(s32 ***)0x03001e70)

/* Old-style declarations are mandatory in overlay sources. */
s32 Func_080000f8();
struct Obj *Func_080090c8();
void Func_080f9010();
void Func_08009080();
void Func_08009098();

void Func_02002974(void)
{
    struct Obj *p;
    struct Sub *sub;
    s32 *reference;
    s32 height;
    s32 x;
    s32 z;
    s32 v;

    switch (Data_0200b38c) {
    case 1:
        if (Data_0200b388 <= 0x3a97) {
            Data_0200b388 = Data_0200b388 + 50;
        }
        /* 240 << 14 */
        if (Data_0200b384 > 0x003c0000) {
            Data_0200b384 = Data_0200b384 + (s32)0xffffc000;
        }
        break;
    case 2:
        if (Data_0200b388 <= 0x752f) {
            Data_0200b388 = Data_0200b388 + 50;
        }
        /* 192 << 13 */
        if (Data_0200b384 > 0x00180000) {
            Data_0200b384 = Data_0200b384 + (s32)0xffffc000;
        }
        break;
    case 3:
        height = Data_0200b384;
        if (height < (s32)0xff800000) {
            /* Through the still-live &Data_0200b38c, not &Data_0200b384. */
            Data_0200b38c = 0;
        } else {
            Data_0200b388 = Data_0200b388 + 50;
            Data_0200b384 = height + (s32)0xffffc000;
        }
        break;
    default:
        break;
    }

    if ((FRAME_COUNTER & 7) != 0) {
        return;
    }

    p = Func_080090c8(0x11d, 0, 0, 0);
    if (p == 0) {
        return;
    }

    reference = REFERENCE_RECORD;

    if ((FRAME_COUNTER & 0x3f) == 0) {
        Func_080f9010(246);
    }

    if (Data_0200b38c != 0) {
        v = Data_0200b388 * Func_080000f8();
        x = reference[0] + (s32)(((u32)v >> 16) << 8);
        x = x + Data_0200b384;
    } else {
        x = reference[0] + (Func_080000f8() << 8);
        x = x + (s32)0xff800000;
    }

    z = reference[2] + (Func_080000f8() << 8);
    z = z + (s32)0xff800000;

    p->f55 = 0;
    p->f0c = 0x00a00000;
    sub = p->f50;
    p->f18 = 0xe666;
    p->f1c = 0xe666;
    p->f08 = x;
    p->f10 = z;
    sub->f26 = 0;
    p->f23 = p->f23 & 0xfe;
    sub->f09 = (sub->f09 & ~12) | 4;

    Func_08009080(p, 1);
    Func_08009098(p, Data_0200b308);
}
