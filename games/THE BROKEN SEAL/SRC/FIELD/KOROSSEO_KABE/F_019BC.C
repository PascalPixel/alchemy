#include "TYPES.H"
#include "FIELD_EVENT.H"

void Korosseo_FinishSoloRound(void);
s32 run_state_interaction(s32 speaker, s32 base);
s32 Local_02002ba8(s32 actor, s32 x, s32 z);
s32 SceneActor_PlaceWithScale14000(s32 no, s32 x, s32 z);
void KorosseoKabe_Func02001898(s32 id, s32 column, s32 row);
void Korosseo_RestoreCompetitor(s32 actor);
void Local_020020b8(s32 speaker, s32 base);
s32 FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base);

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The Colosso guide at the Board Walk: on the first visit pans the camera
 * over the course and walks the competitor through pushing a block while
 * explaining it; afterwards reminds that the logs are rolled into a path. */
void KorosseoKabe_Func020019bc(s32 speaker)
{
    s32 result;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    result = run_state_interaction(speaker, 5);
    if (result == 0) {
        s32 x;
        s32 z;

        Engine_EventSetMessage(8366);
        Call2((void (*)())Engine_CameraSetSpeed, 0x20000, 0x4000);
        Call4((void (*)())Engine_CameraMoveTo, 0x1480000, -1, 0x1080000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Call2((void (*)())Engine_CameraSetSpeed, 0x18000, 0x3000);
        x = 408;
        z = 264;
        Call4((void (*)())Engine_CameraMoveTo, 0x1380000, -1, 0xb00000, 1);
        Engine_CameraWaitForMove();
        Engine_EventShowMessage(speaker, 0);
        Local_02002ba8(0, x, z);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        SceneActor_PlaceWithScale14000(0, x, 216);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0x8000, 10);
        Engine_EventShowMessage(speaker, 0);
        KorosseoKabe_Func02001898(16, 360, 208);
        Engine_ActorShowEmote(0, z, 45);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        x -= 32;
        SceneActor_PlaceWithScale14000(0, x, 216);
        SceneActor_PlaceWithScale14000(0, x, 248);
        SceneActor_PlaceWithScale14000(0, 312, 248);
        Engine_EventShowMessage(speaker, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        Call3((void (*)())Engine_ActorSetPosition, 16, 0x1880000, 0xd00000);
        Local_020020b8(speaker, 5);
    } else if (result == 1) {
        Engine_EventSetMessage(8365);
        Engine_EventShowMessage(speaker, 0);
    }
    FieldScene_RunMiddleSequence(result, speaker, 5);
    Engine_EventEnd();
}
