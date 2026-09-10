#include "types.h"

extern s16 SceneState_Table[];
s32 SceneFlag_Check(s32 flag);
void SceneFlag_Set(s32 flag);
void SceneMap_UpdateRect(s32, s32, s32, s32, s32, s32);
void SceneMap_CopyRect(s32, s32, s32, s32, s32, s32);
void *OverlayObject_CreateConfiguredObjectB(s32, s32, s32, s32);
void SceneState_SetMode(s32 mode);
u8 *SceneActor_Find(s32 actor);
void SceneActor_Place(s32 actor, s32 x, s32 z);

static __inline__ void PlaceActor(void (*place)(s32, s32, s32),
                                 s32 actor, s32 x, s32 z)
{
    place(actor, x, z);
}

static __inline__ void UpdateRect(void (*update)(s32, s32, s32, s32, s32, s32),
                                 s32 x, s32 z, s32 width, s32 height,
                                 s32 sourceX, s32 sourceZ)
{
    update(x, z, width, height, sourceX, sourceZ);
}

s32 SceneSetup_InitializeActorsAndFlags(void)
{
    u8 *actor;
    s16 *scene;
    s32 mode;

    if (SceneFlag_Check(0x200))
        UpdateRect(SceneMap_UpdateRect, 55, 26, 4, 2, 23, 26);
    OverlayObject_CreateConfiguredObjectB(0x800000, 0, 0x1a40000, 223);
    SceneMap_CopyRect(45, 41, 8, 45, 3, 3);
    SceneState_SetMode(1);
    actor = SceneActor_Find(14);
    *(u32 *)(actor + 108) = 0x02008401;
    {
        u8 *actor = SceneActor_Find(14);
        s32 mode = 1;
        *(u16 *)(actor + 100) = mode;
    }
    mode = 0;
    actor = SceneActor_Find(15);
    *(u32 *)(actor + 108) = 0x02008401;
    *(u16 *)(SceneActor_Find(15) + 100) = mode;
    if (SceneFlag_Check(0x858))
        PlaceActor(SceneActor_Place, 18, 0xd80000, 0x1880000);
    if (SceneState_Table[225] <= 2 && !SceneFlag_Check(52) && !SceneFlag_Check(0x109))
        SceneFlag_Set(0x867);
    if (SceneFlag_Check(0x867) && !SceneFlag_Check(52))
        PlaceActor(SceneActor_Place, 21, 0x1980000, 0x780000);
    scene = SceneState_Table;
    if (scene[225] == 11)
        SceneFlag_Set(0x12f);
    if (scene[225] == 13)
        SceneFlag_Set(0x120);
    return 0;
}
