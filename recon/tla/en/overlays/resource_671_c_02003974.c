/* Draft, differ 144 of 115, written from the listing (the cell grid of s48a4.c). As goto loops the
 * body has the native's shape, but col is kept in r4 and saved around the flag test, where the
 * native gives row and col r10 and r8; as for loops loop.c hoists row * 11 and strength-reduces the
 * cell address (differ 170), with (void)&row and (void)&col too (161). The native rebuilds
 * row * 11 + col in every branch and shares one call between the first two. */
#include "types.h"

struct PuzzleCell {
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState {
    struct PuzzleCell cells[88];
    s32 count;
    s16 firstId;
    s16 endId;
    u16 tileEntry;
    s8 redrawA;
    s8 redrawB;
    s16 originX;
    s16 originZ;
};

extern struct SceneState gSceneState;
s32 GameFlag_Test(s32 flag);
void Func_0200566c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003974(void)
{
    struct SceneState *state = &gSceneState;
    u32 row;
    u32 col;

    row = 0;
rows:
    col = 0;
cols:
    {
        struct PuzzleCell *cell = &state->cells[row * 11 + col];
        s32 occupied = cell->occupied;

        if (GameFlag_Test(0x8fb) && cell->occupied == 2) {
            cell->occupied = 1;
            occupied = 1;
        }
        if (occupied == 2)
            Func_0200566c(0, 11, 1, 1, state->originX + col, state->originZ + row);
        else if (occupied == 1)
            Func_0200566c(1, state->cells[row * 11 + col].kind, 1, 1, state->originX + col, state->originZ + row);
        else
            Func_0200566c(occupied, state->cells[row * 11 + col].kind, 1, 1, state->originX + col, state->originZ + row);
    }
    if (++col <= 10)
        goto cols;
    if (++row <= 7)
        goto rows;
}
