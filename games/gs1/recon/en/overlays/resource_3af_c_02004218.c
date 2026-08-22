#include "types.h"
/* resource_3af owner at 0x02004218, 60 bytes. */
void Func_0200863e();
void Func_020084b2();
void Func_020085a8();
struct Rec_3af { u8 pad00[6]; u16 f6; };
struct Rec_3af *Func_0200854e();
void Func_020084aa();
void Func_02004218(void)
{
    Func_0200863e(0x00e80000, -1, 0x02a40000, 0);
    Func_020084b2();
    Func_020085a8(0, 0x00e80000, 0x02a40000);
    Func_0200854e(0)->f6 = 0x4000;
    Func_020084aa(1);
}
