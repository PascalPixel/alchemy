typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x02003d10 (220 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x02003d10, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x02003dd0-0x02003dd4. The trailing pool at 0x02003dd8-0x02003de8
 * (0x0000cccc, 0x00006666, 0x00000266, 0x0200e738, 0x00001e3b) is
 * included per the usual pool rule, immediately followed by the next
 * owner's push {lr} at 0x02003dec, already this overlay's row
 * `0x02003dec | 1 call`, so the span is 0x02003d10-0x02003dec,
 * 220 bytes.
 *
 * A flat setter sequence, no branches. `Func_0200833a`'s return value is
 * kept live across two calls and threaded into `Func_0200a1d8` alongside
 * a pool-loaded handle at 0x0200e738.
 *
 * Raw callee naming.
 */

extern u8 Value_00001e3b;

void Func_0200a1dc(void);
void Func_02008606();
void Func_0200a2a6();
void Func_0200a1ec();
void Func_0200a22e();
void Func_0200a272();
void Func_0200a280();
void Func_0200a32c();
void Func_0200a2d6();
u8 *Func_0200833a(void);
void Func_0200a232();
void Func_0200a3d8();
void Func_0200a1d8();
void Func_0200a246();
void Func_0200a2ee();
void Func_0200a254();
void Func_0200a2d2();
void Func_0200862c();
void Func_0200a362();
void Func_0200a384();
void Func_020086b6();

void Func_02003d10(void)
{
    u8 *obj;

    Func_0200a1dc();
    Func_02008606(15, 0, 1);
    Func_0200a2a6(8, 1);
    Func_0200a1ec(20);
    Func_0200a22e(8, 0xcccc, 0x6666);
    Func_0200a272(8, 0x1d4, 0x266);
    Func_0200a280(8, 0x1d8, 0x254);
    Func_0200a32c(8, 0x8000, 20);
    Func_0200a2d6(8, 4, 20);

    obj = Func_0200833a();
    Func_0200a232(20);
    Func_0200a3d8(0xd6);
    Func_0200a1d8(obj, 0x0200e738);
    Func_0200a246(0x28);
    Func_0200a2ee(8, 3);
    Func_0200a254(20);
    Func_0200a2d2(8, 0x1d2, 0x270);
    Func_0200862c(8, 0x5000);
    Func_0200a32c(8, 2);
    Func_0200a362((s32)&Value_00001e3b);
    Func_0200a384(8, 0, 20);
    Func_020086b6(9, 11, 0);
}
