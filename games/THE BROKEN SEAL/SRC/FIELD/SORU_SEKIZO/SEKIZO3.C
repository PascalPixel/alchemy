#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "STATUE_HALL.H"

s32 SceneEventRuntime_SelectInitialSceneByFlags(void);
void SoruSekizo_RunSealOpenedSequence(void);

/* A map-cell copy as MapCopyCellsTo takes it. */
struct CellCopy {
    s32 src_x;
    s32 src_y;
    s32 dest_x;
    s32 dest_y;
    s32 width;
    s32 height;
};

struct SealScene {
    struct CellCopy open;
    struct CellCopy pulse_a;
    struct CellCopy pulse_b;
    struct CellCopy opened;
    s32 flag;
    s32 facing;
    s32 dest_x;
    s32 dest_z;
    struct CellCopy stairs[3];
    s32 actor;
    s32 actor_x;
    s32 actor_z;
    s32 unknown_c4[3];
};

/* The seal scene the guarded steps fill in; the first of the overlay's own work. */
struct SealScene gSealScene;
#define COPY_CELLS(c) Call6(Engine_MapCopyCellsTo, (c).src_x, (c).src_y, (c).dest_x, (c).dest_y, (c).width, (c).height)

/* Sol Sanctum seal: unless flag 0x80f is set, pan to the seal and pulse its cells twenty times, open it, then either walk the party onto the revealed stairs or, once the three statue flags are set, run the follow-up scene. */
void SoruSekizo_OpenSeal(void)
{
    struct SealScene *scene;
    s32 result;
    s32 i;

    result = 0;
    Engine_EventBegin();
    if (!Engine_GameFlagIsSet(0x80f)) {
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2400000, -1, 0xac0000, 1);
        Engine_CameraWaitForMove();
        Engine_AudioPlayCue(186);
        COPY_CELLS(gSealScene.open);
        for (i = 0; i != 20; i++) {
            Engine_AudioPlayCue(246);
            COPY_CELLS(gSealScene.pulse_a);
            Engine_EventWait(4);
            Engine_AudioPlayCue(246);
            COPY_CELLS(gSealScene.pulse_b);
            Engine_EventWait(4);
        }
        scene = &gSealScene;
        COPY_CELLS(scene->opened);
        Engine_GameFlagSet(scene->flag);
        result = SceneEventRuntime_SelectInitialSceneByFlags();
        if (result == -1) {
            if (!Engine_GameFlagIsSet(0x818)) {
                Engine_CameraFollowActor(0, 1);
                Object_GetById(0)->facing = scene->facing;
                Call3(Engine_ActorSetSpeed, 0, 0x20000, 0x20000);
                Object_GetById(0)->unknown_5a &= ~1;
                Engine_ActorJump(0, 4, 0);
                Engine_ActorSetDestination(0, scene->dest_x, scene->dest_z);
                COPY_CELLS(scene->stairs[0]);
                COPY_CELLS(scene->stairs[1]);
                COPY_CELLS(scene->stairs[2]);
                Object_GetById(scene->actor)->unknown_5a &= ~1;
                Engine_ObjectMotionSetPositionAndCommit(scene->actor, scene->actor_x, scene->actor_z);
                Object_GetById(0)->unknown_5a |= 1;
                Engine_GameFlagClear(scene->flag);
            }
        } else if (result == 0 && Engine_GameFlagIsSet(0x818)) {
            if (Engine_GameFlagIsSet(0x80b) && Engine_GameFlagIsSet(0x80d) && Engine_GameFlagIsSet(0x80e)) {
                if (!Value1(Engine_GameFlagIsSet, 0x80f)) {
                    Engine_GameFlagSet(0x80f);
                    SoruSekizo_RunSealOpenedSequence();
                }
            } else if (Engine_GameFlagIsSet(0x812)) {
                Engine_EventRequestExit(5);
                result = 1;
            }
        }
    }
    if (result == 1) {
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
    }
    Engine_EventEnd();
}

void Scene_ShineLeftBeam(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x818) == 0) {
        if (Engine_GameFlagIsSet(FLAG_LEFT_BEAM_SHINING) == 0) {
            Engine_CameraSetSpeed(0x20000, 0x4000);
            Engine_CameraMoveTo(0x11e0000, -1, 0x920000, 1);
            Engine_CameraWaitForMove();
            Engine_AudioPlayCue(186);
            Engine_MapCopyCellsTo(0, 59, 15, 38, 4, 3);
            if (Engine_GameFlagIsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
                Engine_MapCopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Engine_EventWait(30);
            Engine_GameFlagSet(FLAG_LEFT_BEAM_SHINING);
            if (Engine_GameFlagIsSet(FLAG_RIGHT_BEAM_SHINING) != 0) {
                Scene_OpenTheHole();
            }
        }
    }
    Engine_EventEnd();
}

void Scene_ShineRightBeam(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x818) == 0) {
        if (Engine_GameFlagIsSet(FLAG_RIGHT_BEAM_SHINING) == 0) {
            Engine_CameraSetSpeed(0x20000, 0x4000);
            Engine_CameraMoveTo(0x11e0000, -1, 0x920000, 1);
            Engine_CameraWaitForMove();
            Engine_AudioPlayCue(186);
            Engine_MapCopyCellsTo(4, 59, 17, 38, 4, 3);
            if (Engine_GameFlagIsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
                Engine_MapCopyCellsTo(8, 60, 17, 39, 2, 2);
            }
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Engine_EventWait(30);
            Engine_GameFlagSet(FLAG_RIGHT_BEAM_SHINING);
            if (Engine_GameFlagIsSet(FLAG_LEFT_BEAM_SHINING) != 0) {
                Scene_OpenTheHole();
            }
        }
    }
    Engine_EventEnd();
}

/* Shows the next line of dialogue, then holds the scene for a moment. */
void Event_SayThenWait(s32 speaker, s32 frames)
{
    Engine_EventShowMessage(speaker, 0);
    Engine_EventWait(frames);
}
