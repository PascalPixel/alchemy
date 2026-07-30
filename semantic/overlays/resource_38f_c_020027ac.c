typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_38f owner at 0x020027ac, 356 bytes: code 0x020027ac-0x020028df and
 * the twelve-word literal pool at 0x020028e0-0x0200290f.  The next inventory
 * row starts at 0x02002910 with its own `push {lr}`, so the owner is exactly
 * its advertised span.
 *
 * Epilogue `pop {r5, r6, r7} / pop {r0} / bx r0`: r0 holds the popped return
 * address, so the owner returns nothing.  No incoming argument register is
 * read either - r2/r3 are overwritten from the pool before anything else and
 * every call site sets its own arguments - so it takes none.
 *
 * Call accounting: 7 `bl` sites, 5 distinct import veneers, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 27ac`.  Func_080000f8 is
 * reached three times (0x0200285a, 0x02002874, 0x02002882) with no argument
 * register set and its r0 consumed each time - the image's random source.  The
 * remaining four are Func_080090c8 (spawn), Func_080f9010 (cue),
 * Func_08009080 and Func_08009098.  The disassembler's own `bl` annotations
 * are wrong in the usual overlay way and were not used.
 *
 * The object ABI is not inferred here, it is copied from the byte-exact
 * sibling assets/code/resource_38f_c_02002608.c, which spawns through the same
 * veneer and touches the same fields: Func_080090c8(kind, x, y, z) returns the
 * object or 0, +0x50 is the sub-record whose +0x09 bitfield gets
 * `(v & ~12) | 4` and whose +0x26 is cleared, +0x23 gets `v & 0xfe`, and
 * +0x55 is cleared.  This owner additionally writes +0x08/+0x0c/+0x10 (the
 * 16.16 X/Y/Z triple that resource_373 and resource_39f settled for the same
 * record shape) and +0x18/+0x1c.
 *
 * All four 0x0200_bxxx pool words are in-image addresses under this overlay's
 * confirmed 0x02008000 link base (jump-table witness in
 * semantic/overlays/resource_38f_c_02000304.c): 0x0200b694 -> file offset
 * 0x3694, 0x0200b690 -> 0x3690, 0x0200b68c -> 0x368c, 0x0200b610 -> 0x3610.
 * 0x03001e40 and 0x03001e70 are genuine IWRAM: the first is the frame counter
 * the byte-exact siblings mask with 7, the second a pointer to a pointer to
 * the reference record read at +0 and +8.
 *
 * Behaviour: a three-phase emitter.  Phase 1 and 2 ramp Data_0200b690 by 50 up
 * to 14999 / 29999 respectively and, once Data_0200b68c is past a phase
 * ceiling, walk it down by 0x4000 per frame; phase 3 walks it down
 * unconditionally until it drops below -128.0, at which point the phase
 * counter is reset to 0.  Then, every eighth frame, one particle is spawned at
 * a randomised offset from the reference record's X/Z, with a cue fired every
 * 64th frame.
 *
 * UNCERTAINTIES:
 *  - In phase 3 the reset `str r5, [r2, #0]` writes through r2, which still
 *    holds &Data_0200b694 from the switch head - phase 3 never reloads r2.
 *    That is deliberate in the original and is reproduced here; it is the one
 *    place where reading the disassembly casually would give &Data_0200b68c.
 *  - Data_0200b690 is used both as the ramp counter and as the multiplier of
 *    the random value in phase 1/2 (`(u32)(ramp * rand) >> 16 << 8`), so it is
 *    a spread width in 16.16.  The `<< 8` on a raw random word in the phase-0
 *    path gives a 0..0xffff00 spread instead; the two paths are not the same
 *    scale and that asymmetry is in the original.
 *  - Data_0200b610 is the in-image block handed to Func_08009098, the same
 *    argument position where the byte-exact sibling passes Data_0200b5d8.  Its
 *    shape is not reconstructed, so it stays an opaque byte array.
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
extern s32 Data_0200b694;
extern s32 Data_0200b690;
extern s32 Data_0200b68c;
extern u8 Data_0200b610[];

/* IWRAM. */
#define FRAME_COUNTER (*(volatile s32 *)0x03001e40)
#define REFERENCE_RECORD (**(s32 ***)0x03001e70)

/* Old-style declarations are mandatory in overlay sources. */
s32 Func_080000f8();
struct Obj *Func_080090c8();
void Func_080f9010();
void Func_08009080();
void Func_08009098();

void Func_020027ac(void)
{
    struct Obj *p;
    struct Sub *sub;
    s32 *reference;
    s32 height;
    s32 x;
    s32 z;
    s32 v;

    switch (Data_0200b694) {
    case 1:
        if (Data_0200b690 <= 0x3a97) {
            Data_0200b690 = Data_0200b690 + 50;
        }
        /* 240 << 14 */
        if (Data_0200b68c > 0x003c0000) {
            Data_0200b68c = Data_0200b68c + (s32)0xffffc000;
        }
        break;
    case 2:
        if (Data_0200b690 <= 0x752f) {
            Data_0200b690 = Data_0200b690 + 50;
        }
        /* 192 << 13 */
        if (Data_0200b68c > 0x00180000) {
            Data_0200b68c = Data_0200b68c + (s32)0xffffc000;
        }
        break;
    case 3:
        height = Data_0200b68c;
        if (height < (s32)0xff800000) {
            /* Through the still-live &Data_0200b694, not &Data_0200b68c. */
            Data_0200b694 = 0;
        } else {
            Data_0200b690 = Data_0200b690 + 50;
            Data_0200b68c = height + (s32)0xffffc000;
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

    if (Data_0200b694 != 0) {
        v = Data_0200b690 * Func_080000f8();
        x = reference[0] + (s32)(((u32)v >> 16) << 8);
        x = x + Data_0200b68c;
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
    Func_08009098(p, Data_0200b610);
}
