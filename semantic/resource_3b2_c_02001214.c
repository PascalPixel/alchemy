typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x02001214, 160 bytes: code 0x02001214-0x0200129d,
 * a two-byte alignment `movs r0, r0` at 0x0200129e, and the five-word literal
 * pool 0x020012a0-0x020012b3.
 *
 * Complete owner: `push {r5, r6, lr}` prologue, an 8-byte outgoing-argument
 * frame, and the single epilogue at 0x02001296 ending `pop {r0} / bx r0` — so
 * the owner is **void**.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 1214 --json` (8 sites, 6 distinct targets, all import veneers):
 *   0x0200121a -> 0x02003088 -> Func_0808a080
 *   0x02001234 -> 0x02003058 -> Func_080770c0
 *   0x02001246 -> 0x020030b8 -> Func_0808a0f0
 *   0x02001250 -> 0x02003088 -> Func_0808a080
 *   0x02001256 -> 0x02003038 -> Func_080091e0
 *   0x0200125e -> 0x020030d0 -> Func_0808a1e0
 *   0x02001286 -> 0x02003028 -> Func_080091c0
 *   0x0200128e -> 0x02003088 -> Func_0808a080
 *
 * Behaviour: a scene beat for slot 8.  It looks up a companion entity whose
 * selector is built from the scene table (`Data_02000240[224] + 2132`, the
 * 2132 being the encoded difference 0x8d2 - 0x7e of two pool words).  When the
 * companion is present the slot is cued to (650, 168), lifted to y = -32,
 * given presentation mode 0, marked, and a collision rectangle is repainted;
 * when it is absent the slot's byte at +85 is simply cleared.
 *
 * UNCERTAINTY: the two pool words 0x0000007e and 0x000008d2 are only ever used
 * as the difference 2132, so which of them is the table's base id and which the
 * beat's own id is not established; they are written as the subtraction the
 * code performs.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
u8 *Func_080770c0();                    /* companion entity by selector, or 0 */
void Func_0808a0f0();                   /* cue a slot to (x, z) in 16.16 */
void Func_080091e0();                   /* set presentation mode */
void Func_0808a1e0();                   /* slot state notification */
void Func_080091c0();                   /* collision repaint, six arguments */

void Func_02001214(void)
{
    u8 *entity;
    u8 *companion;

    entity = Func_0808a080(8);
    companion = Func_080770c0(Data_02000240[224] + (0x8d2 - 0x7e));

    if (companion == 0) {
        goto absent;
    }

    Func_0808a0f0(8, 0x028a0000, 0x00a80000);   /* (650.0, 168.0) */
    *(s32 *)(entity + 12) = (s32)0xffe00000;    /* y = -32.0 in 16.16 */

    Func_080091e0(Func_0808a080(8), 0);
    Func_0808a1e0(8, 3);

    entity[85] = 0;
    entity[35] = (u8)(entity[35] | 2);

    Func_080091c0(42, 10, 1, 1, 40, 10);
    return;

absent:
    /* Slot absent: clear the entity's flag byte and stop. */
    Func_0808a080(8)[85] = 0;
}
