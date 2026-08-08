#include "types.h"

/*
 * Resource 3c2 dialogue step at 0x0200006c.
 *
 * Owner is complete: `push {r5, r6, lr}` at 0x0200006c and the single
 * interworking epilogue at 0x020000ee.  Two literal pools sit inside the span:
 * 0x02000096-0x0200009b (0x02000096 is alignment, 0x02000098 = 0xffffc000)
 * and 0x020000f4-0x020000ff after the epilogue.  The first ENDS MID-ROW: the
 * `bne.n 0x0200009c` at 0x0200008a lands on the `ldr r0,[pc,#84]` that
 * immediately follows the pool word, which `overlay_show.ts` renders as
 * nonsense ARM because it keeps decoding the pool.
 *
 * Return type: `pop {r5, r6} / pop {r0} / bx r0` -- r0 holds the popped return
 * address, so the owner is void.
 *
 * All twelve call sites resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c2 006c --json`:
 *   0x02000072 -> veneer 0x0b64 -> Func_0808a080
 *   0x02000090 -> veneer 0x0c1c -> Func_080b0008
 *   0x0200009e -> veneer 0x0b34 -> Func_080770c0
 *   0x020000aa -> veneer 0x0bc4 -> Func_0808a170
 *   0x020000b2 -> veneer 0x0bcc -> Func_0808a178
 *   0x020000ba -> veneer 0x0b5c -> Func_0808a070
 *   0x020000c4 -> veneer 0x0b44 -> Func_0808a010
 *   0x020000ca -> veneer 0x0bc4 -> Func_0808a170
 *   0x020000d2 -> veneer 0x0bc4 -> Func_0808a170
 *   0x020000da -> veneer 0x0bd4 -> Func_0808a180
 *   0x020000e2 -> veneer 0x0bc4 -> Func_0808a170
 *   0x020000ea -> veneer 0x0bd4 -> Func_0808a180
 * Per target: Func_0808a170 x4, Func_0808a180 x2, and one each of
 * Func_0808a080, Func_080b0008, Func_080770c0, Func_0808a178, Func_0808a070,
 * Func_0808a010.  Twelve C calls against twelve sites, target by target.
 *
 * What it does: read scene record 0's facing halfword; if the actor is facing
 * the fourth quadrant, hand the interaction to Func_080b0008(31, actor) and
 * stop.  Otherwise pick a message by whether story flag 0x96f is raised, and
 * for the raised case pick between two follow-ups on the result of
 * Func_0808a070(0, 0).
 *
 * Uncertainties.  The facing test is written the way the original computes it:
 * halfword +6 of the record biased by 0x2000, masked with 0xffffc000, then
 * shifted left 16 and compared with 0xc0000000 -- so only bits 14-15 of the
 * biased value are observable and the high half of the mask is dead.  The
 * message ids 0x261c/0x261d/0x261e/0x25cf and the meaning of Func_0808a070's
 * two zero arguments are transcribed, not interpreted.
 */

/* Overlay imports (through the veneer table).  Old-style declarations:
 * overlay imports vary their argument count between call sites. */
u8 *Func_02000bd8();
void Func_02000cae();
s32 Func_02000bd4();
void Func_02000c70();
void Func_02000c80();
s32 Func_02000c18();
void Func_02000c0a();
void Func_02000c90();
void Func_02000c98();
void Func_02000cb0();
void Func_02000ca8();
void Func_02000cc0();

                    

                    

void Func_0200006c(s32 actor)
{
    u8 *record;
    u32 facing;
    s32 message;

    record = Func_02000bd8(0);
    facing = (((u32)*(u16 *)(record + 6) + 0x2000) & 0xffffc000) << 16;

    if (facing == 0xc0000000u) {
        Func_02000cae(31, actor);
        return;
    }

    if (Func_02000bd4(0x96f) != 0) {
        message = 0x261c;
        Func_02000c70(message);
        Func_02000c80(actor, 0);
        if (Func_02000c18(0, 0) == 0) {
            Func_02000c0a(10);
            Func_02000c90(message + 1);
        } else {
            Func_02000c98(message + 2);
        }
        Func_02000cb0(actor, 0);
    } else {
        Func_02000ca8(0x25cf);
        Func_02000cc0(actor, 0);
    }
}
