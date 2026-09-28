/* Draft of resource_371 0x0200b84c..0x0200b8fc (176 bytes with pool),
 * WorldMap_RaiseActors; the listing keeps the rows. Remaining difference:
 * the reference loads the exit scene 2 for Event_SetPairWork1c0 from its
 * literal pool, a link-time value; the integer scene is an immediate (172
 * bytes, 2 differ at +0x90). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

void Event_SetPairWork1c0(s32 scene, s32 entrance);

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
    Event_SetPairWork1c0(2, 27);
}
