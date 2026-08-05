#include "types.h"

/*
 * resource_373 owner at 0x02002f14, 672 bytes: code 0x02002f14-0x0200318f and
 * the nine-word literal pool at 0x02003190-0x020031b3.  The next row begins at
 * 0x020031b4, which already has byte-exact C, so the owner is exactly its
 * advertised span.
 *
 * A sibling of the scene at 0x02002cb0 and structurally almost the same: the
 * Func_0808a018 / Func_0808a020 brackets, the same camera placement, the same
 * two record queries around actor selector 1, the same scene-counter bump, the
 * same closing Func_080770c8 cue.  It differs in its actor set (0, 1 and 9
 * rather than 0, 1 and 8) and in the branch polarity of the query at
 * 0x020030cc.
 *
 * Call accounting: 64 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_373 2f14` (21 distinct import
 * veneers, no intra-overlay call, no `call_via` slot).  Each site appears
 * below exactly once.  The disassembler's `bl` annotations are unusable here
 * for the usual reason - an overlay `bl` stores `target_offset - 2` - and this
 * owner shows it plainly: 0x02003088 and 0x02003188 both print 0x0200914e
 * while resolving to Func_0808a1e8 and Func_0808a020.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 is the popped return address and
 * the owner is void.  The prologue is `push {lr}` with no frame; the first
 * branch is taken with no argument register written, so the caller's r0-r3
 * reach Func_0808a018 unchanged and nothing here reads them.  The owner takes
 * no arguments.
 *
 * Import shapes are as recorded in resource_373_c_02002cb0.c and in the
 * resource_39f sources that share this family.  Two readings this owner
 * sharpens:
 *  - Func_0808a210 takes 16.16 values, not addresses: 0x01650000 is 357.0 and
 *    0x02e20000 is 738.0, and 0x02e20000 is not a valid address on this
 *    machine.  The neighbouring Func_0808a0d0 call passes 0x016f (367) and
 *    0x02e9 (745) - the same placement in whole units.
 *  - Func_0808a1e8's second argument is a pooled or built small constant
 *    (0x100, 0x102, 0x103, 0x105) with a frame count third; it behaves as a
 *    keyed animation request.  Not proven beyond the shape.
 *
 * UNCERTAINTY: Func_0808a1b8's third argument is 0 at three sites here and a
 * frame count at the rest, the same immediate-versus-tweened split the sibling
 * owner shows.  Nothing in either owner proves it.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc.
 * The byte-exact assets/code/resource_373_c_02000eb4.c reads the same u16 at
 * +0x1d8 through the same pointer. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
void Func_080770c8();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a210();

void Func_02002f14(void)
{
    u8 *record;

    /* No argument register is written before this branch. */
    Func_0808a018();

    /* 357.0 and 738.0 in 16.16. */
    Func_0808a210(0x1650000, -1, 0x2e20000, 1);

    Func_0808a0d0(0, 0x16f, 0x2e9);
    Func_0808a1b8(0, 0xa000, 0);        /* 160 << 8 */

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a0d0(1, 346, 0x2e9);       /* 173 << 1 */
    Func_0808a1b8(1, 0xd000, 20);       /* 208 << 8 */
    Func_0808a170(0x1c53);
    Func_0808a180(1, 0);
    Func_0808a138(9, 2);
    Func_0808a1e8(9, 0x100, 0);         /* 128 << 1 */
    Func_0808a1b8(9, 0x3000, 10);       /* 192 << 6 */
    Func_0808a1b8(9, 0x5000, 10);       /* 160 << 7 */
    Func_0808a1b8(9, 0x3000, 40);
    Func_0808a188(9, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a138(9, 1);
    Func_0808a1b8(9, 0x5000, 10);
    Func_0808a188(9, 0, 20);
    Func_0808a130(1, 1);
    Func_0808a1e8(1, 0x103, 40);
    Func_0808a188(1, 0, 10);
    Func_0808a110(9, 3);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(0, 0x8000, 0);        /* 128 << 8 */
    Func_0808a1b8(1, 0x1000, 40);       /* 128 << 5 */
    Func_0808a110(9, 4);
    Func_0808a180(9, 0);
    Func_0808a1b8(0, 0xb000, 0);        /* 176 << 8 */
    Func_0808a1b8(1, 0xd000, 10);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a188(1, 0, 10);
    Func_0808a110(9, 3);
    Func_0808a188(9, 0, 20);
    Func_0808a1e8(0, 0x102, 0);         /* 129 << 1 */
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a188(1, 0, 20);
    Func_0808a138(9, 2);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(1, 0x1000, 20);
    Func_0808a178(1, 0);

    /* `cmp r0, #0 / bne` - the counter runs on the nonzero result here, the
     * mirror of the `== 1` test the sibling owner uses. */
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a1e8(1, 0x105, 60);
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    } else {
    }

    Func_0808a188(1, 0, 20);
    Func_0808a1b8(1, 0xd000, 10);
    Func_0808a170(0x1c60);
    Func_0808a180(1, 0);
    Func_0808a110(9, 3);
    Func_0808a188(9, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x1000, 20);
    Func_0808a110(1, 3);
    Func_0808a110(0, 3);
    Func_0808a100(1, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        /* The s16 integer views at +10 and +18 of the 16.16 X and Z. */
        Func_0808a0b8(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_080770c8(772);                 /* 193 << 2 */

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
