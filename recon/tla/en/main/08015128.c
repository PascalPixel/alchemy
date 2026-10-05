#include "TYPES.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void SceneTransform_ApplyPosition(const s32 *position)
{
    s32 matrix[12];

    CAMELOT_ASM("the fixed-register identity store of TRANSFORM.H")
    Transform_SetIdentity(matrix);
    matrix[9] = position[0];
    matrix[10] = position[1];
    matrix[11] = position[2];
    Iwram_TransformMatrix(matrix);
}
