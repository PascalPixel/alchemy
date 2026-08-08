#include "types.h"

/*
 * resource_3a4 owner at 0x020006dc, 268 bytes: scripted scene beat --
 * lower a scene record by 24.0 world units while a background layer
 * flickers out, with an actor-8 cameo in the middle, ending on the
 * standard fade tail and story flag 0x8fd.
 *
 * TWIN of 0x020007e8 (resource_3a4_c_020007e8.c): both were drafted
 * together and each was verified against its OWN disassembly, per the
 * colour-blend discipline.  Shared body: identical sink loop (24
 * iterations of `-0x10000` on the record's +12 field, four frames
 * each), identical flicker bracket (install 0x020086a1 =
 * Func_020006a0 | 1, zero the counter at 0x0200d238, step it to 100,
 * one extra frame, uninstall), identical Func_080f9010(0x121) sting and
 * `Func_080091f0(-1, -1, 0xe666) / Func_0808a010(30)` fade tail.  Real
 * deltas, confirmed on both sides: this row has NO opening
 * Func_080091b8 panel, DOES run the i==8 actor-8 block inside the loop,
 * closes with Func_080091c0(0, 0, 1, 2, 3, 14) instead of a
 * Func_080091b8 repeat, and sets flag 0x8fd where the twin sets 0x8fe.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020006dc, `sub sp, #8`,
 * through `add sp, #8 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x20007d6-0x20007dc.  The `b.n 0x20007ac` at 0x200078a only hops the
 * literal pool at 0x200078c-0x20007aa; the landing block through the
 * second pool word cluster at 0x20007e0-0x20007e6 is this owner's own
 * tail, not shared code (the twin carries its own inline copy).  No
 * arguments, void.
 *
 * The record base comes from the scene-state pointer at 0x03001e70 plus
 * 0x164 (178 << 1); its +12 field is the same y/height slot this
 * package's record layout keeps at that offset.  The counter at
 * 0x0200d238 sits just past the overlay image end (0x5238 + the
 * 0x8000 link base) -- runtime scratch, not image data.  0x0200bd48 is
 * image offset 0x3d48, inside the 0x3d24-0x50f2 data band: a script
 * blob for Func_0808a098, which settles the "phantom 0x02004xxx
 * cluster" question -- every callee below is a veneer into the main
 * ROM, and the old Func_02004xxx names in the park notes were
 * pc-relative misdecodes (see cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --).
 *
 * All 17 bl sites resolve through the veneer table with the `+2`
 * stored-displacement rule:
 *   0x20006e6, 0x2000782 -> Func_080f9010    sound/sting (id)
 *   0x20006f6, 0x20007b2 -> Func_080091f0    camera zoom triple
 *   0x20006fc, 0x20007b8 -> Func_0808a010    frame wait, established
 *   0x2000714, 0x200075c, 0x2000772 -> Func_080000c0   wait n frames
 *   0x200071e, 0x2000726 -> Func_0808a080    scene-record accessor
 *   0x2000736 -> Func_0808a0f0               place entity (id, x, z)
 *   0x200073e -> Func_0808a098               (selector, script)
 *   0x200074e, 0x200077c -> Func_08000130    per-frame callback hook
 *   0x20007cc -> Func_080091c0               six-argument panel call
 *   0x20007d2 -> Func_080770c8               set a story flag
 */

extern u8 Data_0200bd48[];     /* image offset 0x3d48: script blob */

u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_080f9010();          /* sound/sting, established */
void Func_080091f0();          /* camera zoom triple, established */
void Func_0808a010();          /* frame wait, established */
void Func_080000c0();          /* wait n frames, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_0808a098();          /* (selector, script), established */
void Func_08000130();          /* per-frame callback install/clear */
void Func_080091c0();          /* six-argument panel call */
void Func_080770c8();          /* set a story flag, established */

void Func_020006dc(void)
{
    u8 *record;
    u8 *actor;
    u16 *counter;
    u32 i;

    record = *(u8 **)0x03001e70 + 0x164;

    Func_080f9010(230);
    Func_080091f0(0x20000, 0x20000, 0x10000);   /* 128<<10, 128<<9 */
    Func_0808a010(10);

    for (i = 0; i <= 23; i++) {
        *(s32 *)(record + 12) += -0x10000;      /* pool 0xffff0000 */
        Func_080000c0(4);

        if (i == 8) {
            actor = Func_0808a080(8);
            actor = Func_0808a080(8);
            *(s32 *)(actor + 0x18) = 0x1999;
            *(s32 *)(actor + 0x1c) = 0x1999;
            Func_0808a0f0(8, 152 << 16, 216 << 16);
            Func_0808a098(8, Data_0200bd48);
        }
    }

    Func_08000130(1, 0, 0x020086a1);            /* Func_020006a0 | 1 */

    *counter = 0;
    do {
        Func_080000c0(1);
        *counter += 1;
    } while ((u32)(*counter << 16) <= (u32)(200 << 15));
    counter = (u16 *)0x0200d238;

    Func_080000c0(1);
    Func_08000130(1, 0, 0);
    Func_080f9010(0x121);

    Func_080091f0(-1, -1, 0xe666);              /* 0.9 in 16.16 */
    Func_0808a010(30);
    Func_080091c0(0, 0, 1, 2, 3, 14);
    Func_080770c8(0x8fd);
}
