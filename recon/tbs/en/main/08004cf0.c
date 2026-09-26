/* NONMATCHING: 60 bytes, candidate 56, 26 differing halfwords.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The reference initializes the 12-word identity matrix with three multiple-register stores; scalar C selects a different store sequence.
 */
#include "TYPES.H"


void SceneTransform_ApplyScale(const s32 *scale)
{
    s32 matrix[12];

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
    matrix[0] = scale[0];
    matrix[4] = scale[1];
    matrix[8] = scale[2];
    ((void (*)(s32 *))0x030002c0)(matrix);
}
