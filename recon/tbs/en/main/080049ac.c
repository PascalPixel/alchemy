/* NONMATCHING: reference 60 bytes, candidate 64; 28 differing halfwords.
 * Hypothesis 1: typed scalar rows emit twelve str stores, not three stmia;
 * adjacent absolute cells merge into one base plus 104. Full pool compared.
 */
#include "TYPES.H"

struct TransformMatrix {
    s32 row[3][4];
};

void *Runtime_AllocateBlock(s32 kind, s32 size);

void Render_ResetTransformState(void)
{
    struct TransformMatrix *work = (struct TransformMatrix *)0x03000350;
    void *buf = Runtime_AllocateBlock(2, sizeof(*work));

    *(s32 *)0x03001cc4 = 0;
    *(void **)0x03001d2c = buf;
    work->row[0][0] = 0x10000;
    work->row[0][1] = 0;
    work->row[0][2] = 0;
    work->row[0][3] = 0;
    work->row[1][0] = 0x10000;
    work->row[1][1] = 0;
    work->row[1][2] = 0;
    work->row[1][3] = 0;
    work->row[2][0] = 0x10000;
    work->row[2][1] = 0;
    work->row[2][2] = 0;
    work->row[2][3] = 0;
}
