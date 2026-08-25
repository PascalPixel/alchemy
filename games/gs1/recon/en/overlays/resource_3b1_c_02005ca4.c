#include "types.h"

/* Complete C reconstruction of the map event at 0x02005ca4. */

void Func_0200a554();
void Func_0200a59a();
void Func_0200a5b6();
void Func_0200a5f2();
void Func_0200c170();
void Func_0200c19c();
void Func_0200c1c0();
void Func_0200c256();
void Func_0200c25a();
void Func_0200c2b8(s32, s32, s32);
void Func_0200c2c2();

static __inline__ void SetDirection(s32 actor, s32 direction, s32 duration)
{
    Func_0200c2b8(actor, direction, duration);
}

#define RunActorsEightAndNineMapEvent Func_02005ca4

void RunActorsEightAndNineMapEvent(void)
{
    Func_0200c170();
    Func_0200a59a(15, 1, 0);
    Func_0200a554(9, 468, 616, 32768);
    Func_0200a5b6(8, 1, 20);
    Func_0200c256(9, 2);
    Func_0200c19c(20);
    SetDirection(8, 53248, 80);
    Func_0200c2c2(8, 0, 20);
    Func_0200c25a(8, 3);
    Func_0200c1c0(20);
    Func_0200a5f2(9, 21, 0);
}
