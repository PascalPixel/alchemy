#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "FIELD_EFFECT.H"

void Event_SetPairWork1c0(s32 scene, s32 entrance);

/* The selected actor and actor 54 rise out of sight together; the map closes
   and the party is sent to the world map's entrance 27. */

void WorldMap_RaiseActors(void)
{
    struct FieldActor *actor = Actor_Get(gGameState.selected_actor);
    struct FieldActor *other = Actor_Get(54);
    s32 frames;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Audio_PlayCue(219);
    Actor_SetSpriteFlags(actor, 0);
    other->motion_flags = 0;
    actor->motion_flags = 0;
    actor->velocity_y = 0;
    actor->unknown_5d[4] = 1;
    other->unknown_5d[4] = 1;
    for (frames = 59; frames >= 0; --frames) {
        actor->velocity_y += 0x3333;
        other->velocity_y += 0x3333;
        Task_Wait(1);
    }
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
    GameFlag_Set(0x122);
    Event_SetPairWork1c0((s32)&SceneId_WorldMap, 27);
}
