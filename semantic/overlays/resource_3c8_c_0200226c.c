typedef signed int s32;

/*
 * Resource 3c8 at 0x0200226c: cutscene step guarded by a one-shot query.
 *
 * The owner is complete: `push {lr} ; sub sp, #8` at 0x0200226c and the
 * matching `add sp, #8 ; pop {r0} ; bx r0` at 0x0200229c..0x020022a0.  No
 * literal pool sits inside the span and no live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The eight reserved stack bytes carry arguments five and six of the
 * six-argument placement ABI this package uses throughout: r0-r3 then
 * [sp,#0] then [sp,#4].  The byte-exact sibling 0x020010cc shows the same
 * shape (`Func_02005f1c(36, 8, 1, 3, 23, 8)`).
 *
 * All six `bl` sites are accounted for: 0x02007106, 0x02002f3e, 0x020070c2,
 * 0x02002606, 0x02007132, 0x020041fa.
 *
 * Import naming: 0x02002f3e, 0x02002606 and 0x020041fa fall numerically
 * inside this overlay's own code region, but overlay branch displacements are
 * fixed up at load time, so an encoded address is an identity for the import
 * a call site reaches, not a place to disassemble.  This matches the existing
 * byte-exact sources in this package (0x020009c8 calls `Func_02000b9a`,
 * 0x02000b08 calls `Func_02000c8a`) and the note in
 * semantic/overlays/resource_3c8_c_02001d48.c.  None of them names a local
 * continuation owned by this C file.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007106();
void Func_020070c2();
void Func_02002606();
void Func_02007132();
void Func_020041fa();

/* Used for its return value. */
s32 Func_02002f3e();

void Func_0200226c(void)
{
    Func_02007106();
    if (Func_02002f3e() == 0) {
        Func_020070c2(108, 39, 13, 7, 44, 39);
        Func_02002606();
    }
    Func_02007132();
    Func_020041fa();
}
