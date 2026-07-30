/*
 * Resource 3c8 at 0x020026e4: three-call cutscene step.
 *
 * The owner is complete: `push {lr}` at 0x020026e4 and the matching
 * `pop {r0} ; bx r0` at 0x020026f2..0x020026f4, followed by one alignment
 * halfword (0x020026f6) before the next prologue at 0x020026f8.  No live
 * frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * No argument register is written anywhere in the body, so no argument is
 * asserted at any of the three call sites.  Whatever r0-r3 hold on entry is
 * simply passed through to the first callee; the reconstruction does not
 * claim that any of them is read.
 *
 * Import naming: 0x02002a60 falls numerically inside this overlay's own code
 * region and, disassembled at that offset, lands inside another routine's
 * live frame.  Overlay branch displacements are fixed up when the module is
 * loaded, so an encoded address is a stable identity for the import a call
 * site reaches, not a place to disassemble; this package's byte-exact sources
 * already rely on that (0x020009c8 calls `Func_02000b9a`, two bytes into the
 * prologue of 0x02000b98).  None of the three targets names a local
 * continuation owned by this C file.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_0200757c();
void Func_02002a60();
void Func_0200758c();

void Func_020026e4(void)
{
    Func_0200757c();
    Func_02002a60();
    Func_0200758c();
}
