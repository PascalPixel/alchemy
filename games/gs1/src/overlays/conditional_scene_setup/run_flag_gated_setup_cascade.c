#include "types.h"

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

extern u8 Value_0000093e;
extern u8 Value_00000928;
extern u8 Value_00000925;
extern u8 Value_00000911;
extern u8 Value_00000922;

s32 Func_020099c8();
void Func_02009a7e();
void Func_02009a88();
void Func_02009a92();
void Func_02009a9c();
void Func_02009aa6();
void Func_02007efa();

s32 Func_02009a0a();
void Func_02007e0e();
void Func_02009a86();
void Func_02007e32();
void Func_02007e40();
void Func_02007e4c();
void Func_02007e58();
void Func_02007e64();
void Func_02007e74();
void Func_02009aec();

s32 Func_02009a94();
void Func_02008600();
s32 Func_02009aa4();

s32 Func_02009aba();
s32 Func_02009ac4();
void Func_02007f1a();
void Func_02009b84();

/*
 * A four-way gated cascade. Each gate either runs its own setter sequence
 * and returns or falls through to the next, and all four share one exit.
 * The 336-byte owner at 0x0200351c includes its eight trailing pool words.
 */
void SceneState_RunFlagGatedSetupCascade(void)
{
    if (Func_020099c8((s32)&Value_0000093e) != 0) {
        Func_02009a7e(8, 0, 0);
        Func_02009a88(9, 0, 0);
        Func_02009a92(10, 0, 0);
        Func_02009a9c(11, 0, 0);
        Func_02009aa6(12, 0, 0);
        Func_02007efa(14, 0, 0);
        return;
    }

    if (Func_02009a0a(0x8a0) != 0) {
        Func_02007e0e(8, 0x98, 0x1bc, 0x3000);
        Func_02009a86(8, 0x0200e958);
        Func_02007e32(10, 0xb8, 0x1e0, 0xb000);
        Func_02007e40(12, 0xaa, 0x1e8, 0xb000);
        Func_02007e4c(13, 0x88, 0x1e8, 0xd000);
        Func_02007e58(15, 0x78, 0x1e0, 0xd000);
        Func_02007e64(14, 0xb8, 0x20e, 0xb000);
        Func_02007e74(11, 0x88, 0x248, 0x8000);
        Func_02009aec(11, 0x0200e840);
        return;
    }

    {
        s32 t = Func_02009a94((s32)&Value_00000928);
        if (t != 0) {
            Func_02008600(t);
            return;
        }
    }

    if (Func_02009aa4((s32)&Value_00000925) != 0) {
        Func_02007efa(18, 0, 0);
        return;
    }

    if (Func_02009aba((s32)&Value_00000911) != 0 &&
        Func_02009ac4((s32)&Value_00000922) != 0) {
        Func_02007f1a(14, 0, 0);
        Func_02009b84(12, 0, 0);
    }
}
