#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MAPCOPY.H"
#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "GOMA.H"
#include "SCENE_IDS.H"






const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEntrances2;
    }
    return gGomaSuiroEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return GomaSuiro_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroPlacements2;
    }
    return gGomaSuiroPlacementsOther;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    struct FieldActor *target;
    s32 value;

    record = (s32 *)Object_GetById(8);
    value = record[2] / 0x100000;
    if (value == 24) {
        SceneEffect_RunActorBurst(8);
        Object_GetById(8)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
        target = Object_GetById(8);
        Engine_ActorSetSpriteFlags((s32)target, 0);
        ((void (*)())Engine_GameFlagSet)(0x864);
    }
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEvents2;
    }
    return gGomaSuiroEventsOther;
}

s32 FieldScene_PlaceActor8OnEntry(void)
{
    struct FieldActor *record;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (gGameState.scene == (s32)&SceneId_GomaSuiro1) {
        if (gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        } else {
            Object_GetById(8)->collision_flags |= 16;
            if (Engine_GameFlagIsSet(0x864) != 0) {
                Engine_ActorSetPosition(8, 0x15a0000, 0x1240000);
                record = Object_GetById(8);
                Engine_ActorSetSpriteFlags(record, 0);
                *((u8 *)Object_GetById(8) + 35) |= 2;
                Engine_ActorSetAnimation(8, 2);
                Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}
