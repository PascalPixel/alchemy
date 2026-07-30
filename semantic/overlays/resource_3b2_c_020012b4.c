typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x020012b4, 192 bytes: code 0x020012b4-0x02001365,
 * a two-byte alignment `movs r0, r0` at 0x02001366, and the three-word literal
 * pool 0x02001368-0x02001373.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r5, r6, r7}` (r8/r9/sl), an
 * 8-byte outgoing-argument frame, and the single epilogue at 0x02001356 ending
 * `pop {r0} / bx r0` — so the owner is **void**.  Both guards `b.n` to it.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 12b4 --json` (11 sites, 9 distinct targets):
 *   0x020012c2 -> veneer 0x02003088 -> Func_0808a080
 *   0x020012ea -> veneer 0x02003058 -> Func_080770c0
 *   0x020012fe -> veneer 0x02003068 -> Func_0808a010
 *   0x02001304 -> prologue 0x02000e00 -> Func_02000e00 (this overlay's ring)
 *   0x0200130a -> veneer 0x02003118 -> Func_080f9010
 *   0x02001310 -> veneer 0x02003068 -> Func_0808a010
 *   0x02001316 -> veneer 0x02003088 -> Func_0808a080
 *   0x0200131c -> veneer 0x02003038 -> Func_080091e0
 *   0x02001324 -> veneer 0x020030d0 -> Func_0808a1e0
 *   0x02001346 -> veneer 0x02003028 -> Func_080091c0
 *   0x02001352 -> veneer 0x02003060 -> Func_080770c8
 *
 * Behaviour: the companion-ABSENT counterpart of the beat at 0x02001214, which
 * handles the same slot 8 and the same companion selector
 * (`Data_02000240[224] + 2132`) when the companion IS present.  It fires only
 * while slot 8 stands in tile column 40 and the companion is missing: mark the
 * slot busy, run two scene notifications around this overlay's ring-of-effects
 * at 0x02000e00 and a sound cue, then clear the mark, set the slot's flag bit,
 * repaint the same collision rectangle the 0x02001214 beat repaints, and hand
 * the companion selector to Func_080770c8.
 *
 * The 2132 is the encoded difference of the two pool words 0x8d2 and 0x7e; the
 * same pair appears in 0x02001214's pool with the same use.
 *
 * UNCERTAINTY: as in 0x02001214, which of those two pool words is the table
 * base and which the beat id is not established, so the subtraction is written
 * out as encoded.
 */

extern s16 Data_02000240[];             /* the overlay's scene table */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
u8 *Func_080770c0();                    /* companion entity by selector, or 0 */
void Func_080770c8();                   /* companion notification by selector */
void Func_0808a010();                   /* scene notification */
void Func_080f9010();                   /* play a sound cue */
void Func_080091e0();                   /* set presentation mode */
void Func_0808a1e0();                   /* slot state notification */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's ring-of-effects burst at 0x02000e00. */
void Func_02000e00();

void Func_020012b4(void)
{
    u8 *entity;
    s32 column;
    s32 selector;

    entity = Func_0808a080(8);
    column = *(s32 *)(entity + 8) >> 20;    /* 16.16 -> 16-pixel tile grid */
    if (column != 40) {
        return;
    }

    selector = Data_02000240[224] + (0x8d2 - 0x7e);
    if (Func_080770c0(selector) != 0) {
        return;                             /* handled by 0x02001214 instead */
    }

    entity[85] = 3;

    Func_0808a010(8);
    Func_02000e00(8);
    Func_080f9010(136);
    Func_0808a010(40);

    Func_080091e0(Func_0808a080(8), 0);
    Func_0808a1e0(8, 3);

    entity[85] = 0;
    entity[35] = (u8)(entity[35] | 2);

    Func_080091c0(42, 10, 1, 1, column, 10);

    /* The selector is rebuilt from the same table word, as encoded. */
    Func_080770c8(Data_02000240[224] + (0x8d2 - 0x7e));
}
