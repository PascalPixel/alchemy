#include "types.h"

struct PuzzleCell
{
    u8 kind;
    u8 blocked;
    u8 state;
    u8 occupied;
};

struct SceneState
{
    struct PuzzleCell cells[88];
    u8                unk160[4];
    s16               firstId;
    s16               endId;
    u16               tileEntry;
    s8                redrawA;
    s8                redrawB;
    s16               originX;
    s16               originZ;
};

extern struct SceneState gSceneState;

void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200567c(a, b, c, d, e, f);
}

void Func_020045d8(void)
{
    if (gSceneState.redrawA != 0)
    {
        Map_Copy(1, 12, 1, 1, gSceneState.originX + 3, gSceneState.originZ - 2);
        gSceneState.redrawA = 0;
    }
}
