#include "types.h"

/*
 * Resource 381, ten-entity set piece at 0x020029a4
 * (508 bytes, 28 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7,sl / mov r6,r9 / mov r5,r8 / push {r5,r6,r7}` at 0x020029a4, with
 * the matching unwind at 0x02002b6c ending `pop {r0} / bx r0`.  The popped
 * branch register IS r0, so it holds the return address and the owner is
 * `void`.  A two-byte zero alignment word at 0x02002b7a is followed by the
 * nine-word literal pool 0x02002b7c-0x02002b9f, past the final `bx`.
 *
 * Pool words, classified by the link band (in-image is 0x02008000-0x0200bfff,
 * offset = word - 0x8000) and then by parity:
 *   0x004039d2 / 0x004049d2 / 0x00404a4e / 0x00403a52 — BELOW the band, so
 *     they are packed argument words for Func_0808a330, not addresses.  This
 *     is the documented 0x02000240 trap in its general form: check the band
 *     before deciding what a constant is.
 *   0x02c70000 — ABOVE the band; a 16.16 x-offset of 711.0.
 *   0x0200b684 (EVEN) -> in-image table at 0x3684, 8 bytes per entry.
 *   0x0200bb40 and 0x0200bb10 (EVEN) -> in-image arrays at 0x3b40 / 0x3b10.
 *     The byte-exact sibling `assets/code/resource_381_c_02002ba0.c` already
 *     declares exactly these two as `s32 Data_0200bb40[]` and
 *     `Ent *Data_0200bb10[]` — an independent confirmation of both the link
 *     base and the layout, taken from tracked material rather than inferred.
 *   0x0200aba1 (ODD) -> Thumb entry at 0x2ba0, i.e. Func_02002ba0 — that same
 *     byte-exact sibling.  It is the per-frame task this owner installs with
 *     Func_080000d0 and removes with Func_080000d8, and it is what advances
 *     the Data_0200bb40 phase counters that the spin below waits on.
 *
 * SHARED CALL SITE, spelled once.  The `Func_0808a330` at 0x020029de is
 * reached from THREE arms of the entry decision tree (variant 0, 1 and 2),
 * each of which only leaves a different word in r0; variant 3 has its own site
 * at 0x020029e8.  There are exactly three Func_0808a330 sites in the assembly
 * — these two plus the closing one — so writing the call per arm would inflate
 * the multiset to five.  The argument is hoisted into a local instead.  The
 * second decision tree, inside the spawn loop, has the same shape and is
 * handled the same way.
 *
 * The spin at 0x02002b28 is a real wait, not a loop over the array: it
 * rescans all ten Data_0200bb40 counters and waits one frame for as long as
 * any of them is non-zero.  The marker value 222 << 2 = 888 is only a sentinel
 * that cannot collide with the index range 0..9; there is ONE Func_080000c0
 * site inside it.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; all 28 sites are
 * import veneers.  Per-target multiset, reproduced exactly by the C below:
 *   Func_080000c0 x14, Func_0808a330 x3, Func_0808a348 x2, Func_0808a1e8 x2,
 *   Func_080f9010 x2, and one each of Func_080000d0, Func_080000d8,
 *   Func_08009080, Func_080090c8, Func_0808a088.
 * Of the fourteen Func_080000c0 sites, one is in the spawn loop, one is the
 * 20-frame settle, TEN are the individual release beats (they are ten
 * consecutive `str` + wait pairs at 0x02002ac2-0x02002b25, not a loop — the
 * offsets are literal 0/4/8/…/36), one is in the spin and one is the 40-frame
 * close.  The inventory row advertises calls=27, one short; the field is a
 * FLOOR and is not used as an equality test here.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_080000c0();   /* wait n frames */
void Func_080000d0();   /* install a per-frame task (callback, period) */
void Func_080000d8();   /* remove a per-frame task */
void Func_08009080();   /* commit an entity's presentation */
u8 *Func_080090c8();    /* spawn an entity at a 16.16 position */
void Func_0808a088();   /* clear a scene slot */
void Func_0808a1e8();   /* play animation id for n frames */
void Func_0808a330();   /* screen transition request */
void Func_0808a348();   /* wait for a screen transition */
void Func_080f9010();   /* play a cue by id */

void Func_02002ba0(void);   /* this overlay, byte-exact in assets/code */

extern s32 Data_0200b684[];     /* ten 8-byte spawn positions (x, y) */
extern s32 Data_0200bb40[];     /* per-entity phase counter, driven by 0x02002ba0 */
extern u8 *Data_0200bb10[];     /* per-entity record */

void Func_020029a4(u32 variant)
{
    s32 *placement;
    u8 *entity;
    u8 *sub;
    s32 transition;
    s32 x;
    s32 y;
    s32 z;
    u32 i;
    u32 busy;

    i = 0;
    do {
        Func_0808a088(i + 16);
        i++;
    } while (i <= 15);

    if (variant == 1) {
        transition = 0x004049d2;
    } else if (variant < 1) {
        transition = 0x004039d2;
    } else if (variant == 2) {
        transition = 0x00404a4e;
    } else if (variant == 3) {
        Func_0808a330(0x00403a52, 1);
        goto opened;
    } else {
        goto opened;
    }
    Func_0808a330(transition, 1);

opened:
    Func_0808a348(60);
    Func_080f9010(214);

    placement = Data_0200b684;
    i = 0;
    do {
        x = placement[0];
        y = placement[1];
        z = 0;
        if (variant == 1) {
            x += 0xe80000;
            z = 232 << 17;
        } else if (variant < 1) {
            x += 0xe80000;
            z = 0x900000;
        } else if (variant == 2) {
            x += 0x02c70000;
            z = 0x900000;
        } else if (variant == 3) {
            x += 0x02c70000;
            z = 232 << 17;
        }

        Data_0200bb40[i] = 0;

        entity = Func_080090c8(284, x, y, z);
        Data_0200bb10[i] = entity;
        entity[85] = 0;

        sub = *(u8 **)(entity + 0x50);
        sub[38] = 0;
        sub[9] = (u8)((sub[9] & ~13) | 4);

        Func_08009080(entity, 6);
        Func_080000c0(6);

        i++;
        placement += 2;
    } while (i <= 9);

    if (variant == 0) {
        Func_0808a1e8(0, 256, 0);
        Func_0808a1e8(1, 256, 0);
    }

    Func_080000c0(20);

    Func_080000d0(Func_02002ba0, 3200);
    Func_080f9010(246);

    /* Ten separate release beats, six frames apart. */
    Data_0200bb40[0] = 1;
    Func_080000c0(6);
    Data_0200bb40[1] = 1;
    Func_080000c0(6);
    Data_0200bb40[2] = 1;
    Func_080000c0(6);
    Data_0200bb40[3] = 1;
    Func_080000c0(6);
    Data_0200bb40[4] = 1;
    Func_080000c0(6);
    Data_0200bb40[5] = 1;
    Func_080000c0(6);
    Data_0200bb40[6] = 1;
    Func_080000c0(6);
    Data_0200bb40[7] = 1;
    Func_080000c0(6);
    Data_0200bb40[8] = 1;
    Func_080000c0(6);
    Data_0200bb40[9] = 1;
    Func_080000c0(6);

    /* Wait for every counter to run down; 0x02002ba0 clears them. */
    for (;;) {
        busy = 0;
        for (i = 0; i <= 9; i++) {
            if (Data_0200bb40[i] != 0) {
                busy = 1;
                break;
            }
        }
        if (busy == 0) {
            break;
        }
        Func_080000c0(1);
    }

    Func_080000c0(40);
    Func_080000d8(Func_02002ba0);

    Func_0808a330(0x10000, 1);
    Func_0808a348(40);
}
