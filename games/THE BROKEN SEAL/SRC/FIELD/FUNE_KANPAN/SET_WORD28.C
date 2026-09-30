#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

s32 SceneActor_SetWord28RandomlyOneIn40(struct FieldActor *actor)
{
    if ((((u32)(Random_Next() * 40)) >> 16) == 0)
        actor->velocity_y = 0x40000;
    return 1;
}

void OverlayObject_DecayFields24And28(struct FieldActor *actor)
{
    if (actor->scale_x > 0x10000) {
        actor->scale_x += 0xFFFFF800;
        actor->scale_y += 0xFFFFF800;
    }
}
