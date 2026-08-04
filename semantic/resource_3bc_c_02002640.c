typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

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

void Func_08077168();          /* established, reset slot */
void Func_08077150();          /* established, reset a member by handle */
void Func_0808a200();          /* established shape (id, flag) */
u8 *Func_08077008();           /* item/party record by id, established */
s32 Func_03000380();           /* relocated IWRAM helper, established */
void Func_0808a548();          /* established no-arg call */

void Func_02002640(s32 id)
{
    u8 *record;
    s32 computed;
    s32 ratio;

    Func_08077168(0);
    Func_08077168(1);
    Func_08077168(2);
    Func_08077168(3);
    Func_08077168(5);
    Func_08077150(id);

    *(s32 *)&Data_02000240[500] = id;

    Func_0808a200(id, 0);

    record = Func_08077008(id);

    *(s16 *)(record + 56) = *(s16 *)(record + 52);
    *(s16 *)(record + 58) = *(s16 *)(record + 54);
    record[0x131] = 0;

    computed = Func_03000380(*(s16 *)(record + 56) << 14, *(s16 *)(record + 52));
    if (computed > 0x4000) {
        ratio = 0x4000;
    } else if (computed < 0) {
        ratio = 0;
    } else {
        ratio = computed;
    }
    *(s16 *)(record + 20) = ratio;
    if (ratio == 0 && *(s16 *)(record + 56) != 0) {
        *(s16 *)(record + 20) = 1;
    }

    computed = Func_03000380(*(s16 *)(record + 58) << 14, *(s16 *)(record + 54));
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

    Func_0808a548();
}
