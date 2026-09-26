/* NONMATCHING: 72 bytes, candidate 72, 18 differing halfwords.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The reference initializes the 12-word identity matrix with three multiple-register stores; scalar C selects a different store sequence.
 */
#include "TYPES.H"


s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);

void SceneTransform_ApplyRoll(s32 angle)
{
    s32 sine;
    s32 cosine;
    s32 matrix[12];

    sine = Func_08002322(angle);
    cosine = Func_0800231c(angle);
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
    ((void (*)(s32 *))0x030002c0)(matrix);
}
