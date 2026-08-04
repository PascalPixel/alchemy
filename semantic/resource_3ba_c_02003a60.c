#include "types.h"

/*
 * resource_3ba owner at 0x02003a60, 160 bytes (0x02003a60-0x02003aff):
 * 150 bytes of code, two alignment bytes at 0x02003af6, and the two-word
 * literal pool at 0x02003af8 (0x02000240, 0xfff00000), which ends exactly
 * where the overlay's veneer table starts at 0x02003b00.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, r8 / push {r7} / sub sp, #12` at
 * 0x02003a60; epilogue `add sp, #12 / pop {r3} / mov r8, r3 /
 * pop {r5, r6, r7} / pop {r1} / bx r1` at 0x02003aea.  The interworking return
 * pops into r1, not r0, so r0 survives and IS the result: this owner returns
 * the value of its last Func_020038b0 call.  No argument register is read
 * before being written, so it takes no arguments.
 *
 * VERBATIM TWIN of `semantic/overlays/resource_3bb_c_02003cf8.c` — the same
 * 160-byte body, instruction for instruction, with resource_3bb's own lookup
 * Func_02003b48 replaced by this overlay's Func_020038b0.  Both lookups have
 * byte-exact sources in `assets/code` and are the same routine: one argument,
 * a three-word position; scan slots 8..65 of the workspace at
 * `Data_03001ebc + 0x14` and return the occupant whose x/y/z agree to whole
 * units, or 0.  The cross-check is free and it is what settles the interface.
 *
 * CALL ACCOUNTING.  Five `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_3ba --annotate`: Func_0808a080
 * once, Func_08000128 twice, Func_020038b0 twice.  The inventory's `calls=5`
 * agrees.  The disassembler's own `bl` annotations are unusable, as always on
 * an overlay, because an overlay `bl` stores `target_offset - 2`.
 *
 * The pool word 0x02000240 is below this overlay's 0x02008000 link base (base
 * witnessed by 0x0200804d = Func_0200004c + 1 in the byte-exact sibling
 * `assets/code/resource_3ba_c_02000158.c`), so it is a RAM global — the shared
 * `s16 Data_02000240[]` table that the byte-exact sources in resource_36f and
 * resource_371 already declare.  The index is built as 250 << 1 = 500, i.e.
 * the pointer-sized word `*(void **)&Data_02000240[250]`, exactly as
 * `assets/code/resource_36f_c_02000054.c` reads it.
 *
 * Shape: take the active subject's record, derive its facing as
 * `(record halfword at +6 + 0x2000) & 0xc000` — the biased quadrant, with no
 * sign extension here — then probe one step ahead at 0x100000 and, if nothing
 * occupies that cell, one step further at 0x200000.  Each probe rounds the
 * record's x and z words down to whole units (`& 0xfff00000`) and re-centres
 * them by half a unit (0x80000) while carrying y through unrounded.
 *
 * Uncertainties: only the record fields at +6 (halfword), +8, +12 and +16 are
 * asserted.  Both Func_020038b0 sites also load r1 with the record pointer;
 * the callee's byte-exact source takes a single parameter, so the extra
 * register is not asserted as an argument.  r8 merely caches the 0xfff00000
 * mask across the first call and carries no other value.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
u8 *Func_0808a080();
void Func_08000128();

/* This overlay's own lookup; byte-exact source in assets/code. */
s32 *Func_020038b0(s32 *);

s32 *Func_02003a60(void)
{
    u8 *record;
    s32 facing;
    s32 position[3];
    s32 *occupant;

    record = Func_0808a080(*(s32 *)(0x02000240 + 500));

    /* 128 << 6 = 0x2000 bias, then masked to bits 14-15 (192 << 8). */
    facing = (*(u16 *)(record + 6) + 0x2000) & 0xc000;

    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
    Func_08000128(0x100000, facing, position);          /* 128 << 13 */

    occupant = Func_020038b0(position);
    if (occupant == 0) {
        position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x80000;
        position[1] = *(s32 *)(record + 12);
        position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x80000;
        Func_08000128(0x200000, facing, position);      /* 128 << 14 */

        occupant = Func_020038b0(position);
    }

    return occupant;
}
