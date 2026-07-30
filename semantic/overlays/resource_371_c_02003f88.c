typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 371 owner at 0x02003f88 (44 bytes, 2 calls).
 *
 * Complete owner: `push {r5, lr}` at 0x02003f88 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02003faa.  r0 holds the popped return
 * address, so the owner returns nothing.  0x02003fb0-0x02003fb3 is the
 * literal pool word 0x03001e40, reached only by `ldr r0, [pc, #32]`.
 *
 * The IWRAM word at 0x03001e40 is a running counter/flag word: bit 0 gates
 * the work, and the value shifted right by one (unsigned `lsrs`) is passed
 * as the first argument of the first import.  The sibling owners at
 * 0x02003f4c and 0x02003f10 use the same word with masks 1, 2 and 0xf.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style declarations are used
 * because the import interfaces are unknown.
 */

extern u32 Data_03001e40;

u32 Func_0200811c();
void Func_02008204();

void Func_02003f88(u8 *object)
{
    if ((Data_03001e40 & 1) != 0) {
        Func_02008204(object, Func_0200811c(Data_03001e40 >> 1, 6));
    }
}
