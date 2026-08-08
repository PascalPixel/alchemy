#include "types.h"

/*
 * Resource 370 id-to-glyph mapping at 0x020002f4.
 *
 * Complete owner: `push {lr}` at 0x020002f4 through `pop {r0} / bx r0` at
 * 0x0200037e.  The return address is popped into r0, so the owner is `void`
 * (HANDOVER section 0, epilogue rule).  It contains no literal pool, no
 * branch out of the span, and no call sites at all, matching the inventory's
 * calls=0; `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_370` reports none in
 * this range either.
 *
 * The body is a straight-line chain of range and equality tests on the id in
 * r0, each arm loading one byte value into r3 and branching to the single
 * store at 0x0200037c.  Every comparison uses `bgt`/`bne`, so the id is
 * treated as signed; ids below 8 (including negative ones) take the first arm.
 *
 * The output block's bytes at +1 and +2 are cleared on entry regardless of
 * which arm runs, and only +0 receives the mapped value, so the destination
 * behaves as a three-byte record whose latter two fields this owner does not
 * populate.  Their meaning is not determined here.
 */

void Func_020002f4(s32 id, u8 *out)
{
    s32 value;

    out[1] = 0;
    out[2] = 0;

    if (id <= 7) {
        value = id + 65;
    } else if (id <= 12) {
        value = id + 66;
    } else if (id <= 23) {
        value = id + 67;
    } else if (id <= 31) {
        value = id + 26;
    } else if (id <= 42) {
        value = id + 65;
    } else if (id <= 44) {
        value = id + 66;
    } else if (id <= 55) {
        value = id + 67;
    } else if (id == 56) {
        value = 33;
    } else if (id == 57) {
        value = 63;
    } else if (id == 58) {
        value = 35;
    } else if (id == 59) {
        value = 38;
    } else if (id == 60) {
        value = 36;
    } else if (id == 61) {
        value = 37;
    } else if (id == 62) {
        value = 43;
    } else {
        value = 61;
    }

    out[0] = (u8)value;
}
