#include "RESOURCE.H"
#include "TASK.H"

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

/* Remove the marker task and release its selected display slot. */
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
        *(s32 *)((u8 *)actor + 48) = scale;
        *(s32 *)((u8 *)actor + 52) = scale >> 1;
        p += 91;
        *p = zero;

        ObjectDispatch_InitFromTable6(actor);
        Object_SetMode(actor, 5);
        Engine_ObjectSetPosition(actor, x << 16, actor[3], z << 16);
    }
}

/*
* Spawn an object and run a short fixed setup sequence on it.  `no' is
 * forwarded unchanged -- it is never freshly loaded before the first call.
 * The object's fields (0x30 and 0x34 taking the fixed 0x14000/0xa000 pair,
 * 0x5b cleared, 0xc read back for the position call) are named by position
 * from call shape alone and are not verified.
 */
void SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z)
{
    u8 *obj = (u8 *)Engine_ActorLookup(no);

    if (obj == 0) {
        return;
    }

    {
        s32 scale = 0x14000;
        u8 *p = obj;
        u8 zero = 0;
        *(s32 *)(obj + 0x30) = scale;
        *(s32 *)(obj + 0x34) = scale >> 1;
        p += 0x5b;
        *p = zero;
    }

    ObjectDispatch_InitFromTable6();
    Object_SetMode(obj, 5);
    Engine_ObjectSetPosition(obj, x << 16, *(s32 *)(obj + 12), z << 16);
    Object_CommitPosition(obj);
    Object_SetMode(obj, 1);
}
