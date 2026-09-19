#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation);

void ActorDraw_SetupActorSceneCells(void)
{
    struct FieldActor *actor = Actor_Lookup(ACTOR_PARTY_LEADER);
    struct FieldSprite *sprite = actor->sprite;

    Audio_PlayCue(188);
    Map_CopyCells(42, 33, 34, 16, 2, 2);
    Map_CopyCells(42, 35, 36, 16, 2, 2);
    Event_Wait(4);
    Map_CopyCells(40, 33, 34, 16, 2, 2);
    Map_CopyCells(40, 35, 36, 16, 2, 2);
    Event_Wait(4);
    {
        s32 dest_x = 3;
        s32 dest_y = 16;
        Map_CopyCellAttributes(33, 21, 2, 2, dest_x, dest_y);
    }
    actor->priority_flags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(64, 272, 11);
}
