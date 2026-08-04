#include "types.h"

/*
 * Resource 3b1 per-object step dispatcher at 0x0200012c.
 *
 * OWNER AND POOL MAP, from a control-flow walk (nothing else settles it —
 * the row's advertised 40 "code" bytes are what the linear walk managed
 * before it hit the jump table).  The owner runs 0x0200012c-0x02000281,
 * the full advertised 342-byte span, and splits as:
 *   0x0200012c-0x0200014f  code (prologue and dispatch)
 *   0x02000150-0x02000183  DATA: the 13-entry `mov pc, r3` jump table
 *   0x02000184-0x0200020f  code (case bodies)
 *   0x02000210-0x02000217  DATA: an interior literal pool, two words
 *                          (0x00000000 and the table base 0x02008150)
 *   0x02000218-0x02000281  code (the remaining case bodies and epilogue)
 * The pool at 0x02000210 ends MID-ROW: the case body at 0x02000218 follows
 * it immediately and is reached only through the jump table, so no branch
 * target marks the pool's end.  Assuming a pool runs to the next label would
 * have swallowed the whole 0x218 case.
 *
 * LINK BASE 0x02008000, proven here twice over.  The dispatcher loads its
 * table base as the pool word 0x02008150 while the table is physically at
 * file offset 0x150, and each of the 13 entries points 0x8000 past the case
 * body it selects.  None of the entries carries the Thumb bit, which is
 * exactly right for `mov pc, rN` — it does not interwork.  (An installed
 * handler pool word would carry it: 0x0200812d elsewhere in this overlay is
 * `Func_0200012c + 1`, this owner plus the Thumb bit.)
 *
 * Signature.  `pop {r5, r6, r7} / pop {r0} / bx r0` puts the saved return
 * address in r0, so nothing is returned: `void`.  One argument, the object.
 *
 * Import naming.  An overlay `bl` stores `target_offset - 2`; resolving each
 * site as `stored_displacement + 2` reaches either the veneer band at
 * 0x02006450+ (named for the main-image address in the veneer's trailing
 * word) or an ordinary prologue in this same overlay.  Two of the three
 * intra-overlay calls here go to 0x020000fc and one to 0x020000d8, both of
 * which already have byte-exact sources in assets/code.
 *
 * Eleven call sites against the manifest's `calls=1`.  The field is derived
 * from the same linear walk that stopped at the jump table, so it is a floor
 * here in the strongest sense; the per-target multiset is the real proof.
 *
 * The 13 table entries select only 9 distinct bodies — indices 1, 3, 6, 8 and
 * 11 all reach 0x02000270.  They are written as grouped `case` labels rather
 * than as copies, and the two shared tails (the counter bump at 0x02000200
 * and the store at 0x0200027a) are reached with `goto` for the same reason:
 * duplicating either would inflate the per-target multiset.
 */

/* Old-style declarations; arity varies by site across this overlay. */
u8  *Func_0808a080();  /* veneer 0x64e8 — actor record accessor */
void Func_08009080();  /* veneer 0x6450 */
void Func_08009150();  /* veneer 0x6470 */

s32  Func_020000fc();
void Func_020000d8();

void Func_0200012c(u8 *object)
{
    u8 *actor;
    u16 *phase;
    s32 next;

    actor = Func_0808a080(8);
    phase = (u16 *) (object + 102);

    switch (*(s16 *) phase) {
    case 0:
        /* 176 << 8. */
        *(u16 *) (object + 6) = 0xb000;
        goto advance_and_clear;

    case 2:
        *(u16 *) (object + 6) = 0;
        goto advance_and_clear;

    case 4:
        Func_08009080(object, 2);
        /* 234 << 17, 128 << 14, 158 << 18. */
        Func_08009150(object, 0x01d40000, 0x00200000, 0x02780000);
        *(s32 *) (object + 76) = 60;
        next = *phase + 1;
        goto store_phase;

    case 5:
        if (Func_020000fc(object) == 0) {
            return;
        }
        Func_08009080(object, 1);
        object[98] = 0;
        if (actor[91] == 0) {
            object[99] = 1;
        }
        next = *phase + 1;
        goto store_phase;

    case 7:
        if (actor[91] == 0) {
            Func_08009080(object, 3);
            object[99] = 2;
        }
        goto advance_and_clear;

    case 9:
        Func_08009080(object, 2);
        /* 240 << 17, 128 << 14, 150 << 18. */
        Func_08009150(object, 0x01e00000, 0x00200000, 0x02580000);
        *(s32 *) (object + 76) = 60;
        *phase = (u16) (*phase + 1);
        if (actor[91] != 0) {
            return;
        }
        object[99] = 3;
        return;

    case 10:
        if (Func_020000fc(object) == 0) {
            return;
        }
        Func_08009080(object, 1);
        object[98] = 0;
        next = *phase + 1;
        goto store_phase;

    case 1:
    case 3:
    case 6:
    case 8:
    case 11:
        Func_020000d8(object);
        return;

    case 12:
        next = 0;
        goto store_phase;

    default:
        /* Any phase above 12 is ignored; the range check is unsigned. */
        return;
    }

advance_and_clear:
    *phase = (u16) (*phase + 1);
    /* The stored 0 comes from the interior pool word at 0x02000210. */
    object[98] = 0;
    return;

store_phase:
    *phase = (u16) next;
}
