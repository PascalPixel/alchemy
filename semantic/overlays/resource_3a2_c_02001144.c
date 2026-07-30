typedef unsigned char u8;

/*
 * resource_3a2 owner at 0x02001144, 60 bytes.  Complete owner: `push {lr}` /
 * `sub sp,#8` prologue at 0x02001144 and the interworking return
 * `add sp,#8 / pop {r0} / bx r0` at 0x02001176.  r0 holds the popped return
 * address, so the owner returns nothing and takes no arguments.
 *
 * A four-byte literal pool sits at 0x0200117c, after the epilogue and inside
 * the row's 60-byte span: 0x00000201.  It is never reached as an instruction
 * (the control-flow walk from the prologue is straight-line and stops at the
 * `bx r0`), so it is data, not the `lsls r1, r0, #8` the disassembler prints.
 * It is the argument of the closing Func_080770c8 cue.
 *
 * Calls resolved with `bun tools/overlay_call_targets.ts resource_3a2 1144
 * --json` (five sites, five distinct veneer targets; the inventory's
 * `calls=4` is a known lower bound):
 *   0x1148 -> veneer 0x14f0 -> Func_0808a018
 *   0x115c -> veneer 0x14b8 -> Func_080091c0
 *   0x1162 -> veneer 0x1508 -> Func_0808a080
 *   0x116e -> veneer 0x14d8 -> Func_080770c8
 *   0x1172 -> veneer 0x14f8 -> Func_0808a020
 *
 * Func_0808a018/Func_0808a020 bracket the beat (the same pair the byte-exact
 * assets/code/resource_3a2_c_0200084c.c opens and closes with).  Func_080091c0
 * is the six-argument collision/tile repaint: four register arguments plus two
 * words at [sp,#0] and [sp,#4].  Func_0808a080 is the established slot-record
 * accessor -- it takes a selector and returns the record pointer -- and the
 * byte written here lands at record + 89.
 *
 * Uncertainties: no register is set before Func_0808a018, so it is spelled with
 * no arguments.  The record field at +89 is written as a byte with the value
 * 254; whether the field is signed (-2) is not determined by the store.
 * Old-style declarations keep every import's interface open.
 */

void Func_0808a018();
void Func_080091c0();
u8 *Func_0808a080();
void Func_080770c8();
void Func_0808a020();

void Func_02001144(void)
{
    Func_0808a018();
    Func_080091c0(14, 6, 1, 2, 21, 11);
    Func_0808a080(15)[89] = 254;
    Func_080770c8(0x201);
    Func_0808a020();
}
