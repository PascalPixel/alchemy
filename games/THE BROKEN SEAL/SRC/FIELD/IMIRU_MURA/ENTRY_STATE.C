#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

void SceneState_UpdateActor11WithFlag203(void);
void RunEventScript02(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void FieldScene_RunThreeActorChoreography(void);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
extern u8 ImiruMura_ActorScriptA[];

/* Imil: entry setup for the two Imil areas, by entrance and story flags. */
s32 ImiruMura_ApplyEntryState(void)
{
    struct FieldActor *leader;
    s32 entrance;

    if (gGameState.scene == (s32)&SceneId_ImiruMura1) {
        leader = Object_GetById(0);
        gEventWork->start_transition = 0x100;
        Engine_ActorSetAnimation(10, 9);
        if (Value1(Engine_GameFlagIsSet, 0x109)) {
            Engine_GameFlagClear(0x200);
            Engine_GameFlagClear(0x201);
        }
        leader->unknown_64 = 0;
        leader->unknown_66 = 0;
        Value2(Engine_TaskAddCallback, (s32)FieldScene_RunScene399SequenceA, 0xc80);
        Engine_TaskAddCallback((s32)SceneState_UpdateZoneFlagsFromActorZero, 0xc80);
        Engine_ActorSetSpritePriority(11, 1);
        if (Engine_GameFlagIsSet(0x203)) {
            SceneState_UpdateActor11WithFlag203();
        }
        if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9) {
            RunEventScript02();
        }
    } else if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        gEventWork->start_transition = 0x209;
        entrance = gGameState.entrance;
        if (entrance == 1) {
            Engine_ActorSetChildValue(21, 15);
            Object_GetById(21)->collision_flags |= 8;
            Engine_ActorSetSpritePriority(21, 1);
            if (Engine_GameFlagIsSet(0x881)) {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                Engine_ActorEnableActionCallback(8, (const u8 *)2);
                Engine_ActorSetPosition(10, 0, 0);
            } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
                Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
                Engine_ActorSetPosition(9, 0xae0000, 0xa40000);
                Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
                Engine_ActorSetAnimation(9, 5);
                Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x980000);
                Object_GetById(8)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82b)) {
                    FieldScene_RunPrimaryScriptChoreography();
                }
            } else {
                Call6(Engine_MapCopyCellAttributes, 10, 7, 1, 1, 10, 8);
                Engine_MapCopyCellsTo(3, 125, 9, 69, 3, 3);
                Engine_MapRedraw();
                Engine_TaskWait(1);
                if (Engine_GameFlagIsSet(0x82c)) {
                    Engine_ActorSetPosition(8, 0x950000, 0x740000);
                    Object_GetById(8)->facing = 0;
                    Object_GetById(9)->unknown_66 = 0;
                    Engine_ActorEnableActionCallback(9, ImiruMura_ActorScriptA);
                } else {
                    Engine_ActorEnableActionCallback(8, (const u8 *)2);
                }
            }
        } else if (entrance == 2) {
            if (!Engine_GameFlagIsSet(0x881)) {
                Object_GetById(11)->unknown_66 = 1;
                Engine_ActorEnableActionCallback(11, ImiruMura_ActorScriptA);
            }
        } else if (entrance == 4) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 12, 0x16c0000, 0x2420000);
                Engine_ActorSetSpritePriority(12, 2);
                Object_GetById(12)->collision_flags |= 4;
                Engine_MapCopyCellsTo(6, 125, 22, 88, 3, 3);
                Call3(Engine_ActorSetPosition, 13, 0x1ec0000, 0x2420000);
                Engine_ActorSetSpritePriority(13, 2);
                Object_GetById(13)->collision_flags |= 4;
                Engine_MapCopyCellsTo(9, 125, 28, 88, 3, 3);
            } else {
                Object_GetById(12)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
                Engine_ActorSetAnimation(12, 5);
                Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
                Engine_ActorSetAnimation(13, 5);
            }
        } else if (entrance == 3) {
            if (Engine_GameFlagIsSet(0x881)) {
                Call3(Engine_ActorSetPosition, 15, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(15, 2);
                Object_GetById(15)->collision_flags |= 4;
                Engine_ActorSetPosition(14, 0x1980000, 0x1080000);
                Object_GetById(14)->facing = 0x1000;
                Engine_MapCopyCellsTo(12, 125, 26, 70, 3, 3);
            } else {
                Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1020000);
                Engine_ActorSetSpritePriority(14, 2);
                Object_GetById(14)->collision_flags |= 4;
                Object_GetById(15)->scale_x = -0x10000;
                Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
                Engine_ActorSetAnimation(15, 5);
            }
        } else if (entrance == 7) {
            if (Engine_GameFlagIsSet(0x881)) {
                Object_GetById(20)->facing = 0x3000;
                if (!Engine_GameFlagIsSet(0x82e)) {
                    Call3(Engine_ActorSetPosition, 20, 0x28a0000, 0xa10000);
                    FieldScene_RunThreeActorChoreography();
                } else {
                    Call3(Engine_ActorSetPosition, 20, 0x2840000, 0xa60000);
                }
            }
        }
    }
    return 0;
}
