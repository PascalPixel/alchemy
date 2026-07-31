typedef signed int s32;

/*
 * Resource 3c8 at 0x02002f30: nine-call camera/scene setup step.
 *
 * The owner is complete: `push {lr}` at 0x02002f30 and the matching
 * `pop {r0} ; bx r0` at 0x02002f88..0x02002f8a.  No literal pool sits inside
 * the span and no live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * Every constant here is materialised as `movs #imm ; lsls #shift`, so the
 * values below are exact: 0x80<<8 = 0x8000, 0x80<<7 = 0x4000, 130<<2 = 520,
 * 178<<2 = 712, 130<<18 = 0x02080000 and 196<<18 = 0x03100000.  The last two
 * are 16.16 world coordinates in the same scale the byte-exact siblings use
 * (0x02000754's 0x03280000-style arguments).
 *
 * All nine `bl` sites are accounted for: 0x02007dc8, 0x02007df6, 0x02007e2c,
 * 0x02007eb8, 0x02002fc4, 0x02007e92, 0x02007e08, 0x02007f26, 0x02007e22.
 * The first is reached with no argument register written, so no argument is
 * asserted for it.
 *
 * Import naming follows the note in resource_3c8_c_020002f0.c: each name is
 * the address its call site computes.  0x02002f3e - the address a *different*
 * owner (0x0200226c) branches to - lands in the middle of this routine's own
 * constant-building sequence, which is the direct proof that overlay branch
 * displacements are fixed up at load time and that an encoded address is an
 * import identity rather than a location.  Nothing in this file is a
 * continuation of another owner.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02000058();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a140();
void Func_0808a1b8();
void Func_0808a248();

void Func_02002f30(void)
{
    Func_0808a018();
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0d0(0, 520, 712);
    Func_0808a1b8(0, 0x4000, 10);
    Func_02000058(0x02080000, 0, 0x03100000, 223);
    Func_0808a140(0, 6, 0);
    Func_0808a010(60);
    Func_0808a248(20);
    Func_0808a020();
}
