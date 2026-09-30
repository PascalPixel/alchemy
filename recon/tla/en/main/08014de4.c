#include "TYPES.H"
#include "SYSTEM.H"

/* Allocates the transform stack's 48-byte slots, empties it and loads the
   identity into the current transform. */

void Render_ResetTransformState(void)
{
    gTransformStackTop = Runtime_AllocateBlock(2, sizeof(gTransform));
    gTransformStackDepth = 0;
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}
