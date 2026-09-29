/* 2026-09-29: Trig_Sin, Trig_Cos and Iwram_TransformMatrix replace the
 * address names and the function-pointer cast the permuter could not parse;
 * alchemy permute scores 1740. The stmia {r1-r4} fill (with the sine parked
 * in ip) is the same wall as 08004c1c. */
/* NONMATCHING: 72 bytes, candidate 72, 18 differing halfwords.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The reference initializes the 12-word identity matrix with three multiple-register stores; scalar C selects a different store sequence.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"


s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void SceneTransform_ApplyRoll(s32 angle)
{
    s32 sine;
    s32 cosine;
    s32 matrix[12];

    sine = Trig_Sin(angle);
    cosine = Trig_Cos(angle);
    matrix[0] = 0x10000;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0x10000;
    matrix[5] = 0;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0x10000;
    matrix[9] = 0;
    matrix[10] = 0;
    matrix[11] = 0;
    matrix[0] = cosine;
    matrix[1] = sine;
    matrix[3] = -sine;
    matrix[4] = cosine;
    Iwram_TransformMatrix(matrix);
}
