typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02000bc8.
 *
 * Complete owner: `push {lr}` at 0x02000bc8 and `pop {r0} / bx r0` at
 * 0x02000be4.  Three pool words follow the return and are data.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address here is an import identity, not a place to
 * disassemble.  That is the convention the byte-exact sources in this same
 * overlay already use (`assets/code/resource_39a_c_02000030.c` declares
 * `Func_02002442`), so imports are named by the address their call site
 * computes and their interfaces are left open.  Declarations are old-style
 * because one name is reached with different argument counts.
 */

/* Imports. */
void Func_02002ef6();
void Func_02002efe();
void Func_02002f04();
void Func_02002f0a();

void Func_02000bc8(void)
{
    /* Four consecutive identifiers, 0x30f..0x312.  0x310 is built with
     * movs #0xc4 / lsls #2 rather than pooled. */
    Func_02002ef6((s32)0x30f);
    Func_02002efe((s32)0x310);
    Func_02002f04((s32)0x311);
    Func_02002f0a((s32)0x312);
}
