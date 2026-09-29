/* 2026-09-29: Trig_Sin, Trig_Cos and Iwram_TransformMatrix (IWRAM_CALL.H)
 * replace the address names; alchemy permute scores 2320, from 2350. The
 * three stmia {r1-r4} fills are the wall: the store-multiple patterns are
 * ARM-only in agscc, so the fill needs a reviewed primitive. */
/* NONMATCHING: the reference fills the matrix with three four-register
 * stmia stores of {0x10000, 0, 0, 0} and reuses r1/r2 as the call arguments.
 * agscc emits at most three-register stmia in Thumb, so this needs a shared
 * fill primitive like Dma_Set, which is not admitted.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void SceneTransform_ApplyYaw(s32 angle)
{
    s32 sine = Trig_Sin(angle);
    s32 cosine = Trig_Cos(angle);
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
    Iwram_TransformMatrix(transform);
}
