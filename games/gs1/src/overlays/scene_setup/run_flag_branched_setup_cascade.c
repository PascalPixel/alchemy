#include "types.h"

/*
 * Flag-branched scene setup for overlay resource_3b1. Each callee name
 * refers to that call site's own call word rather than to a shared runtime
 * address.
 */

extern u8 *Data_03001ebc;
extern u8 Value_0000092b;
extern u8 Value_0000092a;
extern u8 Value_00000929;
extern u8 Value_00000928;

void Func_0200a460(void);
void Func_0200a4f0();
void Func_02008892();
void Func_0200a4fc();
void Func_020088a6();
void Func_02008634();
void Func_020088b6();
void Func_0200a5ce();
void Func_0200a5e8();
void Func_0200a4ae();
void Func_0200a546();
void Func_0200a4bc();
void Func_0200a662();
void Func_0200a4c8();
void Func_0200867e();
void Func_0200a4d4();
void Func_0200868a();
void Func_0200a4e0();
void Func_0200a578();
void Func_0200a4ee();
void Func_0200a694();
void Func_0200a4fa();
void Func_020086b0();
void Func_0200a506();
void Func_020086bc();
void Func_0200a512();
void Func_0200a6b8();
void Func_0200a51e();
void Func_0200895e();

s32 Func_0200a51c();
void Func_0200a69e();
s32 Func_0200a52e();
void Func_0200a6b0();
s32 Func_0200a540();
void Func_0200a6c2();
s32 Func_0200a552();
void Func_0200a6d4();
void Func_0200a6dc();

/*
 * A flat setter cascade, one workspace-slot store, then a four-way gated
 * chain ending in an unconditional default arm. The store spells both its
 * offset and its stored value as 224 << 1 rather than folded constants.
 * The 340-byte owner at 0x02003f94 includes its trailing pool words.
 */
void FieldScene_RunFlagBranchedSetupCascade(void)
{
    Func_0200a460();
    Func_0200a4f0(9, 5);
    Func_02008892(24, 1, 0);
    Func_0200a4fc(0, 0, 0);
    Func_020088a6(17, 0, 0);
    Func_02008634(0);
    Func_020088b6(8, 1, 20);
    Func_0200a5ce(0x6666, 0xccc);
    Func_0200a5e8(0x1b80000, -1, 0xb00000, 1);
    Func_0200a4ae(20);
    Func_0200a546(9, 7);
    Func_0200a4bc(30);
    Func_0200a662(0xbc);
    Func_0200a4c8(30);
    Func_0200867e(16);
    Func_0200a4d4(0x50);
    Func_0200868a(0);
    Func_0200a4e0(0x3c);
    Func_0200a578(9, 7);
    Func_0200a4ee(30);
    Func_0200a694(0xbc);
    Func_0200a4fa(30);
    Func_020086b0(16);
    Func_0200a506(0x50);
    Func_020086bc(0);
    Func_0200a512(0x5a);
    Func_0200a6b8(0xbc);
    Func_0200a51e(30);

    *(u32 *)(Data_03001ebc + (224 << 1)) = (224 << 1) + 67;

    Func_0200895e(9, 0, 0);

    if (Func_0200a51c((s32)&Value_0000092b) != 0) {
        Func_0200a69e(20);
    } else if (Func_0200a52e((s32)&Value_0000092a) != 0) {
        Func_0200a6b0(18);
    } else if (Func_0200a540((s32)&Value_00000929) != 0) {
        Func_0200a6c2(17);
    } else if (Func_0200a552((s32)&Value_00000928) != 0) {
        Func_0200a6d4(16);
    } else {
        Func_0200a6dc(13);
    }
}
