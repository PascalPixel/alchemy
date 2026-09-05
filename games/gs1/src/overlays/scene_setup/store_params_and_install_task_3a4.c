#include "types.h"

extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;

s32 Func_020068d2();           /* per-site veneer -> Func_0808a5e0 */
s32 Func_0200664c();           /* per-site veneer -> Func_080000d0 */

void SceneState_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    Data_0200d240[0] = v0;
    Data_0200d240[1] = v1;
    Data_0200d240[2] = v2;
    Data_0200d23c = v3;

    Func_020068d2(170);
    Func_0200664c(0x0200ab59, 0xc80);
}
