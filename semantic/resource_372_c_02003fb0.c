/* Linear actor-22 scene and camera setup. */
#include "types.h"

  
  
  
  
  
  

extern void Func_020086b0();
extern void Func_020087ea();
extern void Func_02008720();
extern void Func_0200861e();
extern void Func_0200875c();
extern void Func_02008776();
extern void Func_020087f2();
extern u8 * Func_02008728();
extern void Func_020087a0();
extern void Func_02008656();
extern void Func_020087f4();
extern void Func_0200880c();
extern void Func_020087c0();
extern void Func_02008676();
extern void Func_02008862();
extern void Func_02008878();
extern void Func_02008884();
extern void Func_02008752();
extern void Func_02008802();
extern void Func_0200885c();
extern void Func_02008838();
extern void Func_0200887a();
extern void Func_02008896();
extern void Func_0200883e();
extern void Func_020087ec();
extern void Func_02008836();
extern void Func_02008842();
extern void Func_020087b0();
void Func_02003fb0(void)
{
    u8 *actor;
    Func_020086b0();
    Func_020087ea(-1, -1, -1, 0);
    Func_02008720(22);
    Func_0200861e((void *)0x0200c5b9);
    Func_0200875c(0, 0x1e0, 0x570);
    Func_02008776(0, 0, 0);
    Func_020087f2(22, 0x3000, 20);
    actor[35] |= 1;
    actor = Func_02008728(22);
    Func_020087a0(22, 0x00f90000, 0x04d80000);
    Func_02008656(1);
    Func_020087f4(0xed3);
    Func_0200880c(0x1016, 0);
    Func_020087c0(22, 0x00ac0000, 0x04fe0000);
    Func_02008676(1);
    Func_02008862(0x40000, 0x8000);
    Func_02008878(0x00a20000, 0, 0x05050000, 1);
    Func_02008884();
    Func_02008752(40);
    Func_02008802(22, 4);
    Func_0200885c(0x1016, 0, 10);
    Func_02008878(22, 0xc000, 20);
    Func_02008838(22, 2);
    Func_0200887a(0x1016, 0, 10);
    Func_02008896(22, 0x1000, 20);
    Func_0200883e(22, 3);
    Func_020087ec(22, 0x20000, 0x10000);
    Func_02008836(22, 0xa5, 0x514);
    Func_02008842(22, 0xc3, 0x598);
    Func_020087b0(0x842);
}
