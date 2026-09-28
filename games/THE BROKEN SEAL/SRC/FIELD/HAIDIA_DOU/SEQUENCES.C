#include "HAIDIA.H"

void FieldScene_RunInitBracketThenSequence(void)
{
    Event_Begin();
    StagedActor_AdvancePair();
    Event_End();
    HaidiaDou_SinkPillarColumn27();
}

s32 FieldScene_RunPrimarySequence(s32 a0)
{

    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Object_CheckMovementCollision, (s32)rec, (s32)slot) == 0) {
        Event_Begin();
        Object_SetAnimation((s32)rec, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation((s32)rec, 7);
        *(s32 *)(rec + 48) = 0x30000;
        *(s32 *)(rec + 52) = 0x20000;
        *(s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Actor_SetSpriteFlags((s32)rec, 0);
        Value3(Engine_ActorMoveToAndWait, 0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Object_SetAnimation((s32)rec, 6);
        Actor_SetSpriteFlags((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Engine_EventEnd)();
        return 1;
    }
    return 0;
}

void FieldScene_RunScene3a6SequenceA(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        GameFlag_Set(0x200);
        Event_Begin();
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_FollowActor(8, 1);
        Camera_WaitForMove();
        Event_Wait(60);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetAttachedEffect(8, 0x102);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Actor_SetSpeed(8, 0x10000, 0x8000);
        Actor_WalkToAndWait(8, 0x318, 248);
        Audio_PlayCue(152);
        record = Actor_Get(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(8, 0x318, 0x118);
        Event_Wait(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Event_Wait(30);
        Event_End();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) != 0) {
        if (GameFlag_IsSet(0x201) == 0) {
            GameFlag_Set(0x201);
            GameFlag_Set(0x302);
            Event_Begin();
            Actor_SetAttachedEffect(8, 0x102);
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Actor_SetSpeed(8, 0x20000, 0x10000);
            Actor_WalkToAndWait(8, 0x2f8, 0x118);
            Actor_WalkToAndWait(8, 0x2f8, 0x138);
            Actor_WalkToAndWait(8, 0x318, 0x138);
            Event_Wait(10);
            Actor_FaceDirection(8, 0xc000, 20);
            record = Actor_Get(8);
            *(s32 *)(record + 108) = (s32)SceneActor_FaceActorZero;
            Call0((void (*)())Engine_EventEnd);
        }
    }
}

void FieldScene_RunScene3a6SequenceC(void)
{

    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)&gGameState + off24a) != 8) {
            idx = p5->touched_trigger;
            rec8 = Value1(Engine_ActorGet, 8);
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            *(s32 *)(rec8 + 48) = *(s32 *)(record + 48);
            rec8 = Value1(Engine_ActorGet, 8);
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            *(s32 *)(rec8 + 52) = *(s32 *)(record + 52);
            idx -= 45;
            tbl = (s32)HaidiaDou_WalkTargets;
            idx <<= 3;
            idx4 = idx + 4;
            Actor_WalkTo(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void FieldScene_RunActor8ZeroStep(void)
{
    Event_Begin();
    Actor_SetAnimation(8, 0);
    Event_End();
}

