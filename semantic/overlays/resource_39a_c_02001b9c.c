typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001b9c.
 *
 * Complete owner: `push {r5, lr}` at 0x02001b9c and `pop {r5} / pop {r0} /
 * bx r0` at 0x02001bcc, so nothing is returned.  Two pool words follow the
 * return and are data.
 *
 * 0x0200a488 is 0x02008000 + 0x2488, i.e. in-image data of this overlay under
 * its proven 0x02008000 link base; 0x02001b1c is the routine that stores the
 * object into it.  The raw pool value is kept as the symbol spelling, which is
 * the convention the byte-exact sources here already follow.
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

/* Imports. */
void Func_02003e86();
void Func_02003ed8();
void Func_02003edc();
void Func_02003efe();
void Func_02003f0a();
void Func_02003f8e();

void Func_02001b9c(void)
{
    Func_02003ed8();
    /* r5 holds &Data_0200a488 across the calls; the word is reloaded before
     * the second test. */
    if (Data_0200a488 != 0) {
        Func_02003f8e(Data_0200a488, 3);
    }
    Func_02003efe((s32)0xe6, 0);
    Func_02003edc((s32)0xf13);
    if (Data_0200a488 != 0) {
        Func_02003e86(Data_0200a488);
    }
    Func_02003f0a();
}
