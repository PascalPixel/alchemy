#include "ENTRY_SETUP.H"
struct FieldActor;
s32 VinasuHeya_UpdateRisingSpray(struct FieldActor *actor);

void FieldScene_RunLeaderDropSequence(void)
{
    u32 i;
    u8 *p8;
    s32 rec;
    u8 *rec8;
    s32 record;
    s32 none;
    s32 v2;
    s32 slot0;

    rec = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    rec8 = Value1(Engine_ActorGet, 20);
    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(rec + 12) = 0x820000;
    *(s32 *)(rec + 72) = 0x8000;
    none = 0;
    *(s32 *)(rec + 68) = none;
    p8 = rec + 85;
    *p8 = none;
    Event_OpenScreen();
    Event_WaitForScreen();
    Audio_PlayCue(204);
    Event_Wait(30);
    *p8 = 3;
    Event_Wait(24);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    *p8 &= 254;
    *(s32 *)((s32)rec8 + 12) += -0x30000;
    *(s32 *)(rec + 12) += -0x30000;
    *(s32 *)(rec + 20) += -0x30000;
    Task_Wait(2);
    *(s32 *)((s32)rec8 + 12) += -0x20000;
    *(s32 *)(rec + 12) += -0x20000;
    *(s32 *)(rec + 20) += -0x20000;
    Task_Wait(10);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x20000;
    *(s32 *)(rec + 12) += 0x20000;
    *(s32 *)(rec + 20) += 0x20000;
    Task_Wait(4);
    *(s32 *)((s32)rec8 + 12) += 0x10000;
    *(s32 *)(rec + 12) += 0x10000;
    *(s32 *)(rec + 20) += 0x10000;
    *p8 = none;
    rec8[85] = none;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(40);
    *(s32 *)(rec + 108) = (s32)VinasuHeya_UpdateRisingSpray;
    Event_Wait(60);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Actor_SetSpritePriority(20, 1);
    Audio_PlayCue(17);
    Audio_PlayCue(0x134);
    GameFlag_Set(0x101);
    v2 = 0;
    do {
        *(s32 *)(rec + 12) += 0x10000;
        *(s32 *)(rec + 20) += 0x10000;
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        slot0 = v2;
        Task_Wait(1);
        v2 = slot0;
        v2 = (v2 + 1);
    } while ((u32)v2 <= 127);
    Event_RequestExit(21);
}
