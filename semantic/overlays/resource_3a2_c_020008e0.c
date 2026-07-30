typedef unsigned short u16;
typedef unsigned int u32;

/*
 * resource_3a2 owner at 0x020008e0, 68 bytes.  Complete owner: `push {r5, lr}`
 * prologue at 0x020008e0 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000912.  r0 holds the popped return
 * address, so the owner returns nothing; it takes no arguments (r0 is set to 0
 * before the first call).
 *
 * A twelve-word... more precisely a three-word literal pool sits at
 * 0x02000918-0x02000923, after the epilogue and inside the row's 68-byte span.
 * A control-flow walk from the prologue never reaches it, so it is data:
 *   0x02000918 = 0xffff5fff  (-0xa001, the window bias)
 *   0x0200091c = 0x00003ffe  (the window width)
 *   0x02000920 = 0x00001a1e  (a text/cue id)
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 08e0
 * --json` (six sites, six distinct veneer targets):
 *   0x08e4 -> veneer 0x1508 -> Func_0808a080
 *   0x08ea -> veneer 0x14f0 -> Func_0808a018
 *   0x08fa -> veneer 0x1610 -> Func_080b0010
 *   0x0902 -> veneer 0x1580 -> Func_0808a170
 *   0x090a -> veneer 0x1590 -> Func_0808a180
 *   0x090e -> veneer 0x14f8 -> Func_0808a020
 *
 * Behaviour: read the player record's heading halfword (Func_0808a080(0), the
 * established slot-record accessor, field +6), then bracket the beat with
 * Func_0808a018/Func_0808a020.  If the heading lies in the half-quadrant window
 * [0xa001, 0xdfff] the beat is Func_080b0010(15); otherwise the actor is told
 * to say cue 0x1a1e and face/step through Func_0808a180(15, 0), the same
 * (actor, 0) pairing that follows Func_0808a170 in resource_373 and 3aa.
 *
 * The comparison is `cmp r5, r3 / bhi`, i.e. UNSIGNED, on the 32-bit sum of a
 * zero-extended `ldrh` and the negative bias -- so it is the standard wrapped
 * range test, not a signed magnitude test.
 *
 * Uncertainty: the heading load is `ldrh r5, [r0, #6]` on the record returned
 * by Func_0808a080(0); the record pointer is not null-checked here.  No
 * register is set before Func_0808a018, so it is spelled with no arguments.
 * Old-style declarations keep every import's interface open.
 */

u16 *Func_0808a080();
void Func_0808a018();
void Func_080b0010();
void Func_0808a170();
void Func_0808a180();
void Func_0808a020();

void Func_020008e0(void)
{
    u32 heading;

    heading = Func_0808a080(0)[3];      /* record + 6, zero extended */
    Func_0808a018();

    if ((u32)(heading - 0xa001) <= 0x3ffe) {
        Func_080b0010(15);
    } else {
        Func_0808a170(0x1a1e);
        Func_0808a180(15, 0);
    }

    Func_0808a020();
}
