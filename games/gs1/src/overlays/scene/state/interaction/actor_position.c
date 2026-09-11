#include "types.h"

#define GetActorPosition Func_0200146a
#define IsActor9AtTile15x54 Func_02000334
#define SceneActor_IsActor10AtTile16x12 Func_02000458
#define SceneActor_TransformAndApplyRecordPosition Func_02000b54

s32 *Func_0200146a(s32 actor);
s32 *Func_0200158e(s32);
void Func_02001bfc(s32, s32, s32 *);
void Func_02001c38(s32 *, s32, s32, s32);

s32 IsActor9AtTile15x54(void)
{
    s32 *actor = GetActorPosition(9);
    s32 z = actor[4];
    s32 x;
    s32 z_tile;
    s32 x_tile;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = actor[2];
    z_tile = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    x_tile = x >> 20;
    if (x_tile == 15 && z_tile == 54) {
        return 1;
    }
    return 0;
}

s32 SceneActor_IsActor10AtTile16x12(void)
{
    s32 *p = Func_0200158e(10);
    s32 z = p[4];
    s32 x;
    s32 cz;
    s32 cx;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = p[2];
    cz = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    cx = x >> 20;
    if (cx == 16 && cz == 12) {
        return 1;
    }
    return 0;
}

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    Func_02001bfc(v0, v1, pos);
    Func_02001c38(rec, pos[0], pos[1], pos[2]);
}
