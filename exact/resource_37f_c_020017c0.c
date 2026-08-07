#include "types.h"

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
 * All 42 call sites were resolved with `tools/lib/overlay_call_targets.ts`, which
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





extern void Func_0200333c();
extern void Func_0200334e();
extern void Func_02003360();
extern void Func_02003372();
extern void Func_02003384();
extern void Func_02003396();
extern s32 Func_020033c4();
extern void Func_020033b0();
extern void Func_0200345a();
extern s32 Func_020033f4();
extern void Func_020033e0();
extern void Func_0200348a();
extern s32 Func_02003420();
extern void Func_02003410();
extern void Func_020034ba();
extern s32 Func_02003452();
extern void Func_02003442();
extern void Func_020034ec();
extern s32 Func_02003482();
extern void Func_02003472();
extern void Func_0200351c();
extern s32 Func_020034b6();
extern void Func_020034a6();
extern void Func_02003550();
extern s32 Func_020034e6();
extern void Func_020034d6();
extern void Func_02003580();
extern s32 Func_02003518();
extern void Func_02003508();
extern void Func_020035b2();
extern s32 Func_02003548();
extern void Func_02003538();
extern void Func_020035e2();
extern s32 Func_0200357c();
extern void Func_0200356c();
extern void Func_02003616();
extern s32 Func_020035ac();
extern void Func_0200359c();
extern void Func_02003646();
extern s32 Func_020035de();
extern void Func_020035ce();
extern void Func_02003678();
void Func_020017c0(s32 tint)
{
    { s32 f1 = 100; s32 g1 = 32; Func_0200333c(122, 20, 1, 1,  f1, g1); }
    { s32 f2 = 104; s32 g2 = 32; Func_0200334e(122, 20, 1, 1,  f2, g2); }
    { s32 f3 = 108; s32 g3 = 32; Func_02003360(122, 20, 1, 1,  f3, g3); }
    { s32 f4 = 112; s32 g4 = 32; Func_02003372(122, 20, 1, 1,  f4, g4); }
    { s32 f5 = 116; s32 g5 = 32; Func_02003384(122, 20, 1, 1,  f5, g5); }
    { s32 f6 = 120; s32 g6 = 32; Func_02003396(122, 20, 1, 1,  f6, g6); }

    if (Func_020033c4(0x311) != 0) {
        { s32 f7 = 100; s32 g7 = 32; Func_020033b0(121, 20, 1, 1,  f7, g7); }
        if (tint != 0) {
            Func_0200345a(9, 199 << 19, 130 << 18);
        }
    } else if (Func_020033f4(0x310) != 0) {
        { s32 f8 = 100; s32 g8 = 32; Func_020033e0(121, 20, 1, 1,  f8, g8); }
        if (tint != 0) {
            Func_0200348a(9, 203 << 19, 130 << 18);
        }
    }

    if (Func_02003420(0x313) != 0) {
        { s32 f9 = 104; s32 g9 = 32; Func_02003410(121, 20, 1, 1,  f9, g9); }
        if (tint != 0) {
            Func_020034ba(10, 207 << 19, 130 << 18);
        }
    } else if (Func_02003452(0x312) != 0) {
        { s32 f10 = 104; s32 g10 = 32; Func_02003442(121, 20, 1, 1,  f10, g10); }
        if (tint != 0) {
            Func_020034ec(10, 211 << 19, 130 << 18);
        }
    }

    if (Func_02003482(0x315) != 0) {
        { s32 f11 = 108; s32 g11 = 32; Func_02003472(121, 20, 1, 1,  f11, g11); }
        if (tint != 0) {
            Func_0200351c(11, 215 << 19, 130 << 18);
        }
    } else if (Func_020034b6(0x314) != 0) {
        { s32 f12 = 108; s32 g12 = 32; Func_020034a6(121, 20, 1, 1,  f12, g12); }
        if (tint != 0) {
            Func_02003550(11, 219 << 19, 130 << 18);
        }
    }

    if (Func_020034e6(0x317) != 0) {
        { s32 f13 = 112; s32 g13 = 32; Func_020034d6(121, 20, 1, 1,  f13, g13); }
        if (tint != 0) {
            Func_02003580(12, 223 << 19, 130 << 18);
        }
    } else if (Func_02003518(0x316) != 0) {
        { s32 f14 = 112; s32 g14 = 32; Func_02003508(121, 20, 1, 1,  f14, g14); }
        if (tint != 0) {
            Func_020035b2(12, 227 << 19, 130 << 18);
        }
    }

    if (Func_02003548(0x319) != 0) {
        { s32 f15 = 116; s32 g15 = 32; Func_02003538(121, 20, 1, 1,  f15, g15); }
        if (tint != 0) {
            Func_020035e2(13, 231 << 19, 130 << 18);
        }
    } else if (Func_0200357c(0x318) != 0) {
        { s32 f16 = 116; s32 g16 = 32; Func_0200356c(121, 20, 1, 1,  f16, g16); }
        if (tint != 0) {
            Func_02003616(13, 235 << 19, 130 << 18);
        }
    }

    if (Func_020035ac(0x31b) != 0) {
        { s32 f17 = 120; s32 g17 = 32; Func_0200359c(121, 20, 1, 1,  f17, g17); }
        if (tint != 0) {
            Func_02003646(14, 239 << 19, 130 << 18);
        }
    } else if (Func_020035de(0x31a) != 0) {
        { s32 f18 = 120; s32 g18 = 32; Func_020035ce(121, 20, 1, 1,  f18, g18); }
        if (tint != 0) {
            Func_02003678(14, 243 << 19, 130 << 18);
        }
    }
}
