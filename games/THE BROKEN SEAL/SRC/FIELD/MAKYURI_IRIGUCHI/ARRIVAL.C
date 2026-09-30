#include "ENTRANCE.H"

void MakyuriIriguchi_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 flag;
    s32 record;

    actor = (struct FieldActor *)Object_GetById(ACTOR_PARTY_LEADER);
    flag = GameFlag_IsSet(0x109);
    if (flag == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(record, 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriIriguchi_TrailSparks;
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
        Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
        record = Actor_Get(ACTOR_PARTY_LEADER);
        Actor_SetSpriteFlags(record, 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Event_End();
    }
}
