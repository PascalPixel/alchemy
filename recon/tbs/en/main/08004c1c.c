/* NONMATCHING: the reference fills the matrix with three four-register
 * stmia stores of {0x10000, 0, 0, 0} and reuses r1/r2 as the call arguments.
 * agscc emits at most three-register stmia in Thumb, so this needs a shared
 * fill primitive like Dma_Set, which is not admitted.
 */
#include "TYPES.H"

#define SceneTransform_ApplyYaw Func_08004c1c

void SceneTransform_ApplyYaw(s32 angle)
{
    s32 sine = Func_08002322(angle);
    s32 cosine = Func_0800231c(angle);
    s32 transform[12];

    transform[0] = cosine;
    transform[1] = 0;
    transform[2] = -sine;
    transform[3] = 0;
    transform[4] = 0x10000;
    transform[5] = 0;
    transform[6] = sine;
    transform[7] = 0;
    transform[8] = cosine;
    transform[9] = 0;
    transform[10] = 0;
    transform[11] = 0;
    Func_080072f0(transform, 0x10000, sine, 0x030002c0);
}
