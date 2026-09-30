#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
#include "CALL.H"

void BattleFx_SetQueuedSoundAndPlay(s32 value);
extern u8 YamaRama_ActorNineAction[];
void ConfigureAndPlaceActorFourteen(void);
void FieldScene_RunScene3a2SequenceA(void);
void ActorPresentation_PrepareActorFourteenWithCallback(void);
void FieldScene_SetSlot15Byte89AndRunStep(void);
void Scene_RunActorExchange(void);
struct FieldActor *Object_GetById(s32 actor);

/* Mountain entry: set the entrance selector; in the second area stage the actors and the entrance scene, in the first restore the exchange scene's actors and cells from its story flags. */
s32 YamaRama_ApplyEntryState(void)
{
    s32 entrance;

    gEventWork->start_transition = 0x100;
    if (gGameState.scene == (s32)&SceneId_YamaRama2) {
        BattleFx_SetQueuedSoundAndPlay(169);
        Engine_ActorSetAnimation(11, 5);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetAnimation(14, 2);
        Call6(Engine_MapCopyCellAttributes, 21, 9, 1, 1, 21, 73);
        ConfigureAndPlaceActorFourteen();
        if (Engine_GameFlagIsSet(0x8b2)) {
            Call3(Engine_ActorSetPosition, 13, 0x880000, 0x1000000);
            Engine_ActorFaceDirection(13, 0, 0);
        }
        entrance = gGameState.entrance;
        if (entrance == 2) {
            Engine_GameFlagClear(0x12f);
        } else if (entrance == 3 && !Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunScene3a2SequenceA();
        }
    } else if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
        Object_GetById(14)->priority_flags |= 2;
        if (Engine_GameFlagIsSet(0x200)) {
            Engine_ActorSetAnimation(14, 5);
            ActorPresentation_PrepareActorFourteenWithCallback();
        }
        if (Engine_GameFlagIsSet(0x201)) {
            Engine_ActorSetAnimation(15, 4);
            FieldScene_SetSlot15Byte89AndRunStep();
        }
        if (gGameState.entrance == 4 || gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        }
        if (!Engine_GameFlagIsSet(0x89a) && !Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2) && Engine_GameFlagIsSet(0x895) && gGameState.entrance == 2) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_GameFlagSet(0x8b2);
            Engine_GameFlagSet(0x8b3);
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Engine_MapCopyCellsTo(54, 21, 53, 21, 1, 2);
            Engine_MapCopyCellAttributes(18, 20, 1, 3, 17, 21);
            Engine_MapCopyCellsTo(44, 18, 43, 17, 1, 1);
            Engine_MapCopyCellAttributes(8, 17, 1, 1, 7, 17);
        }
        if (Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Call3(Engine_ActorSetPosition, 8, 0xc00000, 0x1080000);
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Call3(Engine_ActorSetPosition, 10, 0xb80000, 0x1300000);
            Call3(Engine_ActorFaceDirection, 8, 0x5000, 0);
            Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            Object_GetById(9)->scale_x = -0x10000;
        }
        if (!Value1(Engine_GameFlagIsSet, 0x8b2)) {
            Call3(Engine_ActorSetPosition, 9, 0xa40000, 0x1180000);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            Object_GetById(9)->scale_x = -0x10000;
        }
        if (gGameState.entrance == 5 && !Engine_GameFlagIsSet(0x8b1) && !Engine_GameFlagIsSet(0x109) && !Engine_GameFlagIsSet(0x8b2)) {
            Scene_RunActorExchange();
        }
    }
    return 0;
}
