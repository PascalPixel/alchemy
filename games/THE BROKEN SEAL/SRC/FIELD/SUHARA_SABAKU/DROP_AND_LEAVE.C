/* The sand swallows the leader and the actor flag byte 0x218 names: cue 219,
 * both sink for sixty frames, the screen closes, flag 0x122 is set and the
 * party leaves for the world map, at entrance 77 from the second area when
 * the flag byte names actor 11 and at entrance 27 otherwise. */
#include "SABAKU.H"

void Event_SetPairWork1c0(s32 scene, s32 entrance);

void SuharaSabaku_DropAndLeave(void)
{
    s32 other;
    struct FieldActor *leader;
    struct FieldActor *partner;
    s32 i;

    other = GameFlag_GetByte(0x218);
    leader = Engine_ActorGet(gGameState.selected_actor);
    partner = Engine_ActorGet(other);
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_AudioPlayCue(219);
    Engine_ActorSetSpriteFlags((struct FieldActor *)gGameState.selected_actor, 0);
    partner->motion_flags = 0;
    leader->motion_flags = 0;
    leader->velocity_y = 0;
    leader->unknown_5d[4] = 1;
    partner->unknown_5d[4] = 1;
    for (i = 0; i < 60; i++) {
        leader->velocity_y += 0x3333;
        partner->velocity_y += 0x3333;
        Engine_TaskWait(1);
    }
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
    Engine_GameFlagSet(0x122);
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2 && GameFlag_GetByte(0x218) == 11) {
        Event_SetPairWork1c0((s32)&SceneId_WorldMap, 77);
    } else {
        Event_SetPairWork1c0((s32)&SceneId_WorldMap, 27);
    }
}
