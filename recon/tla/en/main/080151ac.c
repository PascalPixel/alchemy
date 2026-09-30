#include "TYPES.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* Each builds one elementary matrix on the identity and hands it to the
   IWRAM transform routine. */

void SceneTransform_ApplyScale(const s32 *scale)
{
    s32 matrix[12];

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = scale[0];
    matrix[4] = scale[1];
    matrix[8] = scale[2];
    Iwram_TransformMatrix(matrix);
}
