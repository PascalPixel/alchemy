#include "TYPES.H"
#include "TRANSFORM.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

/* Each builds one elementary matrix on the identity and hands it to the
   IWRAM transform routine. */
void SceneTransform_ApplyPitch(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[4] = cos;
    matrix[5] = sin;
    matrix[7] = -sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyYaw(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = cos;
    matrix[2] = -sin;
    matrix[6] = sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyRoll(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[0] = cos;
    matrix[1] = sin;
    matrix[3] = -sin;
    matrix[4] = cos;
    Iwram_TransformMatrix(matrix);
}

void SceneTransform_ApplyPosition(const s32 *position)
{
    s32 matrix[12];

    /* CAMELOT_ASM: the fixed-register identity store of TRANSFORM.H */
    Transform_SetIdentity(matrix);
    matrix[9] = position[0];
    matrix[10] = position[1];
    matrix[11] = position[2];
    Iwram_TransformMatrix(matrix);
}

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
