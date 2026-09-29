#include "YAMA.H"

void FieldScene_RunSharedSetPiece(s32 a0)
{
    struct FieldActor *actor0;
    struct FieldActor *actor8;
    struct FieldActor *actor9;
    struct FieldActor *actor10;
    struct FieldActor *actor;

    actor0 = Actor_Get(ACTOR_PARTY_LEADER);
    actor8 = Actor_Get(8);
    actor9 = Actor_Get(9);
    actor10 = Actor_Get(10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x3100000, -1, 0x740000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 100);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(20);
    actor9->scale_x = 0x13333;
    actor9->scale_y = 0x13333;
    actor9->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    actor9->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWithFixedY;
    Actor_SetAnimation(8, 4);
    *(s32 *)((u8 *)actor8 + 68) = 0x8000;
    actor8->x.fixed = 0x3120000;
    actor8->y.fixed = 0x200000;
    actor8->z.fixed = 0x5a0000;
    actor8->scale_x = 0x20000;
    actor8->scale_y = 0x20000;
    Event_Wait(10);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
    Event_Wait(20);
    actor10->x.fixed += 0xe0000;
    actor10->y.fixed += -0x80000;
    actor10->sprite->rotation = 0xc000;
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Audio_PlayCue(55);
    Work_SetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
    Actor_SetSpritePriority(8, 0);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    *(u16 *)((u8 *)actor0 + 100) = 0;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, ArutinYama_LeaderRideScript);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetPosition(ACTOR_GERALD, 0x36e0000, 0x2100000);
        actor = Actor_Get(ACTOR_GERALD);
        actor->facing = 0x5000;
    }
    Camera_SetSpeed(0x14000, 0x2800);
    Camera_MoveTo(0x3120000, -1, 0x22c0000, 1);
    Event_Wait(a0);
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpeed(8, 0x195c2, 0xcae1);
    *(u16 *)((u8 *)actor8 + 100) = 0;
    Actor_EnableActionCallback(8, ArutinYama_LogRideScript);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor0 + 100) == 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor8 + 100) == 0);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    actor9->x.fixed = 0x3120000;
    actor9->update = NULL;
    actor9->z.fixed = 0x26a0000;
    actor9->y.fixed = -0x400000;
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    *(s32 *)((u8 *)actor8 + 68) = 0x1999;
    *(s32 *)((u8 *)actor8 + 72) = 0x3333;
    actor8->velocity_y = 0x40000;
    Actor_MoveToAndWait(8, 0x312, 0x25c);
    Actor_SetSpeed(8, 0x33333, 0x19999);
    Actor_SetDestination(8, 0x312, 0x284);
    Event_Wait(15);
    Work_SetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
    Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
    Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Engine_TaskAddCallback(FieldScene_RunScene3a4SequenceG, 0xc80);
    Event_Wait(80);
    Engine_TaskRemoveCallback(FieldScene_RunScene3a4SequenceG);
    Event_Wait(60);
    Audio_PlayCue(17);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(120);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Actor_WalkTo(ACTOR_GERALD, 0x338, 0x22e);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetAnimation(ACTOR_GERALD, 1);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Camera_MoveTo(0x3140000, -0x400000, 0x2620000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(148);
    Event_Wait(240);
    if (GameFlag_IsSet(0x205) != 0) {
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x3560000, 0, 0x2480000, 1);
        Camera_WaitForMove();
        Actor_WalkToAndWait(ACTOR_GERALD, 0x348, 0x228);
        Actor_WalkToAndWait(ACTOR_GERALD, 0x356, 0x232);
        Actor_SetAnimation(ACTOR_GERALD, 2);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
    }
    Audio_PlayCueFromEventWork();
    GameFlag_Set(0x908);
}
