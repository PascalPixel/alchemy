typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;

/*
 * Resource 3ca teardown helper at 0x0200004c (36 bytes, 0x0200004c-0x0200006f).
 *
 * Complete owner: `push {lr}` prologue at 0x0200004c, interworking return
 * `pop {r0} / bx r0` at 0x02000068.  Because the popped register IS r0, the
 * popped value is the return address and nothing is returned: the owner is
 * `void`.  The last four bytes of the row (0x0200006c) are this owner's only
 * literal pool word, 0x03001ebc, and are data.
 *
 * The one call site was resolved with `tools/overlay_call_targets.ts`; the
 * disassembler's own `bl` annotation is wrong for overlays by construction.
 * 0x0200006c -> 0x0200013b4 veneer -> Func_0808a248 in the main image.
 */

/*
 * 0x03001ebc is an IWRAM pointer cell holding the current scene workspace.
 * The overlay reads it, not writes it; the writer is outside this row.
 */
#define SCENE_WORKSPACE (*(s16 **)0x03001ebc)

/* Main-image import reached through the overlay's veneer table.  Old-style
 * declaration: this name is reached with varying arity elsewhere in the
 * overlay, so the interface is deliberately left open. */
void Func_0808a248();

void Func_0200004c(void)
{
    s16 *workspace;

    /* movs r2,#182 / lsls r2,r2,#1 builds 364; the ldrsh then uses r2 = 0 as
     * its index register, so the load is the signed halfword at +364. */
    workspace = SCENE_WORKSPACE;
    Func_0808a248(*(s16 *)((char *)workspace + 364));

    /* movs r3,#160 / lsls r3,r3,#19 is 0x05000000 — palette RAM entry 0.
     * The backdrop colour is forced to black as the scene is torn down. */
    *(volatile u16 *)0x05000000 = 0;
}
