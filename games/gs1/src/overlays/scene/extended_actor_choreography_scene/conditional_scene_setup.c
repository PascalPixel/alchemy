#include "types.h"

#define SceneState_RunFlagGatedSetupCascade Func_0200351c
#define RunSceneSelectionChain Func_0200366c
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
void Func_02006e2a();
void Func_0200703e();
void Func_02007952();
void Func_02007978();
void Func_0200799e();
void Func_020079c4();
void Func_02007f2a();
void Func_02007f3a();
void Func_02007f72();
void Func_02007fc0();
void Func_02007fd6();
void Func_02007fe0();
void Func_02007ffc();
void Func_02008006();
void Func_02008022();
void Func_0200802c();
void Func_02008048();
void Func_02009a92_a();
s32 Func_02009b1c();
s32 Func_02009b54();
s32 Func_02009b80();
s32 Func_02009ba6();
s32 Func_02009bcc();
s32 Func_02009bf2();
void Func_02009c0e();
void Func_02009c1e();
s32 Func_02009c20();
s32 Func_02009c2a();
void Func_02009cb2();
void Func_02009cca();

/*
 * Flag-gated scene setup for overlay resource_3b1. Each callee name refers
 * to that call site's own call word rather than to a shared runtime
 * address.
 */

/* Scene selection chain of the resource_3b1 overlay: tests scene ids in turn and runs the matching call sequence. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

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

void RunSceneSelectionChain(void)
{
    Func_02009a92_a(1);
    Func_02006e2a();
    if (Func_02009b1c(2366) != 0) {
        Func_02007f72(4, 4, 0);
        Func_02007f2a(8, 412, 222, 12288);
        Func_02007f3a(9, 458, 161, 32768);
    } else {
        if (Func_02009b54(2208) != 0) {
            Call3(Func_02009c0e, 8, 30932992, 9961472);
            Func_02009c1e(9, 5);
            Func_02007fc0(4, 4, 0);
        } else {
            if (Func_02009b80(2347) != 0) {
                Func_02007fd6(16, 0, 0);
                Func_02007fe0(4, 4, 0);
                Func_02007952(3);
            } else {
                if (Func_02009ba6(2346) != 0) {
                    Func_02007ffc(16, 0, 0);
                    Func_02008006(4, 3, 0);
                    Func_02007978(2);
                } else {
                    if (Func_02009bcc(2345) != 0) {
                        Func_02008022(16, 0, 0);
                        Func_0200802c(4, 2, 0);
                        Func_0200799e(1);
                    } else {
                        if (Func_02009bf2(2344) != 0) {
                            Func_02008048(16, 0, 0);
                            Func_02009cb2(10, 0, 0);
                            Func_020079c4(0);
                        } else {
                            Func_02009cca(9, 5);
                            if (Func_02009c20(2341) != 0 && Func_02009c2a(2342) == 0) {
                                Func_0200703e();
                            }
                        }
                    }
                }
            }
        }
    }
}
