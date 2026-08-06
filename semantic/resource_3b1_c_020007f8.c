#include "types.h"















extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x020007f8, 176 bytes including alignment and its
 * four-word pool. Flag 0x925 selects the actor-8 answer sequence; its answer
 * branch either performs the movement or advances the shared scene counter.
 * The sole return is at 0x02000894, and all 17 calls appear in machine order.
 */
extern void Func_02006cc4();
extern int Func_02006ca2();
extern void Func_02006d86();
extern void Func_02006dbc();
extern void Func_02005086();
extern void Func_02006dac();
extern void Func_02006ddc();
extern int Func_02006d14();
extern void Func_02006cfe();
extern void Func_020050b0();
extern void Func_020050ce();
extern void Func_02006e0e();
extern void Func_02006e28();
extern void Func_02006e4c();
extern void Func_02006e2c();
extern void Func_02006e44();
extern void Func_02006d60();
void Func_020007f8(void)
{
    Func_02006cc4();
    if (Func_02006ca2(0x925) != 0) {
        Func_02006d86(8, 2);
        Func_02006dbc(0x1e13);
        Func_02005086(8);
        Func_02006dac(8, 0, 10);
        Func_02006ddc(8, 0);
        if (Func_02006d14(0, 0) == 0) {
            Func_02006cfe(40);
            Func_020050b0(8);
            Func_020050ce(8, 0x3000);
            Func_02006e0e(8, 0);
        } else {
            *(u16 *)(Data_03001ebc + 472) += 2;
            Func_02006e28(8, 0);
            Func_02006e4c(8, 0x3000, 0);
        }
    } else {
        Func_02006e2c(0x1d4e);
        Func_02006e44(8, 0);
    }
    Func_02006d60();
}
