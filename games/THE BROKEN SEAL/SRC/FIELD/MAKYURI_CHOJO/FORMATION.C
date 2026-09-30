#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "OVERLAY_OBJECT.H"
#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "AERIE.H"

void FieldScene_RunScene39d_020009fc(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 nearest;

    rec = Actor_Get(ACTOR_PARTY_LEADER);
    Event_Begin();
    record = FindNearestF2Actor();
    nearest = (s32)MakyuriChojo_NearestActor;
    *(s32 *)nearest = record;
    if (record != 0) {
        GameFlag_Set(0x250);
        rec8 = Actor_Get(*(s32 *)nearest);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Task_Wait(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Task_Wait(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Task_Wait(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Event_End();
}

void SceneActor_SetMode55OnSevenRecords(void)
{
    ((struct Record *)Engine_ActorGet(0))->mode55 = 3;
    ((struct Record *)Engine_ActorGet(14))->mode55 = 4;
    ((struct Record *)Engine_ActorGet(15))->mode55 = 4;
    ((struct Record *)Engine_ActorGet(16))->mode55 = 4;
    ((struct Record *)Engine_ActorGet(17))->mode55 = 4;
    ((struct Record *)Engine_ActorGet(18))->mode55 = 4;
    ((struct Record *)Engine_ActorGet(19))->mode55 = 4;
}
