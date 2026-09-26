/* NONMATCHING: reference 60 bytes, candidate 60; 27 differing halfwords.
 * Hypothesis 1: typed scalar rows emit str; ordinary typed pointer call emits
 * _call_via_r2, while the reference uses r3. Frame and full pool differ.
 */
#include "TYPES.H"

struct TransformMatrix {
    s32 row[3][4];
};

void SceneTransform_ApplyPosition(const s32 *position)
{
    struct TransformMatrix work;

    work.row[0][0] = 0x10000;
    work.row[0][1] = 0;
    work.row[0][2] = 0;
    work.row[0][3] = 0;
    work.row[1][0] = 0x10000;
    work.row[1][1] = 0;
    work.row[1][2] = 0;
    work.row[1][3] = 0;
    work.row[2][0] = 0x10000;
    work.row[2][1] = 0;
    work.row[2][2] = 0;
    work.row[2][3] = 0;
    work.row[2][1] = position[0];
    work.row[2][2] = position[1];
    work.row[2][3] = position[2];
    ((void (*)(struct TransformMatrix *))0x030002c0)(&work);
}
