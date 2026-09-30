#include "TYPES.H"

void SceneTransform_ResetMatrix(void)
{
    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(gTransform);
}
