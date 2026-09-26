/* NONMATCHING: reference 60 bytes, candidate 88; 44 differing halfwords.
 * Hypothesis 2: explicit row stores avoid the initializer's unresolved memset;
 * copies emit ldmia/stmia triples plus fourth-word str, with a 64-byte frame.
 * _call_via_r3 matches the call target; full extent, frame and pool differ.
 */
#include "TYPES.H"

struct TransformRow {
    s32 value[4];
};

struct TransformMatrix {
    struct TransformRow row[3];
};

void SceneTransform_ApplyPosition(const s32 *position)
{
    struct TransformMatrix work;
    struct TransformRow row;

    row.value[0] = 0x10000;
    row.value[1] = 0;
    row.value[2] = 0;
    row.value[3] = 0;
    work.row[0] = row;
    work.row[1] = row;
    work.row[2] = row;
    work.row[2].value[1] = position[0];
    work.row[2].value[2] = position[1];
    work.row[2].value[3] = position[2];
    ((void (*)(struct TransformMatrix *))0x030002c0)(&work);
}
