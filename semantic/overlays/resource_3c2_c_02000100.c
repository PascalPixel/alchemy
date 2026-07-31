typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/*
 * Resource 3c2 dialogue step at 0x02000100.
 *
 * Owner is complete: `push {r5, lr}` at 0x02000100 and the single interworking
 * epilogue at 0x02000158.  Two literal pools sit inside the span:
 * 0x0200012a-0x0200012f (alignment halfword plus 0xffffc000) and
 * 0x0200015e-0x0200016b after the epilogue.  The first ENDS MID-ROW -- the
 * `bne.n 0x02000130` at 0x0200011e lands on the `ldr r0,[pc,#44]` immediately
 * after the pool word, which `overlay_show.ts` prints as nonsense ARM because
 * it keeps decoding the pool.
 *
 * Return type: `pop {r5} / pop {r0} / bx r0` -- r0 holds the popped return
 * address, so the owner is void.
 *
 * This owner and 0x0200016c are near twins (108 and 104 bytes): identical
 * facing test, identical flag test, differing only in the fourth-quadrant
 * handler and in the two message ids.  0x0200006c is the same family with a
 * richer raised-flag branch.
 *
 * All seven call sites resolved with
 * `bun tools/overlay_call_targets.ts resource_3c2 0100 --json`:
 *   0x02000106 -> veneer 0x0b64 -> Func_0808a080
 *   0x02000124 -> veneer 0x0c2c -> Func_080b0018
 *   0x02000132 -> veneer 0x0b34 -> Func_080770c0
 *   0x0200013c -> veneer 0x0bc4 -> Func_0808a170
 *   0x02000144 -> veneer 0x0bd4 -> Func_0808a180
 *   0x0200014c -> veneer 0x0bc4 -> Func_0808a170
 *   0x02000154 -> veneer 0x0bd4 -> Func_0808a180
 * Per target: Func_0808a170 x2, Func_0808a180 x2, and one each of
 * Func_0808a080, Func_080b0018, Func_080770c0.  Seven C calls, target by
 * target, against seven sites.
 *
 * The two arms below are deliberately NOT merged even though they differ only
 * in the message id: they are two distinct call sites of Func_0808a170 and of
 * Func_0808a180 in the original, and folding them would deflate the per-target
 * multiset by two.
 *
 * Uncertainties.  The facing test is transcribed as computed: halfword +6 of
 * scene record 0 biased by 0x2000, masked with 0xffffc000, shifted left 16 and
 * compared with 0xc0000000, so only bits 14-15 of the biased value are
 * observable.  Message ids 0x2620/0x25d1 are not interpreted.
 */

/* Overlay imports (through the veneer table).  Old-style declarations:
 * overlay imports vary their argument count between call sites. */
u8 *Func_0808a080();
void Func_080b0018();
s32 Func_080770c0();
void Func_0808a170();
void Func_0808a180();

void Func_02000100(s32 actor)
{
    u8 *record;
    u32 facing;

    record = Func_0808a080(0);
    facing = (((u32)*(u16 *)(record + 6) + 0x2000) & 0xffffc000) << 16;

    if (facing == 0xc0000000u) {
        Func_080b0018(10, actor);
        return;
    }

    if (Func_080770c0(0x96f) != 0) {
        Func_0808a170(0x2620);
        Func_0808a180(actor, 0);
    } else {
        Func_0808a170(0x25d1);
        Func_0808a180(actor, 0);
    }
}
