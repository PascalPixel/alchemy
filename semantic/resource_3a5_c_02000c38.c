#include "types.h"





/* Put actor 8 and actor 53 into their scene-91 presentation variants. */
extern void Func_02002a04();
extern void Func_02002a54();
extern void Func_02002a5e();
void Func_02000c38(void)
{
    Func_02002a04(8, 2);
    Func_02002a54(91, 5);
    *(u8 *)(0x02000240 + 0x22b) = 3;
    Func_02002a5e(53, 5);
}
