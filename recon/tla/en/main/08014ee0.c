#include "types.h"
#include "system.h"
#include "transform.h"

void SceneTransform_ResetMatrix(void)
{
    CAMELOT_ASM("the fixed-register identity store of transform.h")
    Transform_SetIdentity(gTransform);
}
