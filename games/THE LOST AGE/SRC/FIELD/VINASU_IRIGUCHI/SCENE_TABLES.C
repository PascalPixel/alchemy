#include "PROTO.H"
#include "GLOBAL.H"
#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "OBJECT_RUNTIME.H"

struct ObjectRuntime *Object_GetById(u32 id);
void Battle_WaitMode0(s32 frames);
void Engine_AudioPlayCue(s32 cue);


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

void VinasuIriguchi_RunActorSparkBurst(void *object)
{
    Func_02002a34(8, 0x4084);
    Func_02002a44();
    Func_02002a5c(Object_GetById(5), 2);
    Battle_WaitMode0(20);
    Func_020023a4(object);
    Func_02002a5c(Object_GetById(5), 0);
    Func_02002a54();
    Func_02002a4c();
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    Field_EndActorSpriteEffect(8);
#else
    Field_BeginPaletteTransition(8);
#endif
}

void VinasuIriguchi_UpdateSparkTrail(struct ObjectRuntime *object)
{
    Func_0200260c(object, object->x, object->y, object->z);
    if (++object->action % 8 == 0)
        Engine_AudioPlayCue(125);
}

