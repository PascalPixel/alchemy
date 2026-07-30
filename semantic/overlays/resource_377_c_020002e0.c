typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_377 owner at 0x020002e0, 60 bytes.  Complete owner: 'push {r5, lr}'
 * prologue and the interworking return 'pop {r5} ; pop {r0} ; bx r0'.  r0 holds
 * the popped return address, so the owner returns nothing.
 *
 * Scene teardown/hand-off: if progress flag 0x834 is set, run one extra step,
 * then publish scene id 521 into the shared workspace and hand the incoming
 * object to the closing sequence.
 *
 * Literal pool (offsets 0x34 and 0x38 of the owner, branched past by the
 * epilogue and therefore never executed): 0x00000834 and 0x03001ebc.  The
 * second is the IWRAM cell holding the overlay family's workspace pointer, the
 * same one assets/code/resource_371_c_020004a0.c and
 * assets/code/resource_380_c_02000390.c load; that sibling writes a scene id at
 * the same +0x1C0 slot this owner writes.
 *
 * All five calls were resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 02e0 --json', which reports
 * five sites and five distinct veneer targets:
 *   0x02e6 -> 0x18ec -> Func_080770c0   (flag 0x834 query, result tested)
 *   0x02ee -> 0x1a44 -> Func_0808a2d0
 *   0x0300 -> 0x1a64 -> Func_0808a368
 *   0x0304 -> 0x1a6c -> Func_0808a370
 *   0x030a -> 0x1a34 -> Func_0808a248   (called with the owner's argument)
 * The disassembler's own 'bl' annotations are wrong here, as on every overlay.
 *
 * Arity note: Func_0808a2d0, Func_0808a368 and Func_0808a370 are reached with no
 * register set up between the preceding call and the branch, so they are written
 * with no arguments.  Old-style declarations keep that open, as required.
 *
 * Uncertainty: the store is 'movs r2,#224 ; lsls r2,r2,#1' (448) used first as
 * the displacement and then bumped by 73 to 521 as the stored value -- the
 * one-register-two-roles trap.  Read as *(s32 *)(workspace + 448) = 521.
 */

extern u8 *Data_03001ebc;

s32 Func_080770c0();
void Func_0808a2d0();
void Func_0808a368();
void Func_0808a370();
void Func_0808a248();

void Func_020002e0(void *object)
{
    if (Func_080770c0(0x834) != 0) {
        Func_0808a2d0();
    }

    *(s32 *)(Data_03001ebc + 448) = 521;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(object);
}
