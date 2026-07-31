typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x0200366c (328 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x0200366c, epilogue `pop {r0} / bx r0` at
 * 0x02003794-0x02003796. The trailing pool at 0x02003798-0x020037b0
 * (0x0000093e, 0x0000092b, 0x0000092a, 0x00000929, 0x00000928,
 * 0x00000925, 0x00000926) ends exactly where the next owner's `push
 * {r5,r6,r7,lr}` begins (0x020037b4, already this overlay's row
 * `0x020037b4 | 5 calls`), so the span is 0x0200366c-0x020037b4,
 * 328 bytes.
 *
 * Same six-way `Value_XXXXXXXX`-gated cascade shape as 0x0200351c
 * earlier in this overlay, each block returning after its own setter
 * sequence; the final (seventh) gate's positive branch does not return
 * early but runs one more pair of checks before an optional last call.
 *
 * Raw callee naming.
 */

extern u8 Value_0000093e;
extern u8 Value_0000092b;
extern u8 Value_0000092a;
extern u8 Value_00000929;
extern u8 Value_00000928;
extern u8 Value_00000925;
extern u8 Value_00000926;

void Func_02009a92();
void Func_02006e2a(void);

s32 Func_02009b1c();
void Func_02007f72();
void Func_02007f2a();
void Func_02007f3a();

s32 Func_02009b54();
void Func_02009c0e();
void Func_02009c1e();
void Func_02007fc0();

s32 Func_02009b80();
void Func_02007fd6();
void Func_02007fe0();
void Func_02007952();

s32 Func_02009ba6();
void Func_02007ffc();
void Func_02008006();
void Func_02007978();

s32 Func_02009bcc();
void Func_02008022();
void Func_0200802c();
void Func_0200799e();

s32 Func_02009bf2();
void Func_02008048();
void Func_02009cb2();
void Func_020079c4();

void Func_02009cca();
s32 Func_02009c20();
s32 Func_02009c2a();
void Func_0200703e(void);

void Func_0200366c(void)
{
    Func_02009a92(1);
    Func_02006e2a();

    if (Func_02009b1c((s32)&Value_0000093e) != 0) {
        Func_02007f72(4, 4, 0);
        Func_02007f2a(8, 0x19c, 0xde, 0x3000);
        Func_02007f3a(9, 0x1ca, 0xa1, 0x8000);
        return;
    }

    if (Func_02009b54(0x8a0) != 0) {
        Func_02009c0e(8, 0x1d80000, 0x980000);
        Func_02009c1e(9, 5);
        Func_02007fc0(4, 4, 0);
        return;
    }

    if (Func_02009b80((s32)&Value_0000092b) != 0) {
        Func_02007fd6(16, 0, 0);
        Func_02007fe0(4, 4, 0);
        Func_02007952(3);
        return;
    }

    if (Func_02009ba6((s32)&Value_0000092a) != 0) {
        Func_02007ffc(16, 0, 0);
        Func_02008006(4, 3, 0);
        Func_02007978(2);
        return;
    }

    if (Func_02009bcc((s32)&Value_00000929) != 0) {
        Func_02008022(16, 0, 0);
        Func_0200802c(4, 2, 0);
        Func_0200799e(1);
        return;
    }

    if (Func_02009bf2((s32)&Value_00000928) != 0) {
        Func_02008048(16, 0, 0);
        Func_02009cb2(10, 0, 0);
        Func_020079c4(0);
        return;
    }

    Func_02009cca(9, 5);

    if (Func_02009c20((s32)&Value_00000925) != 0 &&
        Func_02009c2a((s32)&Value_00000926) == 0) {
        Func_0200703e();
    }
}
