typedef signed int s32;

/*
 * Resource 37f, owner at 0x020017c0 (702 bytes of code, prologue
 * `push {r5, r6, r7, lr} / sub sp, #8` at 0x020017c0 through the single
 * interworking return `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001a7e, plus a nine-word literal pool at 0x02001a88-0x02001aab).  r0 is
 * the popped return address, so the owner is `void`.
 *
 * Pool map from a control-flow walk: the only unreached range in the span is
 * the trailing pool, whose nine words are the flag ids 0x0311, 0x0313, 0x0312,
 * 0x0315, 0x0317, 0x0316, 0x0319, 0x031b, 0x031a.  The three ids that are NOT
 * pooled (0x0310, 0x0314, 0x0318) are built as 196/197/198 << 2 instead, which
 * is why the pool holds nine words rather than twelve.
 *
 * All 42 call sites were resolved with `tools/overlay_call_targets.ts`, which
 * matches the inventory's `calls=42` exactly.  All are import veneers:
 * 12 x Func_080770c0, 18 x Func_080091c0, 12 x Func_0808a0f0.
 *
 * Argument: the incoming r0 is kept in r6 for the whole owner and only ever
 * tested against 0.  It gates the twelve Func_0808a0f0 tint calls; the layout
 * calls happen either way.  Its callers are the six 72-byte siblings
 * 0x020005ec..0x02000754, which all pass 0, and Func_02000a24, which passes 1.
 *
 * The twelve flag ids 0x0310..0x031b are exactly the cue ids that sibling
 * family emits through Func_080770d0/Func_080770c8, so this owner is the
 * per-frame redraw of whatever those cues enabled.
 *
 * r5 (32) and r7 (100) are the two stack-carried arguments of the six header
 * Func_080091c0 calls; the later blocks store their stack words explicitly.
 */

s32 Func_080770c0();
void Func_080091c0();
void Func_0808a0f0();

void Func_020017c0(s32 tint)
{
    Func_080091c0(122, 20, 1, 1, 100, 32);
    Func_080091c0(122, 20, 1, 1, 104, 32);
    Func_080091c0(122, 20, 1, 1, 108, 32);
    Func_080091c0(122, 20, 1, 1, 112, 32);
    Func_080091c0(122, 20, 1, 1, 116, 32);
    Func_080091c0(122, 20, 1, 1, 120, 32);

    if (Func_080770c0(0x311) != 0) {
        Func_080091c0(121, 20, 1, 1, 100, 32);
        if (tint != 0) {
            Func_0808a0f0(9, 199 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x310) != 0) {
        Func_080091c0(121, 20, 1, 1, 100, 32);
        if (tint != 0) {
            Func_0808a0f0(9, 203 << 19, 130 << 18);
        }
    }

    if (Func_080770c0(0x313) != 0) {
        Func_080091c0(121, 20, 1, 1, 104, 32);
        if (tint != 0) {
            Func_0808a0f0(10, 207 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x312) != 0) {
        Func_080091c0(121, 20, 1, 1, 104, 32);
        if (tint != 0) {
            Func_0808a0f0(10, 211 << 19, 130 << 18);
        }
    }

    if (Func_080770c0(0x315) != 0) {
        Func_080091c0(121, 20, 1, 1, 108, 32);
        if (tint != 0) {
            Func_0808a0f0(11, 215 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x314) != 0) {
        Func_080091c0(121, 20, 1, 1, 108, 32);
        if (tint != 0) {
            Func_0808a0f0(11, 219 << 19, 130 << 18);
        }
    }

    if (Func_080770c0(0x317) != 0) {
        Func_080091c0(121, 20, 1, 1, 112, 32);
        if (tint != 0) {
            Func_0808a0f0(12, 223 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x316) != 0) {
        Func_080091c0(121, 20, 1, 1, 112, 32);
        if (tint != 0) {
            Func_0808a0f0(12, 227 << 19, 130 << 18);
        }
    }

    if (Func_080770c0(0x319) != 0) {
        Func_080091c0(121, 20, 1, 1, 116, 32);
        if (tint != 0) {
            Func_0808a0f0(13, 231 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x318) != 0) {
        Func_080091c0(121, 20, 1, 1, 116, 32);
        if (tint != 0) {
            Func_0808a0f0(13, 235 << 19, 130 << 18);
        }
    }

    if (Func_080770c0(0x31b) != 0) {
        Func_080091c0(121, 20, 1, 1, 120, 32);
        if (tint != 0) {
            Func_0808a0f0(14, 239 << 19, 130 << 18);
        }
    } else if (Func_080770c0(0x31a) != 0) {
        Func_080091c0(121, 20, 1, 1, 120, 32);
        if (tint != 0) {
            Func_0808a0f0(14, 243 << 19, 130 << 18);
        }
    }
}
