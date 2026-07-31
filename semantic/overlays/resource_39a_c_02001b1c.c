typedef unsigned char u8;
typedef signed int s32;
#define NULL ((void *)0)

/*
 * Resource 39a overlay object constructor at 0x02001b1c.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02001b1c and
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02001b90, so nothing is
 * returned; the early exit branches to that same epilogue.  One pool word
 * follows the return and is data.
 *
 * 0x0200a488 is 0x02008000 + 0x2488, in-image data of this overlay under its
 * proven 0x02008000 link base.  0x02001b9c is the reader.
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

extern void *Data_0200a488;

/* Imports.  Two are used for their return value. */
s32 Func_08000140();
void Func_08000150();
void Func_080001c8();
s32 Func_080090c8();
void Func_08015250();
u8 *Func_02003de6();
s32 Func_02003df4();
void Func_02003e1a();
void Func_02003e1c();
void Func_02003e7c();

void Func_02001b1c(void)
{
    u8 *object;
    u8 *record;
    s32 buffer;

    /* movs/lsls build 0xf80000, 0x80000 and 0x980000. */
    object = Func_080090c8(22, (s32)0xf80000, (s32)0x80000, (s32)0x980000);
    if (object == NULL) {
        return;
    }

    record = *(u8 **)(object + 80);
    record[38] = 0;
    record[39] = 0;
    /* movs r3,#33 / negs r3,r3 gives the mask ~0x20. */
    record[5] &= (u8)~0x20;
    record[9] &= 0x0f;
    object[85] = 0;
    object[92] = 1;

    /* movs r1,#0xc1 / lsls r1,#3 builds 0x608. */
    buffer = Func_08000140(17, (s32)0x608);
    /* This result is discarded; only the side effect is wanted. */
    Func_08015250((s32)0xe6);
    /* movs r3,#0x80 / lsls r3,#3 builds the 0x400 advance. */
    buffer += 0x400;
    Func_080001c8((s32)record[28], (s32)0x80, buffer);
    Func_08000150(17);
    Data_0200a488 = object;
}
