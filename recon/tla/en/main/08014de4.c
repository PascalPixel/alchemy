#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "TRANSFORM.H"

void Render_ResetTransformState(void)
{
    gTransformStackTop = Runtime_AllocateBlock(8, sizeof(gTransform));
    gTransformStackDepth = 0;
    CAMELOT_ASM("the fixed-register identity store of TRANSFORM.H")
    Transform_SetIdentity(gTransform);
}
