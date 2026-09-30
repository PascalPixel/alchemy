#include "PROBE.H"

void MakyuriHeya_ArriveWithSparks(void)
{
    struct FieldActor *actor;
    s32 done;

    actor = Actor_Get(0);
    done = GameFlag_IsSet(0x109);
    if (done == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Engine_ActorSetPosition(0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) - 0x100000);
        Actor_SetChildValue(0, 15);
        Actor_SetSpriteFlags(Actor_Get(0), 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))MakyuriHeya_TrailSparks;
        Actor_SetSpeed(0, 0x6666, 0x3333);
        Engine_ActorWalkByAndWait(0, 0, 8);
        Actor_SetChildValue(0, 0);
        Actor_SetSpriteFlags(Actor_Get(0), 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        BattleFx_PlayQueuedSound();
        Event_End();
    }
}

void SceneEffect_SpawnParticleRowsAndDrawTiles(void)
{
    s32 buf[10];
    u32 i, j;

    Map_CopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
    buf[1] = 7;
    buf[2] = 0x8000;
    buf[3] = 0x8000;
    for (j = 0; j <= 1; j++) {
        for (i = 0; i <= 7; i++) {
            if ((i & 1) != 0) {
                s32 a = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
                s32 b = (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

                Effect_Spawn(0x690000, 0, ((-i - (j << 4)) << 16) + 0x2200000,
                              a, 0, b, 0x90000, buf);
                Event_Wait(1);
            }
        }
        Map_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - j, 1, 1);
        Map_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - j, 1, 1);
    }
}
