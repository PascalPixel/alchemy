#include "types.h"

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Scene selection chain of the resource_3b1 overlay: tests scene ids in turn and runs the matching call sequence. */

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
void Func_02009a92();
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

void RunSceneSelectionChain(void)
{
    Func_02009a92(1);
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
