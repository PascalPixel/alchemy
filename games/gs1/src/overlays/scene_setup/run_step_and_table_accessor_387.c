/* Contiguous unnamed leaf-owner run for resource_387. */

#include "types.h"

#define SceneData_GetTable9488 Func_020004ec

#define FieldScene_RunStepWithValue866 Func_020004d4

/* resource_387 three-call story-flag wrapper at 0x020004d4. */

extern void Func_02001604(void);
extern void Func_020015ea(s32 flag);
extern void Func_02001616(void);
void FieldScene_RunStepWithValue866(void)
{
    Func_02001604();
    Func_020015ea(0x866);
    Func_02001616();
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9488(void)
{
    return (void *)0x02009488;
}
