#include "types.h"

/*
 * Resource 38b cutscene at 0x02000404 (372 bytes, 35 call sites over 18
 * distinct callees).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000404 and the interworking
 * return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02000554.  The popped r0
 * is the return address, so the owner is `void`.  0x0200055a is alignment and
 * 0x0200055c-0x02000577 is the literal pool.  The body is straight-line after
 * a single entry guard, so the control-flow walk from the prologue reaches
 * every instruction up to 0x02000558 and nothing else — the pool is data.
 *
 * INVERSE POOL TRAP, live in this listing.  overlay_show.ts reports a "pool
 * word referenced" at 0x020008a0, which is inside a completely different
 * owner (0x02000890).  It is spurious: the real pool word 0x00004ccc at
 * 0x0200056c decodes as `ldr r4,[pc,#816]`, and the tool followed that fake
 * load.  This is exactly why HANDOVER section 0 says the referenced-words
 * listing is NOT authoritative and the pool map must come from control flow.
 * 0x020008a0 is code belonging to 0x02000890 and is untouched by this owner.
 *
 * COMPLETENESS.  35 sites placed against the inventory's `calls=34` — the
 * documented direction, stated as "placed >= row count".  Per target:
 * Func_0808a080 x4, Func_0808a010 x3, Func_0808a1f0 x3, Func_0808a100 x3,
 * Func_0808a0c0 x3, Func_0808a090 x3, Func_0808a0d0 x3, Func_0808a1e0 x2,
 * Func_080000c0 x2, Func_0808a018 x1, Func_0808a138 x1, Func_0808a1b8 x1,
 * Func_0808a468 x1, Func_0808a170 x1, Func_0808a180 x1, Func_0808a470 x1,
 * Func_080770c8 x1, Func_0808a020 x1 = 35, and each appears exactly that many
 * times below.  The three dialogue beats are three separate runs of `bl`s,
 * not one shared site reached three times, so writing them out does not
 * inflate the multiset.  All 35 targets are import veneers, resolved with
 * tools/overlay_call_targets.ts.
 *
 * REGISTER NOTE.  r5 holds the slot-11 record only up to 0x02000474, where
 * `movs r5, #128 / lsls r5, #9` reuses it as the constant 0x10000.  The
 * record's +0x55 store at 0x02000468 uses an address computed at 0x02000460,
 * before the clobber, so nothing is lost.  Track r5 per use, not as one
 * variable.
 */

/* Slot accessor. */
extern u8 *Func_0808a080();

/* Imports; old-style, arity open. */
extern void Func_0808a018();
extern void Func_0808a020();
extern void Func_0808a010();
extern void Func_0808a090();
extern void Func_0808a0c0();
extern void Func_0808a0d0();
extern void Func_0808a100();
extern void Func_0808a138();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_0808a1b8();
extern void Func_0808a1e0();
extern void Func_0808a1f0();
extern void Func_0808a468();
extern void Func_0808a470();
extern void Func_080000c0();
extern void Func_080770c8();

void Func_02000404(void)
{
    u8 *player;
    u8 *actor;
    u8 *flags;

    player = Func_0808a080(0);
    actor = Func_0808a080(11);

    /* Entry guard on the actor's tile column; the whole cutscene is skipped
     * unless it is standing on column 6. */
    if ((*(s32 *)(actor + 0x08) >> 20) != 6) {
        return;
    }

    Func_0808a018();
    Func_0808a1e0(11, 1);
    Func_0808a138(0, 2);
    Func_0808a010(20);

    /* 0x3333 / 0x1999 are the approach speeds. */
    Func_0808a090(0, 0x3333, 0x1999);
    Func_0808a090(11, 0x3333, 0x1999);

    /* Clear bit 0 of the player's +0x5a flag byte for the duration. */
    flags = Func_0808a080(0) + 90;
    *flags = (u8)(*flags & 0xfe);

    *(u8 *)(actor + 0x55) = 0;

    /* Beat 1.  The +0x18 field alternates between 0xffff0000 and 0x10000
     * around each portrait/line pair.  129 << 1 = 258. */
    Func_0808a1f0(0, 258);
    Func_0808a100(0, 16);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;
    Func_0808a0c0(11, 111, 196);
    Func_0808a0d0(0, 128, 185);

    /* Beat 2. */
    Func_0808a010(20);
    Func_0808a1f0(0, 258);
    Func_0808a100(0, 16);
    Func_0808a0c0(11, 121, 190);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;
    Func_0808a0d0(0, 141, 189);

    /* Beat 3. */
    Func_0808a010(20);
    Func_0808a1f0(0, 258);
    Func_0808a100(0, 16);
    Func_0808a0c0(11, 132, 186);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;

    /* Restore bit 0 of the player's flag byte. */
    flags = Func_0808a080(0) + 90;
    *flags = (u8)(*flags | 1);

    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 166, 185);
    /* 128 << 8 = 0x8000. */
    Func_0808a1b8(0, 0x8000, 20);
    Func_0808a1e0(11, 2);
    Func_0808a468(0, 11);
    Func_080000c0(10);
    Func_0808a170(0x1774);
    Func_0808a180(11, 0);
    Func_0808a470();
    Func_080000c0(10);

    /* Mark the cutscene seen. */
    Func_080770c8(0x848);
    Func_0808a020();
}
