#include "types.h"

void Func_02002464();
void Func_0200247e();
void *Func_020024c4();
void Func_020024c6();
void Func_020024cc();
void Func_020024d0();
void Func_020024e6();
void Func_020024e8();
void Func_020024f0();
void Func_020024f6();
void Func_02002504();
void Func_02002506();
void Func_02002516();
void Func_0200251e();

#define ActorPresentation_RunActorElevenRecoveryScene Func_02000f44

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Func_02002464();
    Func_020024c6(11, 0, 0);
    Func_020024d0(0, 11, 0);
    Func_020024d0(0, 1);
    Func_0200247e(10);
    Func_020024f0(0, 11, 0);
    Func_02002506(0x23d9);
    Func_0200251e(11, 0);
    Func_020024f6(11, 2);
    {
        s16 *position = Func_020024c4(0);

        if (position != 0)
            Func_020024e6(11, position[5], position[9]);
    }
    Func_02002504(11);
    Func_02002516(11, 0, 0);
    Func_020024cc(20);
    Func_020024c4(2464);
    Func_020024e8();
}
