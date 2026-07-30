typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_391 owner at 0x020027c8, 156 bytes: code 0x020027c8-0x02002851,
 * two alignment bytes at 0x02002852, and the four-word literal pool at
 * 0x02002854-0x02002863.  A once-per-eight-frames one-shot spawner.
 *
 * Same object ABI and the same import set as the already-converted sibling
 * semantic/overlays/resource_391_c_02002974.c, whose ABI is itself inherited
 * from the byte-exact assets/code/resource_38f_c_02002608.c:
 *   Func_080090c8(kind, x, y, z) returns the object or 0,
 *   +0x50 is the sub-record whose +0x09 bitfield gets '(v & ~12) | 4' and whose
 *   +0x26 is cleared, +0x23 gets 'v & 0xfe', +0x55 is cleared,
 *   +0x08/+0x0c/+0x10 are the 16.16 X/Y/Z triple.
 *
 * Calls resolved with 'bun tools/overlay_call_targets.ts resource_391 27c8'
 * (an overlay bl stores target_offset - 2, so overlay_show's annotations are
 * wrong): 5 sites, 5 distinct targets, all import veneers -
 *   0x27e0 -> 0x2d24 -> Func_080f9010 (cue)
 *   0x27f0 -> 0x2bb4 -> Func_080090c8 (spawn)
 *   0x2830 -> 0x2ba4 -> Func_08009080
 *   0x2840 -> 0x2bcc -> Func_08009150
 *   0x2848 -> 0x2bac -> Func_08009098
 * Four of the five are the same imports 0x02002974 uses; Func_08009150 is the
 * one this owner adds.  placed(5) >= inventory calls=5.
 *
 * Epilogue 'pop {r5, r6} / pop {r0} / bx r0': r0 holds the popped return
 * address, so the owner returns nothing.  No incoming argument register is read
 * before being written, so it takes none.
 *
 * Link base 0x02008000 (confirmed for this overlay by the sibling conversion):
 * the pool words 0x0200b398 and 0x0200b2d0 are in-image data at file offsets
 * 0x3398 and 0x32d0.  0x32d0 is inside the assembled reconstruction; 0x3398
 * sits just past its 0x3384-byte end, in the same trailing zero-initialised
 * block that 0x02002974 uses for its phase/spread/height words - the overlay
 * image is writable EWRAM, so those words are live state, not constants.
 *
 * UNCERTAINTIES:
 *  - Data_0200b398 is only tested against zero here; nothing in this owner
 *    writes it.  Per the shared-globals rule its writer is elsewhere in the
 *    overlay, so its exact meaning (a "cue already played" gate, most likely)
 *    is not settled from this owner alone.  Typed s32 because the read is a
 *    full-word 'ldr'.
 *  - Data_0200b2d0 is the in-image block handed to Func_08009098; its shape is
 *    not reconstructed, so it stays an opaque byte array, exactly as
 *    Data_0200b308 does in the sibling.
 *  - r6 is loaded with the masked frame counter and is provably 0 on every path
 *    that reaches the stores, so the three 'strb r6' are spelled as literal 0.
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
    u8 pad1c[0x23 - 0x1c];
    u8 f23;
    u8 pad24[0x30 - 0x24];
    s32 f30;
    s32 f34;
    u8 pad38[0x50 - 0x38];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

/* In-image data under the 0x02008000 link base. */
extern s32 Data_0200b398;
extern u8 Data_0200b2d0[];

/* IWRAM. */
#define FRAME_COUNTER (*(volatile s32 *)0x03001e40)

/* Old-style declarations are mandatory in overlay sources. */
struct Obj *Func_080090c8();
void Func_080f9010();
void Func_08009080();
void Func_08009098();
void Func_08009150();

void Func_020027c8(void)
{
    struct Obj *p;
    struct Sub *sub;

    if ((FRAME_COUNTER & 7) != 0) {
        return;
    }

    if (Data_0200b398 != 0) {
        Func_080f9010(200);
    }

    /* kind 26 at (231.0, 0, 460.0) in 16.16. */
    p = Func_080090c8(26, 231 << 16, 0, 230 << 17);
    if (p == 0) {
        return;
    }

    sub = p->f50;
    sub->f26 = 0;
    p->f23 = p->f23 & 0xfe;
    sub->f09 = (sub->f09 & ~12) | 4;
    p->f18 = 0x1999;
    p->f30 = 0x80000;
    p->f34 = 0x80000;
    p->f55 = 0;

    Func_08009080(p, 2);
    Func_08009150(p, 231 << 16, 0, 156 << 18);
    Func_08009098(p, Data_0200b2d0);
}
