#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void InitializeSceneRecordBuffer();
void BattleFx_SetQueuedSoundAndPlay();
s32 Scene_RunActorFormation();
void Scene_RunTransitionCue();
void Scene_EnterSolSanctum();
void Scene_SukuretaSuspectsHiddenPassage();

/* Opens the scene with the window transition, playing sound 141 when flag
 * 0x814 is set, then prepares the entrance the party arrived by. Entrances 1
 * and 2 change the map once flag 0x81a is set. Entrance 3 clears actor 9's
 * sprite flags. Entrance 8 stores two game-state halfwords and runs a scene
 * until flag 0x802 is set. Entrances 11 to 13 clear the sprite flags of
 * actors 9 to 19, run a scene until flag 0x804 is set and place actors 9 and
 * 10 by flag. Entrances 14 to 16 clear those of actors 9 to 14, run a scene
 * until flag 0x825 is set, run one more step, set flag 0x234 and change the
 * map once flag 0x821 is set. */
void FieldScene_RunSceneEntryHook(void)
{
    s32 entrance;
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (Engine_GameFlagIsSet(0x814) != 0) {
        BattleFx_SetQueuedSoundAndPlay(141);
        Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x10000, 0x10000);
        InitializeSceneRecordBuffer();
    }

    entrance = gGameState.entrance;
    switch (entrance) {
    case 1:
    case 2:
        if (Engine_GameFlagIsSet(0x81a) != 0) {
            Engine_MapCopyCellsTo(1, 109, 4, 81, 1, 1);
            Engine_MapCopyCellsTo(0, 70, 30, 42, 1, 1);
            Engine_MapCopyCellsTo(0, 29, 3, 1, 3, 2);
            Engine_MapCopyCellAttributes(0, 29, 3, 2, 3, 1);
            Engine_MapRedraw();
        }
        break;

    case 3:
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        break;

    case 8:
        gGameState.retreat_scene = (s32)&SceneId_SoruIriguchi1;
        gGameState.retreat_entrance = 8;
        if (GameFlag_IsSet(0x802) == 0)
            Scene_EnterSolSanctum();
        break;

    case 11:
    case 12:
    case 13:
        Actor_SetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(15), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(16), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(17), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(18), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(19), 0);
        if (Engine_GameFlagIsSet(0x804) == 0)
            Scene_SukuretaSuspectsHiddenPassage();
        if (Engine_GameFlagIsSet(0x303) != 0)
            Call3(Engine_ActorSetPosition, 9, 0x5d80000, 0x880000);
        else if (GameFlag_IsSet(0x302) != 0)
            Actor_SetPosition(9, 0x5f80000, 0x880000);
        if (Engine_GameFlagIsSet(0x301) != 0)
            Call3(Engine_ActorSetPosition, 10, 0x7180000, 0x880000);
        else if (Engine_GameFlagIsSet(0x300) != 0)
            Call3(Engine_ActorSetPosition, 10, 0x7380000, 0x880000);
        break;

    case 14:
    case 15:
    case 16:
        Engine_ActorSetSpriteFlags(Engine_ActorGet(9), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(10), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(11), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(12), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(13), 0);
        Engine_ActorSetSpriteFlags(Engine_ActorGet(14), 0);
        if (Engine_GameFlagIsSet(0x825) == 0)
            Scene_RunTransitionCue();
        Scene_RunActorFormation(1);
        Engine_GameFlagSet(0x234);
        if (GameFlag_IsSet(0x821) != 0) {
            Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
            Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
            Call6(Engine_MapCopyCellAttributes, 122, 20, 1, 2, 120, 30);
            Map_Redraw();
        }
        break;

    }
}
