/*
 * resource_380 owner at 0x0200449c, 728 bytes: 0x0200449c-0x02004773.
 *
 * NO INVENTORY ROW.  Found by sweeping the two-byte gaps in
 * `metrics/gs1-en-executable.json` for `push {..,lr}` prologues; there is no
 * row for it in `out/decomp/overlays.json`, no exact sibling and no semantic
 * source, so the span below is DERIVED.
 *
 * How the span was established.  A control-flow walk from the prologue at
 * 0x0200449c, following every branch, reaches its last instruction at
 * 0x02004772 and closes on the single matching unwind
 * `add sp,#68 / pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02004762-0x02004773.  One epilogue, no `mov pc, rN` dispatch, and no `bl`
 * whose `+2` target lands inside the body, so there is no long tail branch and
 * nothing behind a jump table.  Extent 0x0200449c-0x02004774 = 728 bytes.
 *
 * POOL MAP: EMPTY.  Every one of the 728 bytes is an instruction the walk
 * reaches - this owner has no interior pool and no alignment hole.  Its
 * literals live in the pool that begins immediately at 0x02004774
 * (0x0200d0e4, 0x0200d102, 0x0200d140, 0x0200d168, 0x00001999, 0x0200d120,
 * ...), which is OUTSIDE the registered span; the pool is followed directly by
 * the next function with nothing between, so the boundary rests on the
 * epilogue alone.
 *
 * LINK BASE.  All five in-image pool words are EVEN and land inside the
 * assembled image (0x5190 bytes) under `pool_word - 0x8000`: 0x0200d0e4 ->
 * 0x020050e4, 0x0200d102 -> 0x02005102, 0x0200d120 -> 0x02005120,
 * 0x0200d140 -> 0x02005140 and 0x0200d168 -> 0x02005168.  Even = data, and all
 * five are read as data here, so the parity rule holds without exception.  The
 * spacing settles their shapes: 0x5140 and 0x5168 are 0x28 = 40 bytes apart,
 * which is ten s32 entries, and the three byte tables are indexed 3 per
 * element, so every table is sized for the same ten elements as the array this
 * owner walks.
 *
 * Call accounting: 10 `bl` sites over 5 distinct targets, all import veneers -
 * no intra-overlay call and no `call_via` slot.  Resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_380 449c 4774`, never from
 * `overlay_show.ts`'s printed annotations.  Per-target multiset:
 * Func_030003f0 x3, Func_080000f8 x3, Func_08000118 x2, Func_08000120 x1,
 * Func_08000148 x1.
 *
 * WHAT IT IS.  One frame of a shake/settle update over an array of at most ten
 * 40-byte records obtained from `Func_08000148(33, 404)` - 404 = ten records of
 * 40 bytes plus the u16 live count at +400.  For each live record it
 *   * decrements a 3-frame countdown at +36 and does nothing but write back
 *     until the countdown reaches zero, so the expensive part runs every third
 *     frame;
 *   * when the burst counter at +37 is zero, integrates the fall value at +32
 *     into the height at +28 and bounces: past `limit[i]` the fall is negated,
 *     and at or below 0x1999 the height is clamped to 0x1999, the fall is set
 *     to `+bounce[i]`, the attached object's three position words are saved
 *     into the record and zeroed, and a 24-frame burst is armed;
 *   * draws three random magnitudes, each `random * amplitude[3i+k] >> 16`,
 *     divides each by 1000 through the relocated IWRAM helper
 *     `Func_030003f0`, and applies them to the three sway values at +16/+20/+24
 *     under the sign table at 0x02005120: `+1` adds, `-1` subtracts, and any
 *     other value subtracts and then forces the sway to zero;
 *   * shapes each sway by `Func_08000118` / `Func_08000120` scaled by the
 *     weight table at 0x02005102, doubles the result, and either accumulates it
 *     into the record's own three burst words (while the burst counter is
 *     running, flushing them into the object when the counter expires) or adds
 *     it straight into the object.
 * Every intermediate is written back to the record before the next element, so
 * the whole thing is a per-element state machine.
 *
 * TWO DETAILS THAT LOOK LIKE NOISE AND ARE NOT:
 *  - The Y axis is guarded twice.  Both object updates test the SCALED Y
 *    magnitude (r9), not the shaped value, and r9 survives because it is a high
 *    register while its X and Z counterparts have already been overwritten by
 *    the shaping results.  So a zero Y magnitude leaves the object's Y
 *    untouched, while X and Z are written unconditionally.  Reading the test
 *    against the shaped value would change which frames move Y.
 *  - The countdown is decremented as a u8 (`adds #255 / lsls #24 / lsrs #24`),
 *    reloaded to 3 only on the frame it expires, and stored back through a
 *    stack copy that is re-read as a byte at the very end.  Both the decrement
 *    and the reload are visible in the record, so both are written.
 *
 * The compiler addresses one element through two bases at once - `sl` is
 * `element + 8` while the stack slot holds `element` - so `sl->f0` and
 * `element->f8` are the SAME word.  They are written as one field here; a
 * reading that keeps both bases invents a field that does not exist.
 *
 * The epilogue's final `pop {r0}` takes the return address, so the owner is
 * `void`.  No argument register is read before it is written, so the owner
 * takes no arguments.
 *
 * UNCERTAINTIES:
 *  - Func_08000118 and Func_08000120 are the two shaping helpers; their exact
 *    curves are not established here, only that each takes one argument and
 *    that the result is doubled.
 *  - 0x1999 is the low clamp and reads as a fixed-point floor; nothing here
 *    names its scale.
 *  - The object fields at +56/+60/+64 are written with the same values as
 *    +8/+12/+16 on every path, which reads as a shadow copy for a renderer,
 *    but no consumer is visible from here.
 */

#include "types.h"

/* Old-style declarations: overlay imports vary in arity between call sites. */
s32 Func_030003f0();            /* relocated IWRAM quotient helper */
s32 Func_080000f8();            /* random */
s32 Func_08000118();            /* shaping helper */
s32 Func_08000120();            /* shaping helper, second curve */
u8 *Func_08000148();            /* fetch the working block by id and size */

/* In-image data, under the 0x02008000 link base. Three bytes per element. */
extern const u8 Data_020050e4[];        /* 0x0200d0e4: random amplitudes */
extern const u8 Data_02005102[];        /* 0x0200d102: shaping weights */
extern const s8 Data_02005120[];        /* 0x0200d120: per-axis sign */
/* One s32 per element. */
extern const s32 Data_02005140[];       /* 0x0200d140: height limit */
extern const s32 Data_02005168[];       /* 0x0200d168: bounce magnitude */

struct Shaken_0200449c {
    s32 *object;        /* +0  the attached object */
    s32 burstX;         /* +4  */
    s32 burstY;         /* +8  also reached as (element + 8)->f0 */
    s32 burstZ;         /* +12 */
    s32 swayX;          /* +16 */
    s32 swayY;          /* +20 */
    s32 swayZ;          /* +24 */
    s32 height;         /* +28 */
    s32 fall;           /* +32 */
    u8 countdown;       /* +36 */
    u8 burst;           /* +37 */
    u8 pad[2];          /* +38 */
};

struct ShakenBlock_0200449c {
    struct Shaken_0200449c elements[10];
    u16 live;                                  /* +400 */
};

void Func_0200449c(void)
{
    struct ShakenBlock_0200449c *block;
    struct Shaken_0200449c *element;
    s32 index;

    block = (struct ShakenBlock_0200449c *)Func_08000148(33, 202 << 1);   /* 404 */
    index = 0;
    if (block->live == 0) return;

    element = &block->elements[0];
    do {
        s32 swayX = element->swayX;
        s32 swayY = element->swayY;
        s32 swayZ = element->swayZ;
        s32 height = element->height;
        s32 burstX = element->burstX;
        s32 burstY = element->burstY;
        s32 fall = element->fall;
        s32 burstZ = element->burstZ;
        s32 *object = element->object;
        u8 burst = element->burst;
        u8 countdown;
        s32 magnitudeX;
        s32 magnitudeY;
        s32 magnitudeZ;
        s32 shapedX;
        s32 shapedY;
        s32 shapedZ;
        s8 sign;

        countdown = (u8)(element->countdown - 1);
        if (countdown != 0) goto store;

        countdown = 3;

        if (burst == 0) {
            height += fall;
            if (height >= Data_02005140[index]) {
                fall = -Data_02005168[index];
            } else if (height <= 0x1999) {
                fall = Data_02005168[index];
                height = 0x1999;
                burstX = object[2];             /* +8  */
                burstY = object[3];             /* +12 */
                burstZ = object[4];             /* +16 */
                object[2] = 0;
                object[3] = 0;
                object[4] = 0;
                burst = 24;
            }
            object[6] = height;                 /* +24 */
            object[7] = height;                 /* +28 */
        }

        magnitudeX = (Data_020050e4[3 * index] * Func_080000f8()) >> 16;
        magnitudeY = (Data_020050e4[3 * index + 1] * Func_080000f8()) >> 16;
        magnitudeZ = (Data_020050e4[3 * index + 2] * Func_080000f8()) >> 16;

        magnitudeX = magnitudeX != 0 ? Func_030003f0(magnitudeX << 16, 250 << 2) : 0;
        magnitudeY = magnitudeY != 0 ? Func_030003f0(magnitudeY << 16, 250 << 2) : 0;
        magnitudeZ = magnitudeZ != 0 ? Func_030003f0(magnitudeZ << 16, 250 << 2) : 0;

        sign = Data_02005120[3 * index];
        if (sign == 1) {
            swayX += magnitudeX;
        } else {
            swayX -= magnitudeX;
            if (sign != -1) swayX = 0;
        }

        sign = Data_02005120[3 * index + 1];
        if (sign == 1) {
            swayY += magnitudeY;
        } else {
            swayY -= magnitudeY;
            if (sign != -1) swayY = 0;
        }

        sign = Data_02005120[3 * index + 2];
        if (sign == 1) {
            swayZ += magnitudeZ;
        } else {
            swayZ -= magnitudeZ;
            if (sign != -1) swayZ = 0;
        }

        shapedX = Func_08000118(Data_02005102[3 * index] * swayX) << 1;
        shapedY = Func_08000118(Data_02005102[3 * index + 1] * swayY) << 1;
        shapedZ = Func_08000120(Data_02005102[3 * index + 2] * swayZ) << 1;

        if (burst != 0) {
            burstX += shapedX;
            burstY += shapedY;
            burstZ += shapedZ;
            burst = (u8)(burst - 1);
            if (burst != 0) goto store;
            object[2] = burstX;
            object[14] = burstX;                /* +56 */
            if (magnitudeY != 0) {
                object[3] = burstY;
                object[15] = burstY;            /* +60 */
            }
            object[4] = burstZ;
            object[16] = burstZ;                /* +64 */
        } else {
            object[2] += shapedX;
            object[14] = object[2];
            if (magnitudeY != 0) {
                object[3] += shapedY;
                object[15] = object[3];
            }
            object[4] += shapedZ;
            object[16] = object[4];
        }

    store:
        element->swayX = swayX;
        element->swayY = swayY;
        element->swayZ = swayZ;
        element->height = height;
        element->fall = fall;
        element->burst = burst;
        element->burstX = burstX;
        element->burstY = burstY;
        element->burstZ = burstZ;
        element->countdown = countdown;

        index += 1;
        element += 1;
    } while (index != block->live);
}
