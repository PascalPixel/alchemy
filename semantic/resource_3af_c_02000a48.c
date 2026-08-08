#include "types.h"

/*
 * resource_3af owner at 0x02000a48, 168 bytes: actor 21's interaction
 * callback -- three story-dependent replies chosen from flags 0x925 and
 * 0x922.
 *
 * PUBLISHED, NOT CALLED. Thumb pointer 0x02008a49 at 0x020057ec in the
 * script-record table. Absent from every inventory and from
 * `tools-rs/overlay-unindexed`; see resource_3af_c_02000b98.c for the
 * sweep that finds this class.
 *
 * BYTE-IDENTICAL TWIN of 0x02000af0 apart from the actor id and three
 * dialogue ids -- 21/0x1e08/0x1d6f/0x1d36 here against 24/0x1e09/
 * 0x1d70/0x1d37 there. Same instruction sequence, same branch offsets,
 * same two flags, same shared data pointer 0x0200c4d8. Both were
 * diffed before either was written, which is the habit that made the
 * 0xbb8/0xbf0 pair cheap earlier in this overlay.
 *
 * The three replies:
 *   flag 0x925 set   -> line 0x1e08, then hand the actor back.
 *   flag 0x922 set   -> face the actor (Func_0808a138(id, 2)), line
 *                       0x1d6f, hand back, then reload the u16 at
 *                       record +100 with a scaled random draw and
 *                       reattach the data pointer 0x0200c4d8.
 *   neither          -> Func_0808a1e8(id, 0x103, 0), Func_0808a130(id, 3),
 *                       line 0x1d36, hand back.
 * All three paths converge on Func_0808a020.
 *
 * The random reload is the same scale-by-multiply-and-shift used in
 * 0x020008e0, spelled with a real `muls` here rather than a shift/add
 * chain: `(draw * 90) >> 16` plus 60, i.e. a value in [60, 150). The
 * shift is `lsrs`, so the draw is unsigned.
 *
 * Complete owner: `push {r5, lr}` at 0x02000a48 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02000ace-0x02000ad2; seven trailing pool words
 * (0x00000925, 0x00001e08, 0x00000922, 0x00001d6f, 0x0200c4d8,
 * 0x00000103, 0x00001d36) end at 0x02000aef, and the next owner
 * prologue (its twin, 0x02000af0) begins at 0x02000af0.
 *
 * Uncertainty: the actor id and the four constants are transcribed;
 * flags 0x925 and 0x922 are exact and are two of the six the
 * 0x02001444 driver's tail chain also tests.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_080770c0(s32 flagId);
extern s32 Func_080000f8(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);

void Func_02000a48(void)
{
    u8 *record;
    u32 draw;

    Func_0808a018();
    if (Func_080770c0(0x925) != 0) {
        Func_0808a170(0x1e08);
        Func_0808a180(21, 0);
    } else if (Func_080770c0(0x922) != 0) {
        Func_0808a138(21, 2);
        Func_0808a170(0x1d6f);
        Func_0808a180(21, 0);
        record = Func_0808a080(21);
        draw = ((u32)Func_080000f8() * 90) >> 16;
        draw = (u16)(draw + 60);
        *(u16 *)(record + 100) = (u16)draw;
        Func_0808a098(21, 0x0200c4d8);
    } else {
        Func_0808a1e8(21, 0x103, 0);
        Func_0808a130(21, 3);
        Func_0808a170(0x1d36);
        Func_0808a180(21, 0);
    }
    Func_0808a020();
}
