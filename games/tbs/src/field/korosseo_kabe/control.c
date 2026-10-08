#include "resource.h"
#include "mem.h"
#include "task.h"

void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    struct FieldActor *first_record;
    struct FieldActor *second_record;
    s32 handle;
    s32 extent;

    descriptor = Runtime_AllocateBlock(59, 0x7170);
    handle = (s32)Runtime_BumpAllocateAlternatePool(512);

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Object_GetById(first);
    second_record = Object_GetById(second);

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

    Engine_TaskAddCallback(Korosseo_DrawGauge, 0xc76);

    Runtime_BumpFree(handle);
}

void SceneState_InitControlRecordAndStartTask(s32 resource)
{
    u8 *state = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    struct StageControl *m = (struct StageControl *)gSceneState;

    Resource_DecodeType01((const u8 *)Resource_GetTableEntry(resource), state + 240);
    if (Engine_GameFlagIsSet(0x109) == 0) {
        m->status = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}
