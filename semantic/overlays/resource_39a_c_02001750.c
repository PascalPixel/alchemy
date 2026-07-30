typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001750.
 *
 * Complete owner: `push {lr}` at 0x02001750 and `pop {r1} / bx r1` at
 * 0x02001792.  r1 carries the return address, so r0 survives and the constant
 * zero written at 0x02001790 is the result.  Four pool words follow the
 * return and are data.
 *
 * This routine is exported: the eight-byte veneer at file offset 0x28
 * (`ldr r4,[pc,#0] / bx r4 / .word 0x02009751`) reaches it.  0x02009751 is
 * 0x02008000 + 0x1751, which is this entry plus the Thumb bit and is one of
 * the three witnesses that this overlay is linked at 0x02008000 - so pool
 * words in 0x0200_8xxx..0200_bxxx are in-image, while the `bl` band above is
 * not.
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

/* 0x03001ebc is the overlay's workspace pointer. */
extern u8 *Data_03001ebc;
/* 0x02000240 is below the link base, so it is a resident table rather than
 * in-image data; the byte-exact siblings in this overlay index it the same
 * way. */
extern s16 Data_02000240[];

/* Imports. Those used for their return value are typed. */
void Func_02002f30();
void Func_02002f72();
s32 Func_02003a7e();
void Func_02003aa4();

s32 Func_02001750(void)
{
    u8 *workspace = Data_03001ebc;

    /* movs r1,#0xe0 / lsls r1,#1 gives the byte offset 448; movs r2,#0x81 /
     * lsls r2,#2 builds 0x204. */
    *(s32 *)(workspace + 448) = 0x204;

    if (Func_02003a7e((s32)0x109) == 0 && Data_02000240[224] == 0x34) {
        /* movs r0,#0xa2 / lsls r0,#1 builds 0x144. */
        Func_02003aa4((s32)0x144);
        Func_02002f30();
    } else {
        Func_02002f72();
    }
    return 0;
}
