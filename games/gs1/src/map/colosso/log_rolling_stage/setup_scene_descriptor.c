#include "colosso_log_rolling_stage.h"

/*
 * This 232-byte owner ends immediately before InitializeSceneControl at
 * 0x0200457c. Its high-register save and four-byte local frame are required
 * by the seven-argument call shape; the literal pool is part of this owner.
 *
 * SIGNATURE.  Seven arguments: r0-r3, then three incoming stack words.  With
 * 32 bytes pushed and a 4-byte frame the caller's stack arguments sit at
 * [sp,#36], [sp,#40] and [sp,#44], which is exactly what the prologue reads
 * (`ldr r5,[sp,#40]`, `ldr r6,[sp,#44]`, and later `ldr r2,[sp,#36]`).  r3 is
 * spilled to the local slot at [sp,#0] immediately and re-read twice.
 *
 * CALL ACCOUNTING.  Ten `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bc --annotate`: Func_0808a080
 * twice, and one each of Func_08000148, Func_08000170, Func_080770c0,
 * Func_080001a8, Func_080001d0, Func_080001c8, Func_080000d0 and
 * Func_08000178.  The C below reproduces that multiset exactly; the
 * inventory's `calls=10` agrees.
 *
 * LINK BASE 0x02008000, proven for this overlay by a byte-exact sibling
 * (pool word 0x0200804d =
 * Func_0200004c + the Thumb bit).  Two of this owner's pool words fall in the
 * in-image band, and the parity test classifies them:
 *   0x0200cd80  even -> in-image DATA at file offset 0x3f14, passed as the
 *               source of the graphics upload Func_080001a8;
 *   0x0200c0d1  odd  -> Func_020033a0 + the Thumb bit, i.e. a task callback,
 *               installed here with Func_080000d0(callback, 0xc76).
 * That install has the same callback-plus-budget shape as sibling tasks.
 *
 * SHAPE.  Build a scene descriptor: allocate its record (Func_08000148),
 * reserve a 512-entry graphics handle (Func_08000170), stamp the seven
 * arguments into the record's parameter block at +218..+239, optionally patch
 * the second actor's position from the first when story flag 0x109 is clear,
 * upload the descriptor's image and palette, install the per-frame task
 * Func_020033a0, and release the handle.
 *
 * UNCERTAINTIES.
 *  - Record layout is asserted only for the fields written here: u16 at +222,
 *    +224, +226, +228 and +230, s32 at +232 and +236, and the two cleared u16
 *    at +218 and +220 (the pool word 0x00000000 is stored to both) and the
 *    u16 at +216 that receives Func_080001d0's result.
 *  - Func_080001d0 is called with no argument register set; the tree's other
 *    users call it the same way, for a palette slot index.  Its result is
 *    stored as a halfword and then sign-extended (`lsls #16 / asrs #16`)
 *    before being passed on, so the narrowing is deliberate.
 *  - Actor records: only the words at +8 and +16 are touched, and only on the
 *    flag-clear path.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_02008d2e();
s32 Func_02008d48();
u8 *Func_02008f08();
u8 *Func_02008f10();
s32 Func_02008eb0();
void Func_02008dbe();
s32 Func_02008de2();
s32 Func_02008dee();
void Func_02008d8e();
void Func_02008ddc();
                                /* allocate/fetch a record by (id, size) */
                                /* reserve a graphics handle */
                                /* release a graphics handle */
                                /* upload image data to a handle */
                                /* upload a palette ramp */
                                /* next palette slot index */
                                /* install a per-frame task (callback, rate) */
                                /* test a story flag */
                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200cd80 - 0x8000). */
extern u8 Data_0200cd80[];

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200c0d0 - 0x8000). */
void Func_0200c0d0(void);

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Func_02008d2e(59, 0x7170);
    handle = Func_02008d48(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Func_02008f08(first_actor);
    second_record = Func_02008f10(second_actor);

    if (Func_02008eb0(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Func_02008dbe(Data_0200cd80, handle);

    extent = Func_02008de2();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Func_02008dee((s16)extent, 512, handle);

    Func_02008d8e((s32)Func_0200c0d0 + 1, 0xc76);

    Func_02008ddc(handle);
}
