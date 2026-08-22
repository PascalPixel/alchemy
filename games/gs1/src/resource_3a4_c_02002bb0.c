#include "types.h"

extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;

s32 Func_020068d2();           /* per-site veneer -> Func_0808a5e0 */
s32 Func_0200664c();           /* per-site veneer -> Func_080000d0 */

void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    Data_0200d240[0] = arg0;
    Data_0200d240[1] = arg1;
    Data_0200d240[2] = arg2;
    Data_0200d23c = arg3;

    Func_020068d2(170);
    Func_0200664c(0x0200ab59, 0xc80);
}
