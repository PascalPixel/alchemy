/* The palette task and positioning scaled objects. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    ColossoLogRollingStage_EnsurePaletteHandle();

    gColossoPaletteFirst = (u16)first_value;
    gColossoPaletteSecond = (u16)second_value;
    gColossoPaletteMode = (u16)(mode & 3);
    gColossoPaletteStep = 0;
    gColossoPaletteFlags = 0;

    {
        s32 budget = 0xc80;
        s32 task = (s32)ColossoLogRollingStage_PaletteTask;
        Engine_TaskAddCallback(task, budget);
    }
}

void ColossoLogRollingStage_StartPaletteTaskFromState(u32 first_value, u32 second_value, u32 mode)
{
    gColossoPaletteNextFirst = (u16)first_value;
    gColossoPaletteNextSecond = (u16)second_value;
    gColossoPaletteSavedFirst = gColossoPaletteFirst;
    gColossoPaletteSavedSecond = gColossoPaletteSecond;
    gColossoPaletteFlags = (u16)mode;
    gColossoPaletteProgress = 0;

    {
        s32 budget = 0xc80;
        s32 task = (s32)ColossoLogRollingStage_PaletteTask;
        Engine_TaskAddCallback(task, budget);
    }
}

void ColossoLogRollingStage_StopPaletteTask(void)
{
    extern void ColossoLogRollingStage_PaletteTask(void);
    extern void Resource_ResetEntry(s32 slot);

    Engine_TaskRemoveCallback(ColossoLogRollingStage_PaletteTask);
    Resource_ResetEntry(gColossoPaletteHandle);
    gColossoPaletteHandle = -1;
}

void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{
    extern ScaledStageObject *ObjectTable_Get();
    extern void ObjectDispatch_InitFromTable6();
    extern void Object_SetPosition();

    ScaledStageObject *object = ObjectTable_Get(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        ObjectDispatch_InitFromTable6(object);
        Object_SetAnimation(object, 5);
        Object_SetPosition(object, x << 16, object->y, z << 16);
    }
}

void ColossoLogRollingStage_SpawnPositionedObject(s32 object_id, s32 x, s32 z)
{
    extern u8 *ObjectTable_Get(s32 object_id);
    extern void ObjectDispatch_InitFromTable6(void);
    extern void Object_SetPosition(u8 *object, s32 x, s32 y, s32 z);
    extern void Object_CommitPosition(u8 *object);

    u8 *object = ObjectTable_Get(object_id);

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
    Object_SetAnimation(object, 5);
    Object_SetPosition(object, x << 16, *(s32 *)(object + 12), z << 16);
    Object_CommitPosition(object);
    Object_SetAnimation(object, 1);
}
