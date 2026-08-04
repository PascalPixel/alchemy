typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3b2 owner at 0x02002984, 274 bytes: code 0x02002984-0x02002a95 and
 * a two-byte alignment `movs r0, r0` at 0x02002a96.  No literal pool.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + `push {r7}` (saving r8), an 8-byte
 * outgoing-argument frame, and the single epilogue at 0x02002a8a ending
 * `pop {r0} / bx r0` — so the owner is **void**.  Six arms `b.n` to it.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3b2
 * 2984 --json` (12 sites, 4 distinct targets):
 *   0x0200298e, 0x02002998, 0x020029a4, 0x020029ae, 0x020029b8, 0x02002a5e
 *                              -> veneer 0x02003088 -> Func_0808a080
 *   0x020029e8, 0x02002a44, 0x02002a52
 *                              -> prologue 0x02001774 -> Func_02001774
 *   0x02002a58                 -> veneer 0x02002fb8 -> Func_080000c0
 *   0x02002a76, 0x02002a86     -> veneer 0x02003028 -> Func_080091c0
 *
 * The slot-19 return leg (ids negated), and the widest member of the
 * transition family: it consults THREE companions — slots 9, 18 and 17, each by
 * tile row against the single value 15 — and has seven column cases.
 *
 * World coordinates become tile units with an arithmetic `>> 20` (>> 16 to
 * integers, >> 4 for the 16-pixel tile grid).
 *
 * NOTE on call sites: the seven column arms funnel into ONE shared `bl` at
 * 0x02002a44, whose id is whatever the arm left in r1.  Two arms have their own
 * extra sites: column 13's default fires -112 at 0x020029e8 before falling into
 * the shared site with 48, and column 12's last arm fires -144 at 0x02002a52
 * instead of the shared site.  Spelling one call per arm would inject five
 * phantom calls, so the C has exactly three Func_02001774 expressions.
 *
 * NOTE on register lifetime: r5 holds slot 18's tile row through the decision
 * tree and is then REASSIGNED to `slot19Row - 1` for the two repaints.
 *
 * UNCERTAINTY: the second argument of Func_02001774 is an opaque transition
 * magnitude; only its sign convention (negated on this leg) is established.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                    /* scene entity by selector */
void Func_080000c0();                   /* wait N frames */
void Func_080091c0();                   /* collision repaint, six arguments */

/* This overlay's transition starter at 0x02001774. */
void Func_02001774();

void Func_02002984(void)
{
    s32 column;
    s32 row;
    s32 companion17Row;
    s32 companion18Row;
    s32 companion9Row;
    s32 transition;

    column = *(s32 *)(Func_0808a080(19) + 8) >> 20;
    row = *(s32 *)(Func_0808a080(19) + 16) >> 20;
    companion17Row = *(s32 *)(Func_0808a080(17) + 16) >> 20;
    companion18Row = *(s32 *)(Func_0808a080(18) + 16) >> 20;
    companion9Row = *(s32 *)(Func_0808a080(9) + 16) >> 20;

    if (column == 3) {
        return;
    }

    transition = 0;
    if (column == 13) {
        if (companion9Row == 15) {
            transition = 16;
        } else if (companion18Row == 15) {
            transition = 64;
        } else if (companion17Row == 15) {
            transition = 112;
        } else {
            /* This arm runs two transitions back to back. */
            Func_02001774(19, -112, 0);
            transition = 48;
        }
    } else if (column == 6) {
        if (companion17Row == 15) {
            return;
        }
        transition = 48;
    } else if (column == 5) {
        transition = 32;
    } else if (column == 8) {
        if (companion18Row == 15) {
            return;
        }
        transition = (companion17Row == 15) ? 32 : 80;
    } else if (column == 9) {
        if (companion18Row == 15) {
            return;
        }
        transition = (companion17Row == 15) ? 48 : 96;
    } else if (column == 12) {
        if (companion9Row == 15) {
            return;
        }
        if (companion18Row == 15) {
            transition = 48;
        } else if (companion17Row == 15) {
            transition = 96;
        } else {
            /* Its own call site, not the shared one. */
            Func_02001774(19, -144, 0);
        }
    }

    if (transition != 0) {
        Func_02001774(19, -transition, 0);
    }

    Func_080000c0(2);

    row -= 1;
    Func_080091c0(column, row, 1, 3,
                  *(s32 *)(Func_0808a080(19) + 8) >> 20, row);
    Func_080091c0(0, 0, 1, 3, column, row);
}
