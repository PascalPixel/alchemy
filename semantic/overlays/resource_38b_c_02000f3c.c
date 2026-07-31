typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 38b effect spawner at 0x02000f3c (100 bytes, 3 distinct callees).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000f3c and the interworking
 * return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000f96.  The popped r0
 * is the return address, so the owner is `void`.  0x02000f9c-0x02000f9f is
 * the literal pool.
 *
 * The single parameter is r0, live on entry and read at +0x08, +0x0c and
 * +0x10 before anything writes it: the caller's actor record.  The only call
 * site, 0x02000f08 inside 0x02000ed8, does indeed still hold the record
 * returned by Func_0808a080 in r0 at that point, which confirms the arity.
 *
 * Link base 0x02008000, so the pool word 0x02009d7c is in-image data at file
 * offset 0x1d7c — an animation script handed to Func_08009098, not a RAM
 * global.  Overlay image data is writable EWRAM and is not const.
 *
 * All three callees are import veneers resolved with
 * tools/overlay_call_targets.ts.
 */

/* Spawner: Func_080090c8(kind, x, y, z) returns the new record, or NULL. */
extern u8 *Func_080090c8();

/* Attaches the animation script at its second argument to a record. */
extern void Func_08009098();

/* Mode/state setter on a sub-record. */
extern void Func_08009020();

/* Animation script in the overlay's own image at file offset 0x1d7c. */
extern u8 Data_02009d7c[];

void Func_02000f3c(u8 *actor)
{
    u8 *spawned;
    u8 *child;

    /* Kind 24, positioned at the source actor's 16.16 x / y / z. */
    spawned = Func_080090c8(24,
                            *(s32 *)(actor + 0x08),
                            *(s32 *)(actor + 0x0c),
                            *(s32 *)(actor + 0x10));
    if (spawned == 0) {
        return;
    }

    /* The child pointer is read from the fresh record *before* the script is
     * attached, and r0 still holds `spawned` at the call — Func_08009098
     * takes the record and the script. */
    child = *(u8 **)(spawned + 0x50);
    Func_08009098(spawned, Data_02009d7c);

    *(u8 *)(spawned + 0x55) = 0;
    *(u8 *)(spawned + 0x22) = 1;
    *(u8 *)(spawned + 0x23) = 2;

    if (child == 0) {
        return;
    }

    Func_08009020(child, 2);
    *(u8 *)(child + 0x26) = 0;

    /* `movs r3,#13 / negs r3 / ands` masks with ~0x0d, not ~0x0c: bit 0 is
     * cleared as well as bits 2 and 3, then bit 2 is set. */
    *(u8 *)(child + 5) = (u8)((*(u8 *)(child + 5) & ~0x0d) | 0x04);
    *(u8 *)(child + 9) = (u8)(*(u8 *)(child + 9) | 0x0c);
}
