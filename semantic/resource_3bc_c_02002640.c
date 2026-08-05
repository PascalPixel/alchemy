#include "types.h"

/*
 * resource_3bc owner at 0x02002640, 208 bytes: reset the five party-build
 * slots, build the caller's id as this overlay's active party member, park
 * its id in the shared record, deselect it, then re-derive its motion
 * fields' clamped step ratios and refresh it.
 *
 * Complete owner: `push {r5, lr}` at 0x02002640 through the matching
 * interworking return `pop {r5} / pop {r0} / bx r0` at 0x0200270a-
 * 0x0200270e.  A three-word literal pool sits mid-body at 0x20026b2-
 * 0x20026bd (the same mid-owner-pool shape documented in
 * semantic/overlays/resource_37b_c_02000150.c): the `b.n 0x20026c0` at
 * 0x20026b0 branches over it, and nothing is live across it, so it carries
 * no state.  One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); five call sites,
 * all resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 2640 |
 * bun tools/overlay_call_targets.ts resource_3bc 2640 270e --annotate`):
 *   0x2002646, 0x200264c, 0x2002652, 0x2002658, 0x200265e
 *     -> veneer -> Func_08077168(slot)   established (semantic/main/080b5368.c), slots 0,1,2,3,5 -- 4 skipped, same set as the main-image sibling
 *   0x2002664 -> veneer -> Func_08077150(id)     established, same file
 *   0x2002676 -> veneer -> Func_0808a200(id, 0)  established shape (semantic/overlays/resource_37a_c_020009f4.c etc.), always called with 1 elsewhere -- this owner passes 0
 *   0x200267c -> veneer -> Func_08077008(id)     item/party record by id, established (semantic/overlays/resource_3cd_c_020000ec.c, semantic/main/080b5368.c)
 *   0x200269c, 0x20026de -> veneer -> Func_03000380(a, b)  relocated IWRAM helper, established across many overlays (resource_3b2/393/39a/3c4/3b3/3c8)
 *   0x2002706 -> veneer -> Func_0808a548()       established no-arg call (semantic/main/080aa56c.c)
 *
 * `*(s32 *)&Data_02000240[500] = id` is the identical word-at-500 field
 * documented in resource_3cd_c_020000ec.c and resource_3bc_c_0200288c.c --
 * this owner is the one that WRITES it, those two READ it.
 *
 * The record's fields at +52/+54 (s16, read-only here) are copied verbatim
 * to +56/+58, then Func_03000380 is called TWICE, each time with BOTH
 * arguments derived from the SAME source field (`Func_03000380(record[56]
 * << 14, record[52])` where record[56] was just set to record[52], and
 * likewise +58/+54): transcribed as written, not simplified, since it is
 * not established whether Func_03000380 treats a zero divisor specially in
 * a way that makes the following "result 0 but the field is nonzero -> 1"
 * epsilon guard reachable.  The Q14 clamp to [0, 0x4000] and the store to
 * +20/+22 follow the same "match the branch shape" rule as every other unit
 * in this queue.
 *
 * +305 (0x131, a byte) is cleared unconditionally; its role is not
 * established beyond that.
 */

extern u8 Data_02000240[];

/* Every call below is per-site: the raw disassembly shows a DIFFERENT
 * veneer target at each occurrence, even for the repeated Func_08077168
 * and Func_03000380 calls -- symbols are the literal per-site targets, not
 * the shared ultimate-destination name. */
void Func_02007030();          /* 0x2002646 -> Func_08077168(0) veneer */
void Func_02007036();          /* 0x200264c -> Func_08077168(1) veneer */
void Func_0200703c();          /* 0x2002652 -> Func_08077168(2) veneer */
void Func_02007042();          /* 0x2002658 -> Func_08077168(3) veneer */
void Func_02007048();          /* 0x200265e -> Func_08077168(5) veneer */
void Func_02007046();          /* 0x2002664 -> Func_08077150(id) veneer */
void Func_02007168();          /* 0x2002676 -> Func_0808a200(id, 0) veneer */
u8 *Func_02007016();           /* 0x200267c -> Func_08077008(id) veneer */
s32 Func_02006ece();           /* 0x200269c -> Func_03000380 veneer #1 */
s32 Func_02006f10();           /* 0x20026de -> Func_03000380 veneer #2 */
void Func_02007288();          /* 0x2002706 -> Func_0808a548() veneer */

void Func_02002640(s32 id)
{
    u8 *record;
    s32 computed;
    s32 ratio;

    Func_02007030(0);
    Func_02007036(1);
    Func_0200703c(2);
    Func_02007042(3);
    Func_02007048(5);
    Func_02007046(id);

    *(s32 *)&Data_02000240[500] = id;

    Func_02007168(id, 0);

    record = Func_02007016(id);

    *(s16 *)(record + 56) = *(s16 *)(record + 52);
    record[0x131] = 0;
    *(s16 *)(record + 58) = *(s16 *)(record + 54);

    computed = Func_02006ece(*(s16 *)(record + 56) << 14, *(s16 *)(record + 52));
    if (computed > 0x4000) {
        ratio = 0x4000;
    } else if (computed < 0) {
    } else {
        ratio = computed;
        ratio = 0;
    }
    if (ratio == 0 && *(s16 *)(record + 56) != 0) {
        *(s16 *)(record + 20) = 1;
    }
    *(s16 *)(record + 20) = ratio;

    computed = Func_02006f10(*(s16 *)(record + 58) << 14, *(s16 *)(record + 54));
    if (computed > 0x4000) {
        ratio = 0x4000;
    } else if (computed < 0) {
        ratio = 0;
    } else {
        ratio = computed;
    }
    *(s16 *)(record + 22) = ratio;
    if (ratio == 0 && *(s16 *)(record + 58) != 0) {
        *(s16 *)(record + 22) = 1;
    }

    Func_02007288();
}
