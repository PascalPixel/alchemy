/* Contiguous unnamed state-owner run for resource_383. */

#include "types.h"

extern s32 Func_020050fe(void);
extern void Func_020050d6(void);
extern void Func_0200516e(s32, s32);
extern void Func_020050dc(s32);
extern void Func_020051cc(s32, s32);
extern void Func_020050f8(void);

void SceneState_RunGuardedActorStep(s32 x)
{
    u8 *flag = (u8 *)Func_020050fe() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_020050d6();
    Func_0200516e(x, 1);
    Func_020050dc(2);
    Func_020051cc(x, 0);
    Func_020050f8();
    *flag = zero;
}

#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_020049ea(s32, s32, s32);
extern s32 Func_020051ea(s32, s32);
extern s32 Func_0200514a(s32, s32);
extern void Func_02005216(s32, s32);

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    Func_020049ea(x, 0, 2);
    Func_020051ea(x, 0);
    if (Func_0200514a(0, 0) != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    Func_02005216(x, 0);
}
