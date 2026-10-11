#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "object_runtime.h"
#include "call.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020001e0(s32 x, s32 y, s32 z, s32 count);
void Func_0200029c(void);
void Func_02000358(void);
void Func_020005a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020005b0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020005c8(s32 id, s32 x, s32 z);
void Func_020005d8(s32 a, s32 b);


const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gScenePlacements;
}

void Func_02000054(void)
{
    Func_020005c8(8, 0, 0);
}

void Func_02000064(s32 mode)
{
    if (mode == 1)
        Func_020005c8(8, 0, 0);
}

void Func_02000078(void)
{
}

void Func_0200007c(void)
{
    Object_SetModeById(9, 3);
}

void Func_02000088(s32 mode)
{
    if (mode == 1) {
        Func_020005b0(49, 1, 1, 2, 34, mode);
        Func_020005a8(49, 1, 1, 2, 34, 3);
    }
}

void Func_020000b8(void)
{
    if (!Value1((s32 (*)())GameFlag_Test, 0x220)) {
        GameFlag_SetBit(0x220);
        Func_020005d8(14, 0);
    }
}

void Func_020000d8(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_0200029c();
    Func_020001e0(object->x, object->y, object->z, 20);
    WaitFrames(20);
    Func_02000358();
}
