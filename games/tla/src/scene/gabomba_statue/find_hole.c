#include "types.h"

struct PuzzleCell {
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState {
    struct PuzzleCell cells[88];
};

extern struct SceneState gSceneState;

void Func_02004f40(u32 *x_out, u32 *z_out)
{
    u32 *row_out = z_out;
    u32 *column_out = x_out;
    u32 z;
    u32 x;

    /* FAKEMATCH: the operand boundary preserves the incoming output-pointer registers; plain unbound pointers add an entry copy and swap z_out/scene between r5/r1 (7 aligned differences). */
    __asm__("" : "+r"(row_out), "+r"(column_out));
    z = 0;
row:
    x = 0;
column:
    if (gSceneState.cells[z * 11 + x].kind == 10 && !(x == 5 && z == 3)
        && !(z == 7 && (x == 1 || x == 9))) {
        *column_out = x;
        *row_out = z;
        return;
    }
    x++;
    if (x <= 10)
        goto column;
    z++;
    if (z <= 7)
        goto row;
}
