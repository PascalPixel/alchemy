/* Contiguous unnamed leaf-owner run for resource_3ba. */

#include "types.h"

extern s32 Func_020043b8(void);
extern void Func_02004708(void);
extern void Func_02000b9e(void);

#define FieldScene_RunBranchedStep Func_02000954

void FieldScene_RunBranchedStep(void)
{
    if (Func_020043b8() == 0) {
        Func_02004708();
    } else {
        Func_02000b9e();
    }
}

#include "types.h"

#define SceneData_GetTablec420 Func_0200096c

extern u8 Data_0200c420[];

u8 *SceneData_GetTablec420(void)
{
    return Data_0200c420;
}
