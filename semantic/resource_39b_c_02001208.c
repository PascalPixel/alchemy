#include "types.h"
















/* One-time setup for actor 0 when scene flag 0x109 has not been published. */
extern u8 * Func_020036fa();
extern s32 Func_020036d2();
extern void Func_020036fc();
extern void Func_020037c2();
extern void Func_0200377a();
extern void Func_020037aa();
extern u8 * Func_02003748();
extern void Func_020036ee();
extern void Func_0200383a();
extern void Func_0200384e();
extern void Func_02003884();
extern void Func_02003772();
extern void Func_020037e4();
extern u8 * Func_02003782();
extern void Func_02003728();
extern void Func_020038b2();
extern void Func_020038cc();
extern void Func_020037a8();
void Func_02001208(void)
{
    u8 *self = Func_020036fa(0);
    u8 *owner_record;

    if (Func_020036d2(0x109) != 0)
        return;

    Func_020036fc();
    Func_020037c2(-1, -1, -1, 0);
    self[0x55] = 0;
    Func_0200377a(0, *(s16 *)(self + 10) << 16,
                  (*(s16 *)(self + 18) << 16) - 0x10000);
    Func_020037aa(0, 15);
    self = Func_02003748(0);
    Func_020036ee(self, 0);
    Func_0200383a();
    Func_0200384e();
    *(void **)(self + 108) = (void *)0x02008cc1;
    Func_02003884(228);
    Func_02003772(0, 0x6666, 0x3333);
    Func_02003884(0, 0, 8);
    Func_020037e4(0, 0);
    self = Func_02003782(0);
    Func_02003728(self, 1);

    owner_record[9] = (owner_record[9] & (u8)~0x0c) | 4;
    owner_record = *(u8 **)(self + 80);
    Func_020038b2(0, 0, 10);
    self[0x55] = 3;
    *(void **)(self + 108) = 0;
    Func_020038cc();
    Func_020037a8();
}
