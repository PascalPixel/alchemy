typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02000d6c.
 *
 * Complete owner: `push {lr}` at 0x02000d6c and `pop {r0} / bx r0` at
 * 0x02000d96.  The popped word is the return address, so nothing is returned.
 * The literal pool after the return is data, not code.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 *
 */

/* Imports. */
void Func_0200efe();
void Func_02002370();
void Func_02002ff0();
void Func_020030b2();
void Func_02003182();
void Func_02003194();

void Func_02000d6c(void)
{
    Func_02003182((s32)0xf1);
    /* movs r2,#0x70 / negs r2,r2 */
    Func_0200efe(8, 0, -112);
    Func_02003194((s32)0x121);
    Func_020030b2((s32)0x313);
    Func_02002ff0(2);
    Func_02002370();
}
