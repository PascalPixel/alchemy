typedef signed int s32;

/*
 * Resource 37f, owner at 0x02000154 (164 bytes of code, prologue
 * `push {r5, r6, lr} / sub sp, #8` at 0x02000154 through the interworking
 * return `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x020001f0, plus a
 * two-word literal pool at 0x020001f8-0x020001ff).  r0 is the popped return
 * address, so the owner is `void`.  Straight-line: no branches at all.
 *
 * All 18 call sites were resolved with `tools/overlay_call_targets.ts` (an
 * overlay `bl` stores target_offset - 2, so the disassembler's annotations are
 * wrong); all 18 are import veneers, and 18 matches the inventory's `calls`
 * field for this row.
 *
 * The two pool words are plain constants, 0x9999 and 0x4ccc.  0x4ccc happens
 * to decode as `ldr r4, [pc, #816]`, which is exactly the pool-word-decodes-
 * as-an-instruction trap; the control-flow walk ends at the `bx r0`, so both
 * words are data.
 *
 * Func_08009180 is the six-argument renderer-family call: r0-r3 plus two
 * stack words, both of which are held in r5 (3) and r6 (2) across the whole
 * owner and re-stored before each of the three calls.  That is why r5/r6 are
 * saved.
 */

void Func_0808a018();
void Func_080f9010();
void Func_08009180();
void Func_080000c0();
void Func_0808a1e0();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a010();
void Func_0808a368();
void Func_0808a370();
void Func_0808a248();
void Func_0808a020();

void Func_02000154(void)
{
    Func_0808a018();
    Func_080f9010(181);

    Func_08009180(16, 28, 21, 3, 3, 2);
    Func_080000c0(10);
    Func_08009180(16, 30, 21, 3, 3, 2);
    Func_080000c0(10);
    Func_08009180(16, 32, 21, 3, 3, 2);
    Func_080000c0(10);

    Func_0808a1e0(0, 2);
    Func_0808a090(0, 0x9999, 0x4ccc);
    Func_0808a0d0(0, 120, 98);
    Func_0808a100(0, 2);
    Func_0808a0e0(0, 0, -8);
    Func_0808a010(10);

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(2);
    Func_0808a020();
}
