typedef signed int s32;

/*
 * Resource 39a overlay scene step at 0x02001050.
 *
 * Complete owner: `push {r5, lr}` and `sub sp, #8` at 0x02001050, and the
 * matching `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x0200113e, so
 * nothing is returned.  Three pool words follow the return and are data.
 *
 * The eight bytes of frame are the outgoing stack arguments of the four
 * six-argument import calls; r5 holds the constant 8 across the first pair.
 * All sixteen branches of the row are accounted for.
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

/* Imports; the three queried ones are typed for their return value. */
void Func_0200207e();
void Func_0200209c();
void Func_020020c4();
void Func_020020e4();
void Func_0200210c();
void Func_0200212c();
void Func_02003356();
void Func_0200337c();
s32 Func_02003384();
void Func_0200339c();
void Func_020033c4();
s32 Func_020033ca();
void Func_020033e4();
void Func_0200340c();
s32 Func_02003412();
void Func_0200342c();

void Func_02001050(void)
{
    Func_02003356(8, 42, 15, 5, 8, 29);

    if (Func_02003384((s32)0x301) != 0) {
        Func_0200207e(8, 22, 31);
        Func_0200337c(9, 30, 1, 3, 8, 30);
    } else {
        Func_0200209c(8, 8, 31);
        Func_0200339c(9, 30, 1, 3, 22, 30);
    }

    if (Func_020033ca((s32)0x302) != 0) {
        Func_020020c4(9, 12, 29);
        Func_020033c4(14, 33, 3, 1, 11, 33);
    } else {
        Func_020020e4(9, 12, 33);
        Func_020033e4(14, 29, 3, 1, 11, 29);
    }

    if (Func_02003412((s32)0x303) != 0) {
        Func_0200210c(10, 18, 29);
        Func_0200340c(14, 33, 3, 1, 17, 33);
    } else {
        Func_0200212c(10, 18, 33);
        Func_0200342c(14, 29, 3, 1, 17, 29);
    }
}
