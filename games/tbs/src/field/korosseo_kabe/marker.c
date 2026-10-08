#include "resource.h"
#include "task.h"

void SceneEffect_SeedMarkerAndInstallTask(u32 x, u32 y, u32 style)
{
    SceneState_InitCursorWhenUnset(x, y, style);

    Korosseo_MarkerX = (u16)x;
    Korosseo_MarkerY = (u16)y;
    Korosseo_MarkerPriority = (u16)(style & 3);
    Korosseo_MarkerBlink = 0;
    Korosseo_MarkerSteps = 0;

    {
        s32 budget = 0xc80;
        Engine_TaskAddCallback(Korosseo_UpdateMarker, budget);
    }
}

void SceneState_StartMarkerMove(u32 x, u32 y, u32 duration)
{
    Korosseo_MarkerEndX = (u16)x;
    Korosseo_MarkerEndY = (u16)y;
    Korosseo_MarkerStartX = Korosseo_MarkerX;
    Korosseo_MarkerStartY = Korosseo_MarkerY;
    Korosseo_MarkerSteps = (u16)duration;
    Korosseo_MarkerStep = 0;

    {
        s32 frame_budget = 0xc80;
        Engine_TaskAddCallback(Korosseo_UpdateMarker, frame_budget);
    }
}

void SceneEffect_RemoveMarkerTask(void)
{
    Engine_TaskRemoveCallback(Korosseo_UpdateMarker);
    Resource_ResetEntry(Korosseo_MarkerSlot);
    Korosseo_MarkerSlot = -1;
}

void SceneActor_PlaceWithScale20000(s32 selector, s32 x, s32 z)
{
    s32 *actor = (s32 *)Engine_ActorLookup(selector);

    if (actor != 0) {
        s32 scale = 0x20000;
        u8 *p = (u8 *)actor;
        u8 zero = 0;
        ((struct FieldActor *)actor)->speed = scale;
        ((struct FieldActor *)actor)->acceleration = scale >> 1;
        p = &((struct FieldActor *)actor)->paused;
        *p = zero;

        ObjectDispatch_InitFromTable6(actor);
        Object_SetMode(actor, 5);
        Engine_ObjectSetPosition(actor, x << 16, ((struct FieldActor *)actor)->y.fixed, z << 16);
    }
}

void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    struct FieldActor *obj = Engine_ActorLookup(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        obj->speed = scale;
        obj->acceleration = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    ObjectDispatch_InitFromTable6();
    Object_SetMode(obj, 5);
    Engine_ObjectSetPosition(obj, x << 16, obj->y.fixed, z << 16);
    Object_CommitPosition(obj);
    Object_SetMode(obj, 1);
}
