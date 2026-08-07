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
 * tools/lib/overlay_call_targets.ts.
 *
 * REGISTER NOTE.  r5 holds the slot-11 record only up to 0x02000474, where
 * `movs r5, #128 / lsls r5, #9` reuses it as the constant 0x10000.  The
 * record's +0x55 store at 0x02000468 uses an address computed at 0x02000460,
 * before the clobber, so nothing is lost.  Track r5 per use, not as one
 * variable.
 */

/* Slot accessor. */
extern u8 *Func_020014da();
extern u8 *Func_020014e2();
extern void Func_020014da_b();
extern void Func_02001582();
extern void Func_02001552();
extern void Func_020014e8();
extern void Func_0200151a();
extern void Func_02001524();
extern u8 *Func_02001522();
extern void Func_020015da();
extern void Func_02001584();
extern void Func_02001568();
extern void Func_0200157c();
extern void Func_0200154a();
extern void Func_0200160e();
extern void Func_020015b6();
extern void Func_02001598();
extern void Func_020015ac();
extern void Func_0200157a();
extern void Func_0200163e();
extern void Func_020015e6();
extern void Func_020015c8();
extern u8 *Func_020015c0();
extern void Func_020015dc();
extern void Func_020015f6();
extern void Func_0200166a();
extern void Func_0200167a();
extern void Func_020016ca();
extern void Func_02001548();
extern void Func_02001666();
extern void Func_02001676();
extern void Func_020016ea();
extern void Func_02001560();
extern void Func_020015ee();
extern void Func_02001612();

/* Imports; old-style, arity open. */

                            

                            

                            

                            

                            

void Func_02000404(void)
{
    u8 *player;
    u8 *actor;
    u8 *flags;

    player = Func_020014da(0);
    actor = Func_020014e2(11);

    /* Entry guard on the actor's tile column; the whole cutscene is skipped
     * unless it is standing on column 6. */
    if ((*(s32 *)(actor + 0x08) >> 20) != 6) {
        return;
    }

    Func_020014da_b();
    Func_02001582(11, 1);
    Func_02001552(0, 2);
    Func_020014e8(20);

    /* 0x3333 / 0x1999 are the approach speeds. */
    Func_0200151a(0, 0x3333, 0x1999);
    Func_02001524(11, 0x3333, 0x1999);

    /* Clear bit 0 of the player's +0x5a flag byte for the duration. */
    flags = Func_02001522(0) + 90;
    *flags = (u8)(*flags & 0xfe);

    *(u8 *)(actor + 0x55) = 0;

    /* Beat 1.  The +0x18 field alternates between 0xffff0000 and 0x10000
     * around each portrait/line pair.  129 << 1 = 258. */
    Func_020015da(0, 258);
    Func_02001584(0, 16);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;
    Func_02001568(11, 111, 196);
    Func_0200157c(0, 128, 185);

    /* Beat 2. */
    Func_0200154a(20);
    Func_0200160e(0, 258);
    Func_020015b6(0, 16);
    Func_02001598(11, 121, 190);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;
    Func_020015ac(0, 141, 189);

    /* Beat 3. */
    Func_0200157a(20);
    Func_0200163e(0, 258);
    Func_020015e6(0, 16);
    Func_020015c8(11, 132, 186);
    *(s32 *)(player + 24) = (s32)0xffff0000;
    *(s32 *)(player + 24) = 0x10000;

    /* Restore bit 0 of the player's flag byte. */
    flags = Func_020015c0(0) + 90;
    *flags = (u8)(*flags | 1);

    Func_020015dc(0, 0x9999, 0x4ccc);
    Func_020015f6(0, 166, 185);
    /* 128 << 8 = 0x8000. */
    Func_0200166a(0, 0x8000, 20);
    Func_0200167a(11, 2);
    Func_020016ca(0, 11);
    Func_02001548(10);
    Func_02001666(0x1774);
    Func_02001676(11, 0);
    Func_020016ea();
    Func_02001560(10);

    /* Mark the cutscene seen. */
    Func_020015ee(0x848);
    Func_02001612();
}
