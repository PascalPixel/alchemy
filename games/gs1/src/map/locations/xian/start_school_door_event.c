#include "types.h"

s32 Func_0200684a();
s32 Func_02006854();
void Func_02006864();
void Func_02006858();
void Func_0200688c();
void Func_02006a2c();
void Func_0200684e();
void Func_020068dc(s32, s32, s32);
void Func_02006908(s32, s32, s32);
void Func_0200690c();
void Func_020068ba();
void Func_020069f0();
void Func_020068d4();

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Func_020068dc(actor, horizontal, vertical);
}

static __inline__ void SetPosition(s32 actor, s32 horizontal, s32 vertical)
{
    Func_02006908(actor, horizontal, vertical);
}

#define StartSchoolDoorEvent Func_02002484

void StartSchoolDoorEvent(void)
{
    Func_02006864();
    if (Func_0200684a(2202) == 0 && Func_02006854(2197) == 0) {
        Func_02006858(6317, 1);
        Func_0200688c();
    } else {
        Func_02006a2c(158);
        Func_0200684e(0x0200c77a, 78, 13);
        SetScale(0, 0x8000, 0x4000);
        SetPosition(0, 306, 248);
        Func_0200690c(0, 304, 216);
        Func_020068ba(20);
        Func_020069f0(4);
        Func_020068d4();
    }
}
