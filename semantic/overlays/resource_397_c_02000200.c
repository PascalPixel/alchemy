typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_397 owner at 0x02000200, 106 bytes: the overlay's ROOT.  Publish
 * the scene id, arm the alpha blend this resource is drawn with, and hand off
 * to the scene's own setup.
 *
 * Complete owner: `push {r5, lr}` at 0x02000200 through
 * `pop {r5} / pop {r1} / bx r1` at 0x02000264-0x02000268.  The single
 * `b.n 0x02000264` at 0x02000242 hops the 32-byte literal pool at
 * 0x02000244-0x02000263, which is inside the advertised span and is not code.
 * The epilogue pops the return address into r1, not r0 (HANDOVER section 0,
 * epilogue rule), so r0 survives and IS the result -- and r0 is set to 0
 * immediately before the branch on the owner's only path, so this returns a
 * constant 0 status.  No argument register is read before being written.
 *
 * This owner is entry 0 of the overlay's exported-entry veneer table at image
 * offset 0: the first table word is 0x02008201, which under the proven
 * 0x02008000 link base is 0x0200 + the Thumb bit.  Working root-down, this is
 * therefore where the overlay's call graph starts.
 *
 * Two call sites, matching the inventory's calls=2.  Resolved with
 * `bun tools/overlay_call_targets.ts resource_397 0200`:
 *   0x02000212 -> veneer 0x034c -> Func_0808a300     (import)
 *   0x0200023c -> in-image prologue 0x020002e0       (this overlay's own)
 * The printed annotations (0x02000560, 0x0200051e) are the usual `bl`
 * decoding artefact; the stored displacement is the target's image offset
 * minus 2.  0x020002e0 is a real intra-overlay call -- the byte-exact
 * assets/code/resource_397_c_020002e0.c is already tracked for it -- and it is
 * NOT inside this owner's span, so it is a call and not a `goto`.
 *
 * Pool words.  0x03001ebc is the well-known IWRAM workspace pointer;
 * 0x04000050/0x04000052 are BLDCNT and BLDALPHA (the owner writes 0x04000052
 * as `0x04000050 + 2` from the same register).  Everything else it loads
 * (0x3f42, 0x0c04, 0x0534, 0x3f3f, 0x0536, 0x052a) is a plain constant: all
 * are far below the 0x02008000 band, so none is an in-image address.
 *
 * What it does, in order:
 *  1. `*(s32 *)(*(0x03001ebc) + 448) = 32` -- the scene/phase id slot
 *     documented in HANDOVER section 0, published on entry with a per-scene
 *     constant.  32 is the same constant the 0x02000180 owner publishes, so
 *     this resource shares that scene id.
 *  2. `Func_0808a300(9)`.
 *  3. BLDCNT = 0x3f42, BLDALPHA = 0x0c04.  0x3f42 selects BG1 as the first
 *     blend target, effect 1 (alpha), and all layers plus backdrop as the
 *     second; 0x0c04 is EVA = 4, EVB = 12.
 *  4. Three halfword writes into the display workspace reached through
 *     `*(0x03001ecc)` (the same slot, reached here as `+16` off the
 *     0x03001ebc pointer): +0x534 = 0x3f3f, +0x536 = 31, +0x52a = 10.
 *  5. `Func_020002e0()`, then return 0.
 *
 * Uncertainties.  The three workspace halfwords are written but never read
 * here.  Their values are blend-shaped (0x3f3f is a full layer mask and 31 is
 * the maximum blend weight), so they read as the engine's shadow copies of the
 * window/blend state rather than as unrelated fields, but nothing in this
 * owner proves that.  Field names below say what is stored, not what consumes
 * it.  The meaning of Func_0808a300's argument 9 is likewise not determined
 * here; the sibling owner at 0x0200015c passes the same 9.
 */

/* Overlay import (via the veneer table).  Old-style declaration: an overlay
 * import can be reached with different argument counts at different sites. */
void Func_0808a300();

/* This overlay's own scene setup, one image offset away in the same file. */
void Func_020002e0(void);

#define BLDCNT_02000200   (*(volatile u16 *)0x04000050)
#define BLDALPHA_02000200 (*(volatile u16 *)0x04000052)

s32 Func_02000200(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u8 *display = *(u8 **)((u8 *)0x03001ebc + 16);

    /* Scene/phase id published on entry. */
    *(s32 *)(workspace + 448) = 32;

    Func_0808a300(9);

    BLDCNT_02000200 = 0x3f42;      /* alpha-blend BG1 over every other layer */
    BLDALPHA_02000200 = 0x0c04;    /* EVA = 4, EVB = 12 */

    *(u16 *)(display + 0x534) = 0x3f3f;
    *(u16 *)(display + 0x536) = 31;
    *(u16 *)(display + 0x52a) = 10;

    Func_020002e0();
    return 0;
}
