typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 39a overlay routine at 0x02001fac.
 *
 * Complete owner: `push {lr}` at 0x02001fac and `pop {r0} / bx r0` at
 * 0x02001ffc, so nothing is returned; the early exit branches to that same
 * epilogue.  Five pool words follow the return and are data.
 *
 * Uncertainty: the prologue saves only lr, yet 0x02001fce writes r4 without
 * saving it.  r4 only ever holds the constant zero stored at 0x02001fd4, so
 * the observable effect is a clobbered callee-saved register; it is written
 * here as a plain constant.
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

/* 0x02000240 is below the 0x02008000 link base, so it is a resident table.
 * 0x03001f30 and 0x03001ecc are RAM pointer words; 0x03001ecc is reached as
 * 0x03001f30 - 100 by the same `ldr` base, which is how the original built
 * it. */
extern s16 Data_02000240[];
extern u8 *Data_03001f30;
extern u8 *Data_03001ecc;

/* Imports. */
void Func_080000c0();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();

void Func_02001fac(void)
{
    u8 *state;
    u8 *table;

    /* movs r2,#0xe1 / lsls r2,#1 gives the byte offset 450, i.e. index 225. */
    if (Data_02000240[225] > 6) {
        return;
    }

    state = Data_03001f30;
    state[52] = 1;

    table = Data_03001ecc;
    table[0x53e] = 0;
    table[0x53c] = 1;
    table[0x53d] = 1;

    Func_0808a338(0, 1);
    Func_0808a330((s32)0x203108, 1);
    Func_0808a348(16);
    Func_080000c0(16);
}
