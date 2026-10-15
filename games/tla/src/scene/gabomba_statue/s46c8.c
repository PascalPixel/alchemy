#include "types.h"

struct PuzzleCell {
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState {
    struct PuzzleCell cells[88];
    u8 unknown_160[4];
    s16 first_id;
    s16 end_id;
    u16 tile_entry;
    s8 redraw_a;
    s8 redraw_b;
    s16 origin_x;
    s16 origin_z;
};

extern struct SceneState gSceneState;

void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200567c(a, b, c, d, e, f);
}

void Func_020046c8(void)
{
    if (gSceneState.redraw_b != 0) {
        Map_Copy(0, 12, 1, 1, gSceneState.origin_x + 7, gSceneState.origin_z - 2);
        gSceneState.redraw_b = 0;
    }
}
