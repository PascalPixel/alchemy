#include "types.h"

void Func_02001f8a();
void Func_02001fa4();
u8 *Func_02001fc4();
void Func_02001fd0();
void Func_02001fe2();
void Func_02002006();

#define ConfigureSceneForActorEightColumn Func_02000b6c
void ConfigureSceneForActorEightColumn(void)
{
    u8 *actor;
    s32 x;
    s32 a4;
    s32 a5;

    actor = Func_02001fc4(8);
    x = *(s32 *)(actor + 8);
    if (x < 0)
        x += 0xfffff;
    x >>= 20;
    Func_02001fc4();
    if (x == 20) {
        a4 = 18;
        a5 = 6;
        Func_02001f8a(18, 40, 6, 3, a4, a5);
        Func_02001fd0(0x302);
    } else {
        a4 = 18;
        a5 = 6;
        Func_02001fa4(24, 40, 6, 3, a4, a5);
        Func_02001fe2(0x302);
    }
    Func_02002006();
}
