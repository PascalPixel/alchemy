#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation);

void ActorDraw_SetupActorSceneCells(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldSprite *sprite = actor->sprite;

    Engine_AudioPlayCue(188);
    Engine_MapCopyCells(42, 33, 34, 16, 2, 2);
    Engine_MapCopyCells(42, 35, 36, 16, 2, 2);
    Engine_EventWait(4);
    Engine_MapCopyCells(40, 33, 34, 16, 2, 2);
    Engine_MapCopyCells(40, 35, 36, 16, 2, 2);
    Engine_EventWait(4);
    {
        s32 dest_x = 3;
        s32 dest_y = 16;
        Engine_MapCopyCellAttributes(33, 21, 2, 2, dest_x, dest_y);
    }
    actor->priority_flags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(64, 272, 11);
}
