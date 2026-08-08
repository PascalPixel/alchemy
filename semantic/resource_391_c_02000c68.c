#include "types.h"

/*
 * resource_391 owner at 0x02000c68, 212 bytes: code 0x02000c68-0x02000d2d,
 * two alignment bytes at 0x02000d2e, and the three-word literal pool at
 * 0x02000d30-0x02000d3b.  A scene-entry setup routine.
 *
 * Calls resolved with 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_391 0c68'
 * (an overlay bl stores target_offset - 2, so overlay_show's annotations are
 * wrong for every site).  17 sites over 8 distinct targets:
 *   0x0c6e -> 0x08c0  prologue  Func_020008c0(10)
 *   0x0c76 -> 0x2bf4  veneer    Func_080770c0(0x200)
 *   0x0c80 -> 0x2c24  veneer    Func_0808a080(10)
 *   0x0c9a -> 0x2bdc  veneer    Func_080091c0(0, 17, 2, 4, 19, 17)
 *   0x0cae -> 0x0244  prologue  Func_02000244(2, 20, 17, 1, 4, 0)
 *   0x0cb4 -> 0x2c24  veneer    Func_0808a080(10)
 *   0x0cba -> 0x2bec  veneer    Func_080091e0(obj, 0)
 *   0x0cc0 -> 0x08c0  prologue  Func_020008c0(8)
 *   0x0cc6 -> 0x08c0  prologue  Func_020008c0(9)
 *   0x0cdc -> 0x2bf4  veneer    Func_080770c0(0x843)
 *   0x0ce4 -> 0x0d3c  prologue  Func_02000d3c()
 *   0x0cea -> 0x2bf4  veneer    Func_080770c0(0x845)
 *   0x0cf8/0x0d02/0x0d0c/0x0d16/0x0d20 -> 0x2c7c veneer
 *                               Func_0808a0f0(17..21, 0, 0)
 * placed(17) >= inventory calls=17.  Note that the tool's own site count is the
 * authority here; the inventory field is only a lower bound.
 *
 * Func_02000244's six-argument signature is taken from the byte-exact
 * assets/code/resource_391_c_02000244.c, and the two stack words this owner
 * writes at [sp,#0]/[sp,#4] are its arg4/arg5.  Func_080091c0 is written with
 * the same four-register-plus-two-stack shape because the call site sets [sp,#0]
 * and [sp,#4] identically; that is the established six-argument renderer ABI.
 *
 * Epilogue 'movs r0,#0 / add sp,#8 / pop {r5} / pop {r1} / bx r1': the popped
 * return address goes to r1, not r0, so r0 survives and IS the result - this
 * owner returns 0 unconditionally.
 *
 * The pool word 0x02000240 is used unshifted, exactly as the byte-exact
 * assets/code/resource_391_c_02000c14.c spells it (it is outside the
 * 0x0200_8xxx..0x0200_bxxx in-image band, so the 0x02008000 link base does not
 * apply to it).  The gate reads a signed halfword at +450.
 *
 * UNCERTAINTIES:
 *  - 0x0843 and 0x0845 are opaque ids handed to Func_080770c0, which is used
 *    three times here in two different senses: once on a raw count-like 0x200
 *    and twice on those ids.  Its return is only ever tested against zero, so
 *    it is declared s32 with an open argument list.
 *  - r5 is loaded with 0 solely to be stored as Func_02000244's arg5; it is
 *    not a live value across anything.
 */

/* RAM globals, spelled unshifted as the byte-exact siblings do. */
extern u8 Data_02000240[];

/* Old-style declarations are mandatory in overlay sources. */
void Func_02001530();
s32 Func_0200386c();
u8 *Func_020038a6();
void Func_02003878();
s32 Func_02000ef4();
u8 *Func_020038da();
void Func_020038a8();
void Func_02001582();
void Func_02001588();
s32 Func_020038d2();
void Func_02001a22();
s32 Func_020038e0();
void Func_02003976();
void Func_02003980();
void Func_0200398a();
void Func_02003994();
void Func_0200399e();

                    

                     

s32 Func_02000c68(void)
{
    u8 *obj;

    Func_02001530(10);

    if (Func_0200386c(0x200) != 0) {
        obj[0x23] = 2;
        obj = Func_020038a6(10);

        Func_02003878(0, 17, 2, 4, 19, 17);
        Func_02000ef4(2, 20, 17, 1, 4, 0);

        obj = Func_020038da(10);
        Func_020038a8(obj, 0);
    }

    Func_02001582(8);
    Func_02001588(9);

    if (*(s16 *)(Data_02000240 + 450) == 4) {
        if (Func_020038d2(0x843) == 0) {
            Func_02001a22();
        }
    }

    if (Func_020038e0(0x845) != 0) {
        Func_02003976(17, 0, 0);
        Func_02003980(18, 0, 0);
        Func_0200398a(19, 0, 0);
        Func_02003994(20, 0, 0);
        Func_0200399e(21, 0, 0);
    }

    return 0;
}
