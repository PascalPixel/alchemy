#include "types.h"

#define SceneState_LinkActorZeroToWork24 Func_02002080

u8 *Func_02004ba2();
extern u8 *Data_03001ee0;

/* Complete scene/entity linker through return and its sole pool word. */
void SceneState_LinkActorZeroToWork24(void)
{
    u8 *obj = Func_02004ba2(0);
    *(u8 **)(Data_03001ee0 + 24) = obj;
    obj[98] = 1;
}
