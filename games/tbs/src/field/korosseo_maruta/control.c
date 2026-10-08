#include "resource.h"
#include "log_rolling.h"
#include "mem.h"

void Korosseo_UpdatePathRival(void);

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    extern void Korosseo_DrawGauge(void);

    u8 *descriptor;
    struct FieldActor *first_record;
    struct FieldActor *second_record;
    s32 handle;
    s32 extent;

    descriptor = Runtime_AllocateBlock(59, 0x7170);
    handle = (s32)Runtime_BumpAllocateAlternatePool(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Object_GetById(first_actor);
    second_record = Object_GetById(second_actor);

    if (Engine_GameFlagIsSet(0x109) == 0) {
        second_record->x.fixed =
            (centre << 1) - first_record->x.fixed;
        second_record->z.fixed = first_record->z.fixed;
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Resource_DecodeType01(Korosseo_GaugeGraphics, handle);

    extent = Resource_FindFreeEntry();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Engine_VramLoad((s16)extent, 512, handle);

    Engine_TaskAddCallback((s32)Korosseo_DrawGauge + 1, 0xc76);

    Runtime_BumpFree(handle);
}

void ColossoLogRollingStage_InitializeSceneControl(s32 resource)
{
    u8 *work = gKorosseoWork;
    SceneControl *control = (SceneControl *)gSceneState;

    Resource_DecodeType01(Resource_GetTableEntry(resource), work + 240);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        control->enabled = 1;
        control->active = 1;
        control->scene_variant = *(u16 *)(work + 224);
        control->timer = 0;
        control->phase = 0;
    }
    Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}
