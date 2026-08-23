#include "types.h"

/* Runs the Xian school-door event once its two story flags are available. */

s32 Func_0200684a();
s32 Func_02006854();
void Func_02006864();
void Func_02006858();
void Func_0200688c();
void Func_02006a2c();
void Func_0200684e();
void Func_020068dc();
void Func_02006908();
void Func_0200690c();
void Func_020068ba();
void Func_020069f0();
void Func_020068d4();

#define StartXianSchoolDoorEvent Func_02002484

void StartXianSchoolDoorEvent(void)
{
    Func_02006864();
    if (Func_0200684a(2202) == 0 && Func_02006854(2197) == 0) {
        Func_02006858(6317, 1);
        Func_0200688c();
    } else {
        Func_02006a2c(158);
        Func_0200684e(0x0200c77a, 78, 13);
        {
            register s32 horizontal_scale = 128;
            register s32 vertical_scale = 128;
            register s32 actor = 0;
            horizontal_scale <<= 8;
            vertical_scale <<= 7;
            Func_020068dc(actor, horizontal_scale, vertical_scale);
        }
        {
            register s32 horizontal_position = 153;
            register s32 actor = 0;
            horizontal_position <<= 1;
            Func_02006908(actor, horizontal_position, 248);
        }
        Func_0200690c(0, 304, 216);
        Func_020068ba(20);
        Func_020069f0(4);
        Func_020068d4();
    }
}
