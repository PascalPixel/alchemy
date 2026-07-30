typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3ae, owner at 0x02000b68 (54 bytes, prologue to epilogue).
 *
 * Prologue `push {r5, r6, lr}` at 0x02000b68, interworking return
 * `pop {r5, r6} / pop {r0} / bx r0` at 0x02000b98.  Because the final pop
 * targets r0, r0 holds the popped return address and nothing is returned:
 * the owner is void.
 *
 * Call targets were resolved with `tools/overlay_call_targets.ts`, which
 * applies the overlay `bl` rule (stored displacement + 2 == target file
 * offset) rather than the disassembler's pc-relative annotation:
 *
 *   0x02000b6c -> file 0x15a0 veneer -> Func_0808a080
 *   0x02000b74 -> file 0x15a0 veneer -> Func_0808a080
 *   0x02000b7a -> file 0x1510 veneer -> Func_080091e0
 *   0x02000b82 -> file 0x1648 veneer -> Func_0808a1e0
 *
 * Func_0808a080 is the scene-record accessor: it maps the actor handle in r0
 * to the actor's record pointer.  It is called twice with the same argument
 * here; the first result is kept in r6 for the trailing field writes and the
 * second is fed straight into Func_080091e0.  The duplicate call is in the
 * bytes and is preserved.
 *
 * Uncertainties: the meanings of the two record fields written at +0x55 and
 * +0x23 are not established beyond their widths (both byte accesses), and the
 * second argument 3 of Func_0808a1e0 is an opaque mode selector.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void *Func_0808a080();
void Func_080091e0();
void Func_0808a1e0();

void Func_02000b68(s32 actor)
{
    u8 *record;

    record = (u8 *)Func_0808a080(actor);

    /* r0 is reloaded with `actor` before this second lookup. */
    Func_080091e0(Func_0808a080(actor), 0);

    Func_0808a1e0(actor, 3);

    record[0x55] = 0;
    record[0x23] |= 2;
}
