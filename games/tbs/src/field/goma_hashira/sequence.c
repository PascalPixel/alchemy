#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"



void Engine_EventWait(s32 frames);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);

void BattleFx_SetPhaseRequest(s32, s32);
void GomaHashira_SpawnPillarEffect();

static __inline__ void Goma_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunActor13Departure from mov r0, #13 to lsl r2, r2, #9 (82/82 assembly lines). */
    Engine_ActorSetSpeed(actor, speed, acceleration);
}

static __inline__ void Goma_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                               s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: a direct call changes FieldScene_SetupPillarsOnEntry from mov r2, #6 to str r3, [sp] (183/185 assembly lines). */
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}

#include "types.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldScene_RunActor13Departure(void)
{
    Engine_TaskRemoveCallback(GomaHashira_DriveActor13Idle);
    Engine_EventBegin();
    Actor_ShowEmote(13, 0x100, 30);
    Engine_ActorRunRepeatedMotion(13, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
    Engine_EventSetMessage((s32)MsgGomaGotWowThatsPrettyImpressive);
    Event_ShowMessage(13, 0);
    Engine_ActorSetAnimationAndWait(13, 3);
    Engine_EventWait(30);
    Object_GetById(10)->priority_flags &= ~ACTOR_PRIORITY_UNDERFOOT;
    Goma_SetSpeed(13, 0x20000, 0x10000);
    Actor_WalkToAndWait(13, 0x258, 216);
    Actor_WalkToAndWait(13, 0x258, 248);
    Actor_WalkToAndWait(13, 0x238, 0x128);
    Actor_SetPosition(13, 0, 0);
    Object_GetById(10)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    GameFlag_Set(0x869);
    Engine_EventEnd();
}

s32 FieldScene_SetupPillarsOnEntry(void)
{
    struct FieldActor *actor;
    struct EventWork *work;

    work = gEventWork;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    work->transition_frames = 24;
    Object_GetById(9)->collision_flags |= 16;
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(8, 0x1580000, 0x680000);
        Goma_CopyCellAttributes(24, 40, 6, 3, 18, 6);
    } else {
        Goma_CopyCellAttributes(18, 40, 6, 3, 18, 6);
    }
    if (GameFlag_IsSet(0x300) != 0) {
        Actor_SetPosition(9, 0, 0);
        Goma_CopyCellAttributes(21, 45, 4, 2, 21, 11);
    }
    if (GameFlag_IsSet(0x301) != 0) {
        Actor_SetPosition(10, 0x2680000, 0xe80000);
        if (gGameState.entrance == 2 || gGameState.entrance == 3) {
            Object_GetById(10)->terrain_id = 2;
            actor = Object_GetById(10);
            actor->y.fixed = actor->y.fixed - 1;
            Object_GetById(10)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
            Goma_CopyCellAttributes(36, 48, 5, 1, 36, 14);
        }
    }
    if (gGameState.entrance == 99) {
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Actor_SetPosition(9, 0x1800000, 0xc00000);
        Engine_EventWait(60);
        Object_GetById(9)->terrain_id = 2;
        Actor_MoveToAndWait(9, 0x198, 192);
        Engine_EventWait(60);
        FieldScene_RunPillarSequence();
    }
    if (gGameState.pending_djinn_event != 0) {
        GomaHashira_Actor13Frames = 0;
        Call2(Engine_TaskAddCallback, (s32)GomaHashira_DriveActor13Idle, 0xc80);
    }
    return 0;
}
