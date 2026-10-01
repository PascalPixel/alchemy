/* The palette task and positioning scaled objects. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    ColossoLogRollingStage_EnsurePaletteHandle();

    Korosseo_MarkerX = (u16)first_value;
    Korosseo_MarkerY = (u16)second_value;
    Korosseo_MarkerPriority = (u16)(mode & 3);
    Korosseo_MarkerBlink = 0;
    Korosseo_MarkerSteps = 0;

    {
        s32 budget = 0xc80;
        s32 task = (s32)Korosseo_UpdateMarker;
        Engine_TaskAddCallback(task, budget);
    }
}

void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    Korosseo_MarkerEndX = (u16)first_value;
    Korosseo_MarkerEndY = (u16)second_value;
    Korosseo_MarkerStartX = Korosseo_MarkerX;
    Korosseo_MarkerStartY = Korosseo_MarkerY;
    Korosseo_MarkerSteps = (u16)mode;
    Korosseo_MarkerStep = 0;

    {
        s32 budget = 0xc80;
        s32 task = (s32)Korosseo_UpdateMarker;
        Engine_TaskAddCallback(task, budget);
    }
}

void ColossoLogRollingStage_StopPaletteTask(void)
{
    extern void Korosseo_UpdateMarker(void);
    extern void Resource_ResetEntry(s32 slot);

    Engine_TaskRemoveCallback(Korosseo_UpdateMarker);
    Resource_ResetEntry(Korosseo_MarkerSlot);
    Korosseo_MarkerSlot = -1;
}

void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();

    ScaledStageObject *object = (ScaledStageObject *)Engine_ActorLookup(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        ObjectDispatch_InitFromTable6(object);
        Object_SetMode(object, 5);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    extern void ObjectDispatch_InitFromTable6(void);
    extern void Object_SetPosition(u8 *object, s32 x, s32 y, s32 z);
    extern void Object_CommitPosition(u8 *object);

    u8 *object = (u8 *)Engine_ActorLookup(object_id);

    if (object == 0) {
        return;
    }

    {
        s32 move_rate = 0x14000;
        u8 *state_byte = object;
        u8 zero = 0;
        *(s32 *)(object + 0x30) = move_rate;
        *(s32 *)(object + 0x34) = move_rate >> 1;
        state_byte += 0x5b;
        *state_byte = zero;
    }

    ObjectDispatch_InitFromTable6();
    Object_SetMode(object, 5);
    Object_SetPosition(object, x << 16, *(s32 *)(object + 12), z << 16);
    Object_CommitPosition(object);
    Object_SetMode(object, 1);
}
