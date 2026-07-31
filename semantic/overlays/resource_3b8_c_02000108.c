typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;

/*
 * Resource 3b8 cutscene step at 0x02000108.
 *
 * The owner is complete: its own `push {r5, r6, lr}` prologue at 0x02000108
 * and the matching `pop {r5, r6} / pop {r0} / bx r0` interworking return at
 * 0x02000256.  Two literal pools sit inside the span (0x02000154-0x0200015f
 * and 0x0200025c-0x02000263); both are branched over and are data, not code.
 *
 * Every `bl` in this overlay targets an address in the 0x02004xxx band.  That
 * band is not overlay code: it is the overlay's import area, part veneer table
 * (`ldr r4, [pc, #0] / bx r4 / .word <main-image address>` in eight-byte
 * entries from 0x02004340) and part loader descriptor data.  The project's
 * convention, already used by the byte-exact sources in this overlay, is to
 * name each import by the address the branch computes and to leave its
 * interface open until the import table itself is reconstructed.  That is done
 * here too, so the numeric names below carry no semantics beyond "the import
 * this call site reaches".
 *
 * Uncertainties are noted next to the code.  The main ones are the two chained
 * calls at 0x02000122/0x02000126, where r0 is carried from one callee's return
 * into the next callee's first argument, and 0x02004612, which is reached once
 * with three set argument registers and once with only r0 set.
 */

/* Imports. Old-style declarations: their real interfaces are not yet known,
 * and 0x02004612 is called with different argument counts from this owner. */
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
s32 Func_0808a080();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a110();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
s32 Func_0808a460();
void Func_0808a580();

/* These are used for their return values. */

void Func_02000108(s32 actor)
{
    u16 *context;
    s32 facing;
    s32 handle;

    context = Func_0808a080(0);

    /*
     * ldrh [context, #6] + 0x2000, masked with 0xffffc000, then sign extended
     * from bit 15 (lsls #16 / asrs #16).  The mask's high half is discarded by
     * that sign extension, so only bits 14-15 of the biased halfword survive.
     */
    facing = (s16)(((s32)context[3] + 0x2000) & 0xc000);

    /*
     * r0 is not reloaded between these two branches: the pointer returned by
     * 0x02004510 is the argument of 0x02004504, and 0x02004504's return value
     * is in turn the argument of 0x02004630.  Whether 0x02004630 actually
     * reads that register is unverified; the dataflow is preserved as written.
     */
    Func_0808a018(Func_0808a460(context));

    if (Func_080770c0(0x200) == 0) {
        Func_080770c8(0x200);
        Func_080770d0(0x969);
        Func_0808a170(0x1ff7);
        Func_0808a180(actor, 0);

        Func_0808a010(10);
        if (facing == 0x4000) {
            Func_0808a0d0(0, 40, 104);
            Func_0808a1b8(0, 0, 0);
        }
        Func_0808a090(actor, 0x10000, 0x8000);
        Func_0808a580(actor, 0, -48);
        Func_0808a580(actor, 64, 0);
        Func_0808a1b8(actor, 0x4000, 0);
    } else {
        Func_080770d0(0x200);
        Func_080770c8(0x969);
        Func_0808a1b8(actor, 0x4000, 0);
        Func_0808a0d0(0, 0x78, 0x60);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a010(20);

        handle = 0x1ff8;
        Func_0808a170(handle);
        Func_0808a178(actor, 0);
        if (Func_0808a070(0, 0) == 0) {
            Func_0808a170(handle + 1);
            Func_0808a180(actor, 0);
        } else {
            Func_0808a170(handle + 2);
            Func_0808a180(actor, 0);
        }
        Func_0808a010(10);
        Func_0808a110(actor, 3);
        /* Only r0 is set here; r1 and r2 hold call-clobbered values from the
         * preceding branch, so no further arguments are asserted. */
        Func_0808a010(20);
        Func_0808a580(actor, -64, 0);
        Func_0808a580(actor, 0, 48);
    }

    Func_0808a020();
}
