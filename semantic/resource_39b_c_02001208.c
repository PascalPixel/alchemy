#include "types.h"

extern u8 *Func_0808a080();
extern s32 Func_080770c0();
extern void Func_0808a018();
extern void Func_0808a210();
extern void Func_0808a0f0();
extern void Func_0808a158();
extern void Func_080091e0();
extern void Func_0808a360();
extern void Func_0808a370();
extern void Func_080f9010();
extern void Func_0808a090();
extern void Func_0808a580();
extern void Func_0808a5e8();
extern void Func_0808a020();

/* One-time setup for actor 0 when scene flag 0x109 has not been published. */
void Func_02001208(void)
{
    u8 *self = Func_0808a080(0);
    u8 *owner_record;

    if (Func_080770c0(0x109) != 0)
        return;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    self[0x55] = 0;
    Func_0808a0f0(0, *(s16 *)(self + 10) << 16,
                  (*(s16 *)(self + 18) << 16) - 0x10000);
    Func_0808a158(0, 15);
    self = Func_0808a080(0);
    Func_080091e0(self, 0);
    Func_0808a360();
    Func_0808a370();
    *(void **)(self + 108) = (void *)0x02008cc1;
    Func_080f9010(228);
    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a580(0, 0, 8);
    Func_0808a158(0, 0);
    self = Func_0808a080(0);
    Func_080091e0(self, 1);

    owner_record[9] = (owner_record[9] & (u8)~0x0c) | 4;
    owner_record = *(u8 **)(self + 80);
    Func_0808a580(0, 0, 10);
    self[0x55] = 3;
    *(void **)(self + 108) = 0;
    Func_0808a5e8();
    Func_0808a020();
}
