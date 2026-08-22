/*
 * resource_3b4 conventions used by this file.
 *
 * `bl` targets: an overlay `bl` stores the target's image offset minus two,
 * not a pc-relative displacement, so a disassembler's printed target is wrong.
 * Every call below was resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.  The band
 * 0x02002468-0x0200261f is an 8-byte-per-entry import veneer table
 * (`ldr r4,[pc,#0] / bx r4 / .word <main-image address>`); calls landing there
 * are named by that trailing word, as the semantic resource_39f sources do.
 * Calls landing on an overlay prologue keep the overlay's `Func_0200xxxx` name.
 *
 * Link base: resource_3b4 is linked at 0x02008000.  The image is 0x2f84 bytes
 * and the table at 0x02002d40-0x02002f84 holds 59 words of the form
 * `<function offset> + 0x8000 + 1` (Thumb bit), e.g. 0x02008ec9 = Func_02000ec8.
 * So a pool word in 0x02008000-0x0200af83 is in-image data at value - 0x8000.
 * `Data_02000240` lies below the base and is referenced by byte-exact sources
 * in many other overlays, so it is a shared EWRAM global, not overlay data.
 *
 * Return type: `pop {r0} / bx r0` pops the return address into r0, so nothing
 * is returned and the owner is void.
 *
 * Declarations are old-style because imports here are reached with differing
 * argument counts from different call sites.
 */
#include "types.h"

/*
 * resource_3b4 owner at 0x02001edc, 156 bytes: code 0x02001edc-0x02001f73 and
 * the pool word 0x000fffff at 0x02001f74.
 *
 * Publish one marker byte (+35) to slots 8, 10, 11 and 12 according to slot 0's
 * height word at +12.  Above 0x100000 (`movs r2,#128 / lsls r2,#13`) the marker
 * is 2 and slot 8 takes it unconditionally; otherwise the marker is 0 and slot
 * 10 is instead driven through Func_0808a1e0 with mode 3 when it is already at
 * height 0 and slot 0 stands past Z tile 56, and with mode 1 (plus its own
 * marker 1) in every other case.
 *
 * r5 carries the marker; the `movs r5,#0` at 0x02001f5e is branched over by the
 * high path, which is why that path keeps the value 2.  Slot 11's record is
 * fetched separately on each path -- both fetches join at the shared store --
 * so the two Func_0808a080(11) calls below are one per path, as in the
 * assembly.
 *
 * The epilogue is `pop {r5} / pop {r0} / bx r0`, so the owner is void.
 */

/* Returns the record for a numbered slot. */



extern s32 * Func_02004422();
extern s32 * Func_02004432();
extern s32 * Func_0200443e();
extern s32 * Func_0200444a();
extern s32 * Func_02004454();
extern s32 * Func_0200445c();
extern s32 * Func_02004468();
extern void Func_020044e0();
extern void Func_020044ea();
extern s32 * Func_02004490();
extern s32 * Func_0200449c();
extern s32 * Func_020044a8();
void Func_02001edc(void)
{
    s32 *slot0 = Func_02004422(0);
    u8 marker;

    if (slot0[3] > 0x100000) {                 /* +12 */
        marker = 2;
        ((u8 *)Func_02004432(8))[35] = marker;
        if (Func_0200443e(10)[3] == 0) {
            ((u8 *)Func_0200444a(10))[35] = marker;
        }
        ((u8 *)Func_02004454(11))[35] = marker;
    } else {
        if (Func_0200445c(10)[3] == 0 &&
            Func_02004468(0)[4] / 0x100000 > 56) {   /* +16 */
            Func_020044e0(10, 3);
        } else {
            Func_020044ea(10, 1);
            ((u8 *)Func_02004490(10))[35] = 1;
        }
        marker = 0;
        ((u8 *)Func_0200449c(11))[35] = marker;
    }

    ((u8 *)Func_020044a8(12))[35] = marker;
}
