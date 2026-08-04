typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;

/*
 * resource_3af owner at 0x02001684, 284 bytes: a story-state-dependent
 * actor arrangement. It hides five actors unconditionally, then picks
 * one of three arrangements from three story flags in the 0x920 family.
 *
 * The opening is five Func_0808a1e0(id, 1) calls for ids 27, 23, 22, 26
 * and 24 -- the same one-argument-plus-mode helper the rest of the
 * corpus uses; mode 1 for all five.
 *
 * Flag 0x920 (built as `movs r0,#146 / lsls r0,#4`, not a pool word)
 * gates the first block: place actor 22 at (0xa20000, 0x029a0000), set
 * the u16 at record +6 to 0x8000, and park actors 23 and 20 at the
 * origin. This block is INDEPENDENT of the two below -- it is a plain
 * `if`, and control falls through to the 0x922 test either way.
 *
 * Flag 0x922 then chooses between two mutually exclusive arrangements
 * (the `beq` at 0x020016f0 jumps to the 0x923 test at 0x02001766, and
 * the taken side ends with `b.n` over it):
 *
 *   - set: actors 21 and 24 both get the billboard treatment first
 *     spelled out in resource_372_c_020031ac.c -- place, set the u16 at
 *     +6, then `*(u16 *)(record + 100) = Func_030003e0(Func_080000f8(),
 *     90) + 60`, then Func_0808a098 with the overlay data pointer
 *     0x0200c4d8 (both actors share it; the pointer is loaded once into
 *     r6 and reused). Actor 21 is placed at (0x01080000, 0x02be0000)
 *     with +6 = 0x5000; actor 24 at (0xf80000, 0x02a80000), keeping the
 *     +6 value 21 already wrote. Finally actor 22 is parked at the
 *     origin.
 *   - clear: if flag 0x923 is set instead, place actor 20 at
 *     (0xf60000, 0x02000000) and clear the u16 at its record +6.
 *
 * A transcription note on that last store: the assembly is
 * `strh r5,[r0,#6]`, and r5 still holds the Func_080770c0(0x922)
 * result -- which is zero on exactly this path, since it is the branch
 * the `beq` took. It is a known-zero register reuse, so the source
 * value is 0, not a live variable.
 *
 * Func_030003e0 is an IWRAM routine reached through a call_via slot,
 * not an overlay import; it is declared old-style here to match the
 * established precedent in resource_372_c_020031ac.c rather than invent
 * a signature.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001684 through
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02001786-0x0200178a; five
 * trailing pool words plus alignment (0x029a0000, 0x00000922,
 * 0x02be0000, 0x0200c4d8, 0x00000923) end at 0x0200179f, and the next
 * owner prologue (`push {r5, r6, lr}`) begins at 0x020017a0.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 1684 17a0, the +2
 * rule against the raw image).
 *
 * Uncertainty: actor ids and coordinate roles by shape; the three flag
 * ids are exact, but which story beat each selects is unknown.
 */

extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a1e0(s32 id, s32 mode);
extern s32 Func_080770c0(s32 flagId);
extern s32 Func_080000f8(void);
s32 Func_030003e0();            /* IWRAM helper via a call_via slot */

void Func_02001684(void)
{
    u8 *record;

    Func_0808a1e0(27, 1);
    Func_0808a1e0(23, 1);
    Func_0808a1e0(22, 1);
    Func_0808a1e0(26, 1);
    Func_0808a1e0(24, 1);

    if (Func_080770c0(0x920) != 0) {
        Func_0808a0f0(22, 162 << 16, 0x029a0000);
        record = Func_0808a080(22);
        *(u16 *)(record + 6) = (u16)(128 << 8);
        Func_0808a0f0(23, 0, 0);
        Func_0808a0f0(20, 0, 0);
    }

    if (Func_080770c0(0x922) != 0) {
        Func_0808a0f0(21, 132 << 17, 0x02be0000);
        record = Func_0808a080(21);
        *(u16 *)(record + 6) = (u16)(160 << 7);
        record = Func_0808a080(21);
        *(u16 *)(record + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
        Func_0808a098(21, 0x0200c4d8);

        Func_0808a0f0(24, 248 << 16, 170 << 18);
        record = Func_0808a080(24);
        *(u16 *)(record + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
        Func_0808a098(24, 0x0200c4d8);

        Func_0808a0f0(22, 0, 0);
    } else if (Func_080770c0(0x923) != 0) {
        Func_0808a0f0(20, 246 << 16, 128 << 18);
        record = Func_0808a080(20);
        *(u16 *)(record + 6) = 0;
    }
}
