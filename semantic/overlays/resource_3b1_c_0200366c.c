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

void Func_080000c0();
void Func_020037b4(void);
s32 Func_080770c0();
void Func_020048e8();
void Func_02004890();
void Func_0808a0f0();
void Func_0808a100();
void Func_02004254();
void Func_020038ac(void);

void Func_0200366c(void)
{
    Func_080000c0(1);
    Func_020037b4();

    if (Func_080770c0((s32)&Value_0000093e) != 0) {
        Func_020048e8(4, 4, 0);
        Func_02004890(8, 0x19c, 0xde, 0x3000);
        Func_02004890(9, 0x1ca, 0xa1, 0x8000);
        return;
    }

    if (Func_080770c0(0x8a0) != 0) {
        Func_0808a0f0(8, 0x1d80000, 0x980000);
        Func_0808a100(9, 5);
        Func_020048e8(4, 4, 0);
        return;
    }

    if (Func_080770c0((s32)&Value_0000092b) != 0) {
        Func_020048e8(16, 0, 0);
        Func_020048e8(4, 4, 0);
        Func_02004254(3);
        return;
    }

    if (Func_080770c0((s32)&Value_0000092a) != 0) {
        Func_020048e8(16, 0, 0);
        Func_020048e8(4, 3, 0);
        Func_02004254(2);
        return;
    }

    if (Func_080770c0((s32)&Value_00000929) != 0) {
        Func_020048e8(16, 0, 0);
        Func_020048e8(4, 2, 0);
        Func_02004254(1);
        return;
    }

    if (Func_080770c0((s32)&Value_00000928) != 0) {
        Func_020048e8(16, 0, 0);
        Func_0808a0f0(10, 0, 0);
        Func_02004254(0);
        return;
    }

    Func_0808a100(9, 5);

    if (Func_080770c0((s32)&Value_00000925) != 0 &&
        Func_080770c0((s32)&Value_00000926) == 0) {
        Func_020038ac();
    }
}
