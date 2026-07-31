typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020012dc (72 bytes incl. three-word
 * pool, 9 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x020012dc, epilogue `pop {r5} / pop {r1} / bx r1` at
 * 0x02001310-0x02001314. The trailing three-word literal pool at
 * 0x02001318-0x02001324 (0x0000092b, 0x0000092a, 0x00000929) holds the
 * three low-address-variable operands taken by `&`, per the
 * `Value_XXXXXXXX` convention already used for this same address family
 * in assets/code/resource_3b1_c_02000b84.c (see its
 * `Value_0000092b`/`Value_0000092a`/`Value_00000929` declarations, same
 * `(s32)&Value_...` call-argument idiom). Pool ends exactly where the
 * next owner's `push {lr}` begins (0x02001324), so the span is
 * 0x020012dc-0x02001324, 72 bytes.
 *
 * Raw callee naming.
 */

extern u8 Value_0000092b;
extern u8 Value_0000092a;
extern u8 Value_00000929;

s32 Func_02007784();
s32 Func_02007792();
s32 Func_020077a0();
void Func_020062b6();

void Func_020012dc(void)
{
    s32 result = 0;

    if (Func_02007784((s32)&Value_0000092b) != 0) {
        result = 3;
    } else if (Func_02007792((s32)&Value_0000092a) != 0) {
        result = 2;
    } else if (Func_020077a0((s32)&Value_00000929) != 0) {
        result = 1;
    }

    Func_020062b6(result, 1);
}
