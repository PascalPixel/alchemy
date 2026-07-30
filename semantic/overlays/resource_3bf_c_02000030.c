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
 * RESOLVED (was recorded here as an uncertainty).  The apparent target
 * `0x0200581c` — an address inside the routine that begins at 0x020057ec — was
 * an artefact of pc-relative `bl` decoding.  An overlay `bl` stores the
 * target's image offset minus two, so the real target is
 * `bun tools/overlay_call_targets.ts resource_3bf 0030` -> 0x020057bc, which
 * is `bx r3` in this overlay's own 14-entry `call_via` bank at
 * 0x020057b0..0x020057ea (r0,r1,r2,r3,r4,... in register order, four bytes per
 * entry).  The call is therefore exactly the indirect call this file already
 * modelled: through r3, which the pool word 0x030001d8 loaded with the
 * relocated ARM-mode IWRAM square root.  No second entry point exists at
 * 0x0200581c.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c.  This tree keeps the
 * printed-offset spelling for symbol names, as the byte-exact sources under
 * assets/code do.
 */
typedef s32 (*IwramHelper_02000030)(s32);

s32 Func_020057bc(s32 value, s32 spare1, s32 spare2, IwramHelper_02000030 helper);

s32 Func_02000030(const s32 *a, const s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;
    s32 sumOfSquares = dx * dx + dy * dy + dz * dz;

    return Func_020057bc(sumOfSquares, dz * dz, dy * dy,
                         (IwramHelper_02000030)0x030001d8);
}
