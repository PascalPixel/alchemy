#include "RUNTIME_MEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "TRANSFORM.H"
#include "DMA.H"
#include "IO_REG.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"

void SceneTransform_ApplyPitch(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    CAMELOT_ASM("the fixed-register identity store of TRANSFORM.H")
    Transform_SetIdentity(matrix);
    matrix[4] = cos;
    matrix[5] = sin;
    matrix[7] = -sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}
