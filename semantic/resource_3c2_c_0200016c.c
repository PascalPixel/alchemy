#include "types.h"

/*
 * Resource 3c2 dialogue step at 0x0200016c.
 *
 * Owner is complete: `push {r5, lr}` at 0x0200016c and the single interworking
 * epilogue at 0x020001ca.  Two literal pools sit inside the span:
 * 0x020001ae-0x020001bb (alignment, 0xffffc000, 0x96f, 0x262c) and the single
 * word 0x020001d0 after the epilogue.  The first ends mid-row -- the
 * `beq.n 0x020001bc` at 0x0200019c lands on the instruction immediately after
 * it, which `overlay_show.ts` prints as nonsense ARM because it keeps decoding
 * the pool.
 *
 * Return type: `pop {r5} / pop {r0} / bx r0` -- r0 holds the popped return
 * address, so the owner is void.
 *
 * Near twin of 0x02000100 (104 vs 108 bytes): identical facing and flag tests,
 * differing only in the fourth-quadrant handler (Func_080b0010 taking just the
 * actor, against Func_080b0018(10, actor)) and in the two message ids.
 *
 * All seven call sites resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c2 016c --json`:
 *   0x02000172 -> veneer 0x0b64 -> Func_0808a080
 *   0x0200018e -> veneer 0x0c24 -> Func_080b0010
 *   0x02000196 -> veneer 0x0b34 -> Func_080770c0
 *   0x020001a0 -> veneer 0x0bc4 -> Func_0808a170
 *   0x020001a8 -> veneer 0x0bd4 -> Func_0808a180
 *   0x020001be -> veneer 0x0bc4 -> Func_0808a170
 *   0x020001c6 -> veneer 0x0bd4 -> Func_0808a180
 * Per target: Func_0808a170 x2, Func_0808a180 x2, and one each of
 * Func_0808a080, Func_080b0010, Func_080770c0.  Seven C calls, target by
 * target, against seven sites.  The two message arms are distinct call sites
 * and are not merged.
 *
 * Uncertainties.  Same as the twin: the facing computation is transcribed as
 * the original performs it, and the message ids 0x262c/0x25d5 are not
 * interpreted.
 */

/* Overlay imports (through the veneer table).  Old-style declarations:
 * overlay imports vary their argument count between call sites. */
u8 *Func_02000cd8();
void Func_02000db4();
s32 Func_02000ccc();
void Func_02000d66();
void Func_02000d7e();
void Func_02000d84();
void Func_02000d9c();

                    

void Func_0200016c(s32 actor)
{
    u8 *record;
    u32 facing;

    record = Func_02000cd8(0);
    facing = (((u32)*(u16 *)(record + 6) + 0x2000) & 0xffffc000) << 16;

    if (facing == 0xc0000000u) {
        Func_02000db4(actor);
        return;
    }

    if (Func_02000ccc(0x96f) != 0) {
        Func_02000d66(0x262c);
        Func_02000d7e(actor, 0);
    } else {
        Func_02000d84(0x25d5);
        Func_02000d9c(actor, 0);
    }
}
