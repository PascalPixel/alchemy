typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 371 owner at 0x02003f10 (60 bytes, 3 calls).
 *
 * Complete owner: `push {r5, lr}` at 0x02003f10 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02003f42.  r0 holds the popped return
 * address, so the owner returns nothing.  0x02003f48-0x02003f4b is the
 * literal pool word 0x03001e40, loaded twice (`ldr r3, [pc, #52]` and
 * `ldr r3, [pc, #20]`).
 *
 * The IWRAM flag word is re-read after the first branch, so both tests are
 * written against the live global rather than a cached copy.
 *
 * At 0x02003f20 only r1 is set: r0 still carries this owner's argument
 * untouched from entry, which is why the first import receives the object.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity in this load-relocated
 * overlay.  Old-style declarations, because the interfaces are unknown.
 */

extern u32 Data_03001e40;

void Func_02008180();
void Func_0200818a();
void Func_02007f98();

void Func_02003f10(u8 *object)
{
    if ((Data_03001e40 & 2) != 0) {
        Func_02008180(object, 7);
    } else {
        Func_0200818a(object, 0);
    }
    if ((Data_03001e40 & 0xf) == 0) {
        Func_02007f98(object);
    }
}
