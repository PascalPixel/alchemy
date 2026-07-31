typedef signed int s32;

/*
 * Resource 39a overlay sequence at 0x02001e08.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus `mov r7, r8 / push {r7}`
 * and `sub sp, #8` at 0x02001e08, and the matching `add sp, #8 / pop {r3} /
 * mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02001ee6, so nothing
 * is returned.  Five pool words follow the return and are data.  All 25
 * distinct branch targets in the row are placed.
 *
 * 0x02009d79 is 0x02008000 + 0x1d79, i.e. the in-image routine 0x02001d78
 * plus the Thumb bit: a third witness for this overlay's 0x02008000 link
 * base.  It is handed to two imports as a callback.
 *
 * 0x02004224 and 0x0200412e are each reached with two different argument
 * counts, which is why the declarations are old-style.
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

/* The callback, named by its in-image address. */
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();
void Func_08009180();
void Func_080770c8();
void Func_080772e8();
void Func_0808a018();
void Func_0808a020();
void Func_0808a330();
void Func_0808a348();
void Func_0808a460();
void Func_0808a4f0();
void Func_080f9010();
extern void Func_02001d78();

/* Imports. */

void Func_02001e08(void)
{
    s32 pass;

    Func_080f9010(19);
    Func_080f9010(182);
    Func_0808a018();
    Func_0808a460();

    /* r8, r7 and r6 hold the constants 8, 7 and 1 across the loop. */
    pass = 0;
    do {
        Func_0808a330((s32)0x204318, 1);
        Func_0808a348(1);
        Func_080000c0(2);
        if (pass == 0) {
            Func_08009180(30, 8, 12, 8, 8, 7);
            Func_08009180(30, 57, 19, 57, 1, 1);
        }
        Func_0808a330((s32)0x203108, 1);
        Func_0808a348(1);
        pass++;
        Func_080000c0(2);
        /* The back edge is an unsigned compare against 3, so the body runs
         * for pass 0..3. */
    } while ((unsigned int)pass <= 3);

    Func_080000c0(30);
    /* movs r1,#0xc8 / lsls r1,#4 builds 0xc80. */
    Func_080000d0((void *)Func_02001d78, (s32)0xc80);
    Func_080000c0(40);
    Func_0808a330((s32)0x201090, 1);
    Func_0808a348(40);
    Func_080000c0(80);
    Func_080000d8((void *)Func_02001d78);
    Func_080000c0(20);
    /* movs r0,#0x80 / lsls r0,#9 builds 0x10000. */
    Func_0808a330((s32)0x10000, 1);
    Func_0808a348(80);
    /* Same import as in the loop, one argument here. */
    Func_080000c0(80);
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    Func_080770c8((s32)0x820);
    Func_080772e8(230);
    Func_0808a4f0();
    /* Same import as the first call, no argument register written here. */
    Func_0808a020();
}
