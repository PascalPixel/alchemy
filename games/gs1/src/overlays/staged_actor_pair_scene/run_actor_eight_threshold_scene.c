#include "types.h"

void Func_02002314();
void Func_020025fc();
void Func_02002624();
void Func_02002638();
void Func_0200263a();
s32 *Func_02002642();
void Func_0200265e();

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Func_02002638(8);
    Func_02002624();
    {
        s32 *actor = Func_02002642(8);

        if ((actor[2] >> 20) <= 30) {
            Func_02002314(8);
            {
                s32 x = 27;
                s32 y = 19;

                Func_020025fc(29, 19, 1, 1, x, y);
            }
            Func_0200263a(2466);
        }
    }
    Func_0200265e();
}
