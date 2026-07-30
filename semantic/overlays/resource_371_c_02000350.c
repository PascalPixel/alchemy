typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 371 owner at 0x02000350 (44 bytes, 0 calls).
 *
 * Complete owner: `push {lr}` at 0x02000350 and `pop {r1} ; bx r1` at
 * 0x02000372.  The popped register is r1, not r0, so r0 survives the return
 * and IS the result — the constant 1 set at 0x02000370.  Bytes
 * 0x02000376-0x0200037b are alignment plus the single literal pool word
 * 0x03001e40; they are reached only by `ldr r3, [pc, #20]`.
 *
 * 0x03001e40 is an IWRAM word this overlay reads all over (see the sibling
 * owners at 0x02003f10/0x02003f4c/0x02003f88, which mask it with 1, 2 and
 * 0xf).  It is a live status/flag word, not overlay image data.
 *
 * The flag byte at object + 0x54 is cleared with `eors` against the same
 * bit that was just tested, which is a clear of bit 0 only.
 */

extern u32 Data_03001e40;

s32 Func_02000350(u8 *object)
{
    u8 *flags = object + 0x54;
    u8 value = *flags;

    if ((value & 1) != 0 && (Data_03001e40 & 1) != 0) {
        *flags = (u8)(value ^ 1);
    }
    return 1;
}
