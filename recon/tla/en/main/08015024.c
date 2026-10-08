#include "mem.h"
#include "edition.h"
#include "types.h"
#include "transform.h"
#include "dma.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "system.h"
#include "iwram_call.h"

void SceneTransform_ApplyPitch(s32 angle)
{
    s32 matrix[12];
    s32 sin = Trig_Sin(angle);
    s32 cos = Trig_Cos(angle);

    CAMELOT_ASM("the fixed-register identity store of transform.h")
    Transform_SetIdentity(matrix);
    matrix[4] = cos;
    matrix[5] = sin;
    matrix[7] = -sin;
    matrix[8] = cos;
    Iwram_TransformMatrix(matrix);
}
