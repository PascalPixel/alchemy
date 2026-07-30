typedef unsigned char u8;
typedef unsigned int u32;

/*
 * resource_377 owner at 0x020015bc, 44 bytes.  Complete owner: a 'push {..., lr}'
 * prologue and the interworking return 'pop {...} ; pop {r0} ; bx r0'.  r0 holds
 * the popped return address, so the owner returns nothing.  Its literal pool
 * word is 0x03001e40, the IWRAM counter/flag word the whole family gates on.
 *
 * TRANSPOSED from semantic/overlays/resource_371_c_02003f88.c.  The two owners are
 * the same routine shared verbatim: over all 22 halfwords they differ in exactly
 * 4 places, all of them BL halfwords.  No pool word differs.
 *
 * What was changed: the entry symbol, and the calls, re-resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 15bc' - two sites: the ARM-mode IWRAM helper Func_030003e0 and the veneer
 *    publishing Func_08009240.
 *   The 371 source predates the corrected 'bl' rule and named every site by its
 *   encoded displacement, which made one import look like several; the corrected
 *   names are used here.
 */

extern u32 Data_03001e40;

u32 Func_030003e0();
void Func_08009240();

void Func_020015bc(u8 *object)
{
    if ((Data_03001e40 & 1) != 0) {
        Func_08009240(object, Func_030003e0(Data_03001e40 >> 1, 6));
    }
}
