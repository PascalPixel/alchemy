#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "field_scene.h"

static void ActorDraw_SetupActorSceneCells(void)
{
    struct FieldActor  *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldSprite *sprite = actor->sprite;

    Engine_AudioPlayCue(188);
    Map_CopyCellsTo(42, 33, 34, 16, 2, 2);
    Map_CopyCellsTo(42, 35, 36, 16, 2, 2);
    Engine_EventWait(4);
    Map_CopyCellsTo(40, 33, 34, 16, 2, 2);
    Map_CopyCellsTo(40, 35, 36, 16, 2, 2);
    Engine_EventWait(4);
    {
        Map_CopyCellAttributes(33, 21, 2, 2, 3, 16);
    }
    actor->priorityFlags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(64, 272, 11);
}
