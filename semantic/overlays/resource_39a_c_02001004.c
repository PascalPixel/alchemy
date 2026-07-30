typedef unsigned char u8;
typedef signed int s32;
#define NULL ((void *)0)

/*
 * Resource 39a overlay routine at 0x02001004.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus `mov r7, r8 / push {r7}` at
 * 0x02001004, and the matching `pop {r3} / mov r8, r3 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x02001046.  The popped word is the return address, so
 * nothing is returned.
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

/* Import; used for its return value. */
u8 *Func_02003362();
void Func_020033b8();

void Func_02001004(s32 subject, s32 x, s32 y)
{
    u8 *entry;

    /* r0 is not rewritten before the branch, so the owner's own first
     * argument is forwarded. */
    entry = Func_02003362(subject);
    if (entry == NULL) {
        return;
    }

    Func_020033b8(subject, 3);
    entry[34] = 2;
    entry[35] |= 2;
    /* movs r2,#0x80 / lsls r2,#12 builds the 0x80000 bias added to each
     * coordinate promoted by 20 bits. */
    *(s32 *)(entry + 8) = (x << 20) + 0x80000;
    *(s32 *)(entry + 16) = (y << 20) + 0x80000;
}
