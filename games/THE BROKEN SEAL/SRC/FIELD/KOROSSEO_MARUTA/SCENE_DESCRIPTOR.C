/* Setting up the stage's scene descriptor. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_SetupSceneDescriptor(s32 first_actor, s32 second_actor,
                   s32 mode, s32 centre, s32 extra, s32 third_actor,
                   s32 fourth_actor)
{
    extern u8 *Engine_AllocateBlock();
    extern s32 Runtime_BumpAllocateAlternatePool();
    extern void Resource_DecodeType01();
    extern s32 Resource_FindFreeEntry();
    extern void Runtime_BumpFree();
    extern void FieldScene_RunScene3bcSequenceB(void);

    u8 *descriptor;
    u8 *first_record;
    u8 *second_record;
    s32 handle;
    s32 extent;

    descriptor = Engine_AllocateBlock(59, 0x7170);
    handle = Runtime_BumpAllocateAlternatePool(512);

    *(u16 *)(descriptor + 222) = (u16)first_actor;
    *(u16 *)(descriptor + 224) = (u16)second_actor;
    *(u16 *)(descriptor + 226) = (u16)third_actor;
    *(u16 *)(descriptor + 228) = (u16)fourth_actor;
    *(u16 *)(descriptor + 230) = (u16)mode;
    *(s32 *)(descriptor + 232) = centre;
    *(s32 *)(descriptor + 236) = extra;

    first_record = Engine_ActorGet(first_actor);
    second_record = Engine_ActorGet(second_actor);

    if (GameFlag_IsSet(0x109) == 0) {
        *(s32 *)(second_record + 8) =
            (centre << 1) - *(s32 *)(first_record + 8);
        *(s32 *)(second_record + 16) = *(s32 *)(first_record + 16);
    }

    *(u16 *)(descriptor + 218) = 0;
    *(u16 *)(descriptor + 220) = 0;

    Resource_DecodeType01(gColossoSceneDescriptor, handle);

    extent = Resource_FindFreeEntry();
    *(u16 *)(descriptor + 216) = (u16)extent;
    Vram_Load((s16)extent, 512, handle);

    Engine_TaskAddCallback((s32)FieldScene_RunScene3bcSequenceB + 1, 0xc76);

    Runtime_BumpFree(handle);
}
