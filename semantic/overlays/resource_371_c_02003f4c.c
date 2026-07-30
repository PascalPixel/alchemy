typedef unsigned char u8;
typedef unsigned int u32;

/*
 * Resource 371 owner at 0x02003f4c (60 bytes, 3 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003f4c and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x02003f7e.  r0 holds the
 * popped return address, so the owner returns nothing.
 * 0x02003f84-0x02003f87 is the literal pool word 0x03001e40.
 *
 * Same shape as 0x02003f88, plus a second stage gated on the low four bits
 * of the IWRAM word being zero — i.e. every sixteenth tick.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity in this load-relocated
 * overlay.  Old-style declarations, because the interfaces are unknown.
 */

extern u32 Data_03001e40;

u32 Func_020080e0();
void Func_020081c8();
void Func_02007fd4();

void Func_02003f4c(u8 *object)
{
    if ((Data_03001e40 & 1) != 0) {
        Func_020081c8(object, Func_020080e0(Data_03001e40 >> 1, 6));
    }
    if ((Data_03001e40 & 0xf) == 0) {
        Func_02007fd4(object);
    }
}
