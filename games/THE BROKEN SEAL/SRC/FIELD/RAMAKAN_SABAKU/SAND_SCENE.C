#include "RAMAKAN.H"
#include "CALL.H"

void FieldScene_RunScene3a5_020014b0(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = gWork;
    RamakanSabaku_UpdateTravelDust();
    if (GameFlag_IsSet(0x90a) == 0) {
        rec8 = GameFlag_IsSet(0x200);
        if (rec8 == 0) {
            GameFlag_Set(0x200);
            SceneState_SetHalfwordB030(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Engine_ActorGet(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 36) = rec8;
            record = Engine_ActorGet(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 44) = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 56) = -0x80000000;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            *(s32 *)(record + 64) = -0x80000000;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
            Event_Wait(40);
            Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
            Event_Wait(40);
            *((u8 *)Engine_ActorGet(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Actor_Get(ACTOR_PARTY_LEADER);
            Call3(Vector_AddPolarOffset, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
            Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
            Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, rect[0] / 0x10000, rect[2] / 0x10000);
            Actor_WaitForMove(ACTOR_PARTY_LEADER);
            Event_Wait(2);
            *((u8 *)Engine_ActorGet(0) + 90) |= 1;
            Event_Wait(30);
            Audio_PlayCue(148);
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Actor_SetSpeed(8, 0x28000, 0x14000);
            Actor_WalkToAndWait(8, 168, 104);
            Actor_SetSpeed(8, 0x8000, 0x4000);
            Actor_WalkToAndWait(8, 168, 92);
            *shown_addr = shown;
            SceneState_SetHalfwordB030(0);
        }
    }
}

