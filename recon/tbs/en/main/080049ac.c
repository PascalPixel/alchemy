/* NONMATCHING: reference 60 bytes, candidate 92; 42 differing halfwords.
 * Hypothesis 2: explicit row stores avoid the initializer's unresolved memset;
 * copies emit ldmia/stmia triples plus fourth-word str, with a 16-byte frame.
 * Full extent/pool differ; separate globals keep all three address literals.
 */
#include "TYPES.H"

struct TransformRow {
    s32 value[4];
};

struct TransformMatrix {
    struct TransformRow row[3];
};

void *Runtime_AllocateBlock(s32 kind, s32 size);
extern s32 Data_03001cc4;
extern void *Data_03001d2c;
extern struct TransformMatrix Data_03000350;

void Render_ResetTransformState(void)
{
    struct TransformRow row;
    void *buf = Runtime_AllocateBlock(2, sizeof(Data_03000350));

    row.value[0] = 0x10000;
    row.value[1] = 0;
    row.value[2] = 0;
    row.value[3] = 0;
    Data_03001cc4 = 0;
    Data_03001d2c = buf;
    Data_03000350.row[0] = row;
    Data_03000350.row[1] = row;
    Data_03000350.row[2] = row;
}
