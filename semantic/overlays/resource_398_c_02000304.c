typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 398 paired-actor gate scene at 0x02000304.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000304 and the matching
 * `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02000414.  288-byte row:
 * 280 bytes of code plus the two pool words 0x00000303 and 0x00000302 at
 * 0x0200041c-0x02000423.  The pool map is from a control-flow walk; every
 * branch target in the body is below 0x02000414 and nothing reaches 0x0200041c.
 * The return address is popped into r0, so the owner is `void`.
 *
 * Role.  Two interaction records name this address (pool word 0x02008305 =
 * 0x0200_0304 + the Thumb bit under the proven 0x02008000 link base), keyed
 * 0x00008c15/0xffff000b and 0x00008c15/0xffff000c — the two halves of the
 * pair the body reads as slots 11 and 12.
 *
 * Tile tests.  `ldr r3,[rN,#8] / asrs r3,#20` and the same at +16 are the
 * 16.16-to-tile idiom (>>16 to integers, >>4 for the 16-pixel grid), so the
 * guards are "is this actor standing on tile (35, 23)".  Slot 11 drives event
 * flag 0x303 and slot 12 flag 0x304: on the tile the flag is set, off it the
 * flag is cleared.
 *
 * Imports resolved with tools/overlay_call_targets.ts (an overlay `bl` stores
 * `target_offset - 2`).  The Func_0808a018/Func_0808a020 bracket and the
 * Func_0808a010/Func_080f9010/Func_0808a110/Func_080091c0 body are the same
 * shape the tracked byte-exact source assets/code/resource_398_c_02000280.c
 * uses under its printed names.
 *
 * Twenty-six call sites against the row's advertised 24; the manifest's
 * `calls` field is a floor.  Per-target: Func_0808a080 2, Func_080770c0 4,
 * Func_080770c8 3, Func_080770d0 3, Func_0808a018 2, Func_0808a010 2,
 * Func_080f9010 2, Func_0808a110 2, Func_080091c0 4, Func_0808a020 2.
 *
 * The `||` below is the faithful spelling of the `bne` at 0x02000360: flag
 * 0x304 is only queried when 0x303 reads clear, which is exactly C's
 * short-circuit, and it keeps Func_080770c0 at four sites rather than five.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_080091c0();  /* place a fixture: (x, z, w, h, sx, sz) */
s32 Func_080770c0();   /* read event flag */
void Func_080770c8();  /* set event flag */
void Func_080770d0();  /* clear event flag */
void Func_0808a018();  /* begin scene */
void Func_0808a020();  /* end scene */
void Func_0808a010();  /* wait / delay by count */
u8 *Func_0808a080();   /* actor record by slot id */
void Func_0808a110();  /* set actor pose (slot, pose) */
void Func_080f9010();  /* present message by id */

void Func_02000304(void)
{
    u8 *first = Func_0808a080(11);
    u8 *second = Func_0808a080(12);

    if ((*(s32 *)(first + 8) >> 20) == 35 && (*(s32 *)(first + 16) >> 20) == 23) {
        Func_080770c8(0x303);
    } else {
        Func_080770d0(0x303);
    }

    if ((*(s32 *)(second + 8) >> 20) == 35 && (*(s32 *)(second + 16) >> 20) == 23) {
        Func_080770c8(0x304);
    } else {
        Func_080770d0(0x304);
    }

    if (Func_080770c0(0x303) != 0 || Func_080770c0(0x304) != 0) {
        if (Func_080770c0(0x302) == 0) {
            Func_0808a018();
            Func_0808a010(0x28);
            Func_080f9010(0xd2);
            Func_0808a110(17, 6);
            Func_080091c0(0, 1, 1, 1, 36, 22);
            Func_080091c0(0, 2, 1, 1, 36, 24);
            Func_0808a020();
        }
        Func_080770c8(0x302);
    } else {
        if (Func_080770c0(0x302) != 0) {
            Func_0808a018();
            Func_0808a010(0x28);
            Func_080f9010(0xdc);
            Func_0808a110(17, 2);
            Func_080091c0(1, 1, 1, 1, 36, 22);
            Func_080091c0(1, 2, 1, 1, 36, 24);
            Func_0808a020();
        }
        Func_080770d0(0x302);
    }
}
