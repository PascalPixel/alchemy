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
    /* FAKEMATCH: ⚓️ keeps the z output in its argument register */
    register u32 *row_out asm("r1") = z_out;
    /* FAKEMATCH: and the x output in its own */
    register u32 *column_out asm("r0") = x_out;
    u32 z;
    u32 x;

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
