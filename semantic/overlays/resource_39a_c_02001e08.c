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
extern void Func_02001d78();

/* Imports. */
void Func_020040a0();
void Func_020040dc();
void Func_020040e6();
void Func_020040f8();
void Func_020040fa();
void Func_0200410c();
void Func_02004118();
void Func_02004122();
void Func_0200412e();
void Func_0200413e();
void Func_02004156();
void Func_020041e4();
void Func_020041f6();
void Func_020041fa();
void Func_0200420c();
void Func_0200421a();
void Func_0200421e();
void Func_02004224();
void Func_0200422a();
void Func_02004234();
void Func_02004250();
void Func_02004266();
void Func_02004272();
void Func_02004288();
void Func_020042e0();

void Func_02001e08(void)
{
    s32 pass;

    Func_02004224(19);
    Func_0200422a(182);
    Func_02004156();
    Func_0200421a();

    /* r8, r7 and r6 hold the constants 8, 7 and 1 across the loop. */
    pass = 0;
    do {
        Func_020041e4((s32)0x204318, 1);
        Func_020041fa(1);
        Func_020040a0(2);
        if (pass == 0) {
            Func_0200412e(30, 8, 12, 8, 8, 7);
            Func_0200413e(30, 57, 19, 57, 1, 1);
        }
        Func_0200421e((s32)0x203108, 1);
        Func_02004234(1);
        pass++;
        Func_020040dc(2);
        /* The back edge is an unsigned compare against 3, so the body runs
         * for pass 0..3. */
    } while ((unsigned int)pass <= 3);

    Func_020040e6(30);
    /* movs r1,#0xc8 / lsls r1,#4 builds 0xc80. */
    Func_020040fa((void *)Func_02001d78, (s32)0xc80);
    Func_020040f8(40);
    Func_02004250((s32)0x201090, 1);
    Func_02004266(40);
    Func_0200410c(80);
    Func_02004122((void *)Func_02001d78);
    Func_02004118(20);
    /* movs r0,#0x80 / lsls r0,#9 builds 0x10000. */
    Func_02004272((s32)0x10000, 1);
    Func_02004288(80);
    /* Same import as in the loop, one argument here. */
    Func_0200412e(80);
    /* movs r0,#0x82 / lsls r0,#4 builds 0x820. */
    Func_020041f6((s32)0x820);
    Func_0200420c(230);
    Func_020042e0();
    /* Same import as the first call, no argument register written here. */
    Func_02004224();
}
