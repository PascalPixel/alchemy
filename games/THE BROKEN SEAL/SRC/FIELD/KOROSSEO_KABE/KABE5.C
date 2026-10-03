#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "TASK.H"

/*
 * Seven arguments: four in registers, three from the caller's stack; five are
 * stored as halfwords and two as words, which is how they are typed here. The
 * 232-byte owner includes the seven-word literal pool the body branches over.
 * When the story flag is clear, `second` is mirrored about `centre`. The
 * extent stored at +216 is passed back sign-extended from sixteen bits.
 * Descriptor layout is asserted only for the fields written here.
 */
void FieldScene_BuildDescriptorAndInstallTask(s32 first, s32 second, s32 mode, s32 centre,
                   s32 extra, s32 third, s32 fourth)
{
    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Runtime_AllocateBlock(59, 0x7170);
    handle = (s32)Runtime_BumpAllocateAlternatePool(512);                /* 128 << 2 */

    *(u16 *)(descriptor + 222) = (u16)first;
    *(u16 *)(descriptor + 224) = (u16)second;
    *(u16 *)(descriptor + 226) = (u16)third;
    *(u16 *)(descriptor + 228) = (u16)fourth;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = (u8 *)Object_GetById(first);
    second_record = (u8 *)Object_GetById(second);

    if (Engine_GameFlagIsSet(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Resource_DecodeType01(Korosseo_GaugeGraphics, handle);

    extent = Resource_FindFreeEntry();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Engine_VramLoad((s16)extent, 512, handle);

    /* The stage task runs every frame. */
    Engine_TaskAddCallback(Korosseo_DrawGauge, 0xc76);

    Runtime_BumpFree(handle);
}

/* Decode the stage's resource into its descriptor, open the control record
 * unless the party's position is kept, and start the rival on its path. */
void SceneState_InitControlRecordAndStartTask(s32 resource)
{
    u8 *state = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    struct StageControl *m = (struct StageControl *)gSceneState;

    Resource_DecodeType01((const u8 *)Resource_GetTableEntry(resource), state + 240);
    if (GameFlag_IsSet(0x109) == 0) {
        m->status = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}
