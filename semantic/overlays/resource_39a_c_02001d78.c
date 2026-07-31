typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 39a overlay renderer call at 0x02001d78.
 *
 * Complete owner: `push {r5, r6, r7, lr}` and `sub sp, #40` at 0x02001d78,
 * and the matching `add sp, #40 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001dee, so nothing is returned; the early exit branches to that same
 * epilogue.  Four pool words follow the return and are data.
 *
 * The 40 bytes of frame are two areas: sp+0..sp+15 is the outgoing argument
 * block for the eight-argument call at 0x02001dea, and sp+16..sp+39 is a
 * six-word parameter block whose address is passed as that call's last
 * argument.  Its first word is never written, so it is left uninitialised
 * here as well.
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

struct Params_02001d78 {
    s32 unset;
    s32 count;
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

extern u32 Data_03001e40;

/* Imports; two are used for their return value. */
void Func_02001c18();
s32 Func_080000f8();
void Func_080f9010();
void Func_020041ba();
s32 Func_02004026();
s32 Func_02004034();
void Func_02003a04();

void Func_02001d78(void)
{
    struct Params_02001d78 params;
    s32 phase;
    s32 sweep;
    s32 spin;

    phase = Data_03001e40 & 3;
    if (phase != 0) {
        return;
    }

    params.count = 10;
    /* movs r3,#0x80 / lsls r3,#8 builds 0x8000. */
    params.first = 0x8000;
    params.second = 0x8000;
    params.third = (s32)0x1cccc;
    params.fourth = (s32)0x1cccc;

    if ((Data_03001e40 & 7) == 0) {
        Func_080f9010((s32)0x88);
    }

    /* (value * 2) kept to its high halfword, subtracted from 0xffff0000. */
    sweep = (s32)((u32)0xffff0000 - (((u32)Func_080000f8() << 1) & 0xffff0000));

    /* (value * 3) >> 16, then multiplied by 3, 17 and 257 with shift-adds -
     * 3 * 17 * 257 = 13107 - and negated. */
    spin = -(s32)(13107 * (((u32)(Func_080000f8() * 3)) >> 16));

    /* movs/lsls build 0x1340000, 0x400000 and 0xde0000; the last four
     * arguments are the ones stored into sp+0..sp+12. */
    Func_02001c18((s32)0x1340000, (s32)0x400000, (s32)0xde0000, sweep,
                  spin, phase, (s32)0x000d0001, &params);
}
