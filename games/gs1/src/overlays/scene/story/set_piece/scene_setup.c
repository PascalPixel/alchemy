#include "types.h"

#define FieldScene_RunValue1528Scene Func_02000a64
#define SceneState_StoreParamsAndInstallTask Func_02002bb0
extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;

void Func_020045d8(void);
void Func_020045b8(s32, s32);
void Func_02004796(s32);
void Func_02001466(void);
void Func_020044f8(s32);
void Func_0200479c(void);
void Func_02004600(void);

s32 Func_020068d2();           /* per-site veneer -> Func_0808a5e0 */

s32 Func_0200664c();           /* per-site veneer -> Func_080000d0 */

void FieldScene_RunValue1528Scene(void)
{
    Func_020045d8();
    Func_020045b8(0x1528, 1);
    Func_02004796(125);
    Func_02001466();
    Func_020044f8(20);
    Func_0200479c();
    Func_02004600();
}

void SceneState_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    Data_0200d240[0] = v0;
    Data_0200d240[1] = v1;
    Data_0200d240[2] = v2;
    Data_0200d23c = v3;

    Func_020068d2(170);
    Func_0200664c(0x0200ab59, 0xc80);
}
