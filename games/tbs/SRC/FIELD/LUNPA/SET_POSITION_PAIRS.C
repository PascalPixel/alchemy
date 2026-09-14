#include "TYPES.H"

#define FieldScene_SetPositionPairs Func_02000d54
#define ScenePositionTable Data_0200f73c

extern s32 Data_0200f73c[];
extern void Func_0200d5d0(s32, s32, s32, s32, s32, s32);
extern void Func_0200d5d8(s32, s32, s32, s32, s32, s32);

static __inline__ void Scene_Call6(
    void (*func)(s32, s32, s32, s32, s32, s32),
    s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    func(a0, a1, a2, a3, a4, a5);
}

void FieldScene_SetPositionPairs(s32 idx)
{
    s32 y;
    s32 x;
    s32 bottom;

    x = ScenePositionTable[idx * 2];
    y = ScenePositionTable[idx * 2 + 1];
    Scene_Call6(Func_0200d5d0, 0, 77, 1, 3, x, y);
    Scene_Call6(Func_0200d5d0, 1, 77, 1, 1, x + 1, y);
    bottom = y - 44;
    Scene_Call6(Func_0200d5d8, x, y - 45, 1, 1, x, bottom);
    if (idx == 1)
        Scene_Call6(Func_0200d5d8, x, bottom, 1, 1, x, y - 43);
}
