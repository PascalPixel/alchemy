#include "types.h"

/*
 * resource_3ba owner at 0x02003764, 232 bytes (0x02003764-0x0200384b).
 *
 * Prologue at 0x02003764: `push {r5, r6, r7, lr}`, then the high-register save
 * `mov r7, fp / mov r6, sl / mov r5, r9 / push {r5, r6, r7}` and
 * `mov r7, r8 / push {r7}`, then `sub sp, #4` — eight registers (32 bytes)
 * plus a 4-byte frame.  The body ends at 0x02003820 with `add sp, #4` and a
 * `b.n` over the six-word literal pool at 0x02003822-0x0200383b to the unwind
 * at 0x0200383c..0x0200384a.  The next row, 0x0200384c, is a separate owner
 * whose byte-exact source is `assets/code/resource_3ba_c_0200384c.c`, so
 * nothing live crosses this row's end.
 *
 * `pop {r0} ; bx r0` — r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * SIGNATURE.  Seven arguments: r0-r3, then three incoming stack words.  With
 * 32 bytes pushed and a 4-byte frame the caller's stack arguments sit at
 * [sp,#36], [sp,#40] and [sp,#44], which is exactly what the prologue reads
 * (`ldr r5,[sp,#40]`, `ldr r6,[sp,#44]`, and later `ldr r2,[sp,#36]`).  r3 is
 * spilled to the local slot at [sp,#0] immediately and re-read twice.
 *
 * CALL ACCOUNTING.  Ten `bl` sites, all resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3ba --annotate`: Func_0808a080
 * twice, and one each of Func_08000148, Func_08000170, Func_080770c0,
 * Func_080001a8, Func_080001d0, Func_080001c8, Func_080000d0 and
 * Func_08000178.  The C below reproduces that multiset exactly; the
 * inventory's `calls=10` agrees.
 *
 * LINK BASE 0x02008000, proven for this overlay by the byte-exact sibling
 * `assets/code/resource_3ba_c_02000158.c` (pool word 0x0200804d =
 * Func_0200004c + the Thumb bit).  Two of this owner's pool words fall in the
 * in-image band, and the parity test classifies them:
 *   0x0200bf14  even -> in-image DATA at file offset 0x3f14, passed as the
 *               source of the graphics upload Func_080001a8;
 *   0x0200b3a1  odd  -> Func_020033a0 + the Thumb bit, i.e. a task callback,
 *               installed here with Func_080000d0(callback, 0xc76).
 * That install is the same shape as the byte-exact 0x02000158's
 * `Func_080000d0(0x0200804d, 0xc80)`.  Note in passing that the byte-exact
 * `resource_3ba_c_0200384c.c` declares the analogous word 0x0200b1c1 as
 * `u8 Data_0200b1c1[]`; by the same parity rule it is Func_020031c0 + 1, a
 * callback, not data — a spelling worth correcting when that file is revisited.
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
u8 *Func_020072c6();
s32 Func_020072e0();
u8 *Func_02007478();
u8 *Func_02007480();
s32 Func_02007428();
void Func_02007356();
s32 Func_0200737a();
void Func_02007386();
void Func_02007326();
void Func_02007374();
                                /* allocate/fetch a record by (id, size) */
                                /* reserve a graphics handle */
                                /* release a graphics handle */
                                /* upload image data to a handle */
                                /* upload a palette ramp */
                                /* next palette slot index */
                                /* install a per-frame task (callback, rate) */
                                /* test a story flag */
                                /* scene record for an actor selector */

/* In-image data at file offset 0x3f14 (0x0200bf14 - 0x8000). */
extern u8 Data_0200bf14[];

/* The per-frame task this owner installs; in-image code at file offset
 * 0x33a0 (0x0200b3a0 - 0x8000). */
void Func_020033a0(void);

void Func_02003764(s32 actorA, s32 actorB, s32 param3, s32 param4,
                   s32 param5, s32 param6, s32 param7)
{
    u8 *scene;
    s32 handle;
    u8 *recordA;
    u8 *recordB;
    s32 paletteIndex;

    scene = Func_020072c6(59, 0x7170);
    handle = Func_020072e0(0x200);

    *(u16 *)(scene + 222) = (u16)actorA;
    *(u16 *)(scene + 224) = (u16)actorB;
    *(u16 *)(scene + 226) = (u16)param6;
    *(u16 *)(scene + 228) = (u16)param7;
    *(u16 *)(scene + 230) = (u16)param3;
    *(s32 *)(scene + 232) = param4;
    *(s32 *)(scene + 236) = param5;

    recordA = Func_02007478(actorA);
    recordB = Func_02007480(actorB);

    if (Func_02007428(0x109) == 0) {
        *(s32 *)(recordB + 8) = (param4 * 2) - *(s32 *)(recordA + 8);
        *(s32 *)(recordB + 16) = *(s32 *)(recordA + 16);
    }

    *(u16 *)(scene + 218) = 0;
    *(u16 *)(scene + 220) = 0;

    Func_02007356(Data_0200bf14, handle);

    paletteIndex = Func_0200737a();
    *(u16 *)(scene + 216) = (u16)paletteIndex;
    Func_02007386((s16)paletteIndex, 0x200, handle);

    Func_02007326((void (*)(void))((s32)&Func_020033a0 | 1), 0xc76);

    Func_02007374(handle);
}
