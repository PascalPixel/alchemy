/* NONMATCHING: 28 bytes, candidate 40, 20 differing halfwords, 15 halfword edits.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The reference initializes the 12-word identity matrix with three multiple-register stores; scalar C selects separate stores.
 */
#include "TYPES.H"

struct MatrixBlock {
    s32 value;
    s32 zero1;
    s32 zero2;
    s32 zero3;
};

void SceneTransform_ResetMatrix(void)
{
    struct MatrixBlock *matrix = (struct MatrixBlock *)0x03000350;

    matrix[0].value = 0x10000;
    matrix[0].zero1 = 0;
    matrix[0].zero2 = 0;
    matrix[0].zero3 = 0;
    matrix[1].value = 0x10000;
    matrix[1].zero1 = 0;
    matrix[1].zero2 = 0;
    matrix[1].zero3 = 0;
    matrix[2].value = 0x10000;
    matrix[2].zero1 = 0;
    matrix[2].zero2 = 0;
    matrix[2].zero3 = 0;
}
