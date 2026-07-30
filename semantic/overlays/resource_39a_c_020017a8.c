typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x020017a8.
 *
 * Complete owner: `push {lr}` at 0x020017a8 and `pop {r0} / bx r0` at
 * 0x020017da, so nothing is returned.  One pool word (0x1999) follows the
 * return and is data.
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
 */

/* Imports.  0x02003b02 is used for its return value. */
void Func_02003ab0();
void Func_02003ae4();
s32 Func_02003b02();
void Func_02003b18();
void Func_02003b20();
void Func_02003b34();
void Func_02003b84();

void Func_020017a8(void)
{
    s32 handle;

    Func_02003ae4();
    /* r0 is not reloaded between these two branches, so 0x02003b02's result is
     * 0x02003ab0's first argument.  Whether 0x02003ab0 reads it is
     * unverified; the dataflow is preserved as written. */
    handle = Func_02003b02(8);
    Func_02003ab0(handle, 0);
    Func_02003b84();
    /* movs r1,#0x80 / lsls r1,#10 builds 0x20000. */
    Func_02003b20(0, (s32)0x20000, (s32)0x1999);
    /* movs r1,#0x84 / lsls r1,#1 builds 0x108. */
    Func_02003b34(0, (s32)0x108, (s32)0xc4);
    Func_02003b18();
}
