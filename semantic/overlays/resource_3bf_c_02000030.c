typedef signed int s32;

/*
 * Resource 3bf 3-D distance helper at 0x02000030.
 *
 * r0 and r1 each address three consecutive 32-bit coordinates.  The routine
 * subtracts them component-wise, arithmetic-shifts each difference right by 16
 * (the coordinates are 16.16 fixed point and only the integer part is kept),
 * squares the three integer differences, sums them, and tail-calls the
 * square-root helper with the sum in r0.
 *
 * The final call has the `call_via r3` shape documented in HANDOVER section 0:
 * r3 is loaded from the literal pool at 0x02000068 with 0x030001d8, the
 * relocated ARM-mode IWRAM square-root helper.  r3 is NOT scratch here, and it
 * is the register the actual work is reached through, so it is spelled as a
 * function-pointer argument.  r1 and r2 still hold the third and second
 * squared differences when the call is made (they are the leftovers of the
 * summation), so they are passed through rather than invented or dropped.
 *
 * The call's result is returned unchanged: the epilogue is
 * `pop {r5}; pop {r1}; bx r1` with r0 untouched.
 *
 * UNCERTAINTY, and the reason this file names the callee by its raw address
 * rather than describing it: the reconstruction assembly really does encode
 * `bl .L_0200581c`, and that label sits inside the routine that begins at
 * 0x020057ec rather than at a prologue.  Two readings are open — either the
 * overlay carries a second entry point there, or the module's +0x8000 link
 * base (see below) means the printed target is not the one the original
 * linker resolved.  Nothing above the call depends on the resolution; the
 * distance computation, the register contents at the call, and the pass-through
 * return are all direct assembly readings.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c.  This tree keeps the
 * printed-offset spelling for symbol names, as the byte-exact sources under
 * assets/code do.
 */
typedef s32 (*IwramHelper_02000030)(s32);

s32 Func_0200581c(s32 value, s32 spare1, s32 spare2, IwramHelper_02000030 helper);

s32 Func_02000030(const s32 *a, const s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;
    s32 sumOfSquares = dx * dx + dy * dy + dz * dz;

    return Func_0200581c(sumOfSquares, dz * dz, dy * dy,
                         (IwramHelper_02000030)0x030001d8);
}
