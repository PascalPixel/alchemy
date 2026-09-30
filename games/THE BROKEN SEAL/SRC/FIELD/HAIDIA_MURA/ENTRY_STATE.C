#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void PaletteGlow_Update(s32 a, s32 b);
void FieldScene_RunLargeStagingSequence(void);
void InitializeStagedActorSceneOrbitingEffect(s32 actor);
void HaidiaMura_OpenVillagerLane(void);
void Scene_RepairTheHouse(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void BattleFx_SetQueuedSoundAndPlay(s32 value);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void FieldScene_RunScene373SequenceB(void);
s32 SceneActor_RunStep18WhenTargetSet();
extern u8 gHaidiaMuraActor22Actions[];

/* Vale entry: from entrance 16 run the staging scene; otherwise place the story actors and cells for the flags, start the village tasks and redraw. */
s32 HaidiaMura_ApplyEntryState(void)
{
    struct FieldActor *actor;
    s32 set;

    if (gGameState.entrance == 16) {
        PaletteGlow_Update(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
        FieldScene_RunLargeStagingSequence();
    } else {
        if (!Engine_GameFlagIsSet(0xfd0)) {
            if (!Engine_GameFlagIsSet(0x87a)) {
                InitializeStagedActorSceneOrbitingEffect(26);
            } else {
                InitializeStagedActorSceneOrbitingEffect(20);
            }
        }
        Engine_MapCopyCellsTo(2, 102, 84, 41, 2, 1);
        Engine_MapCopyCellsTo(1, 102, 83, 41, 1, 1);
        actor = Object_GetById((Engine_GameFlagIsSet(0x87a) != 0) + 20);
        Engine_ActorSetSpriteFlags(actor, 0);
        if (Engine_GameFlagIsSet(0x314)) {
            actor->x.fixed = 181 << 17;
        } else if (Engine_GameFlagIsSet(0x316)) {
            actor->x.fixed = 197 << 17;
        } else {
            actor->x.fixed = 189 << 17;
        }
        actor->z.fixed = 0x2480000;
        actor->y.fixed = 0xc00000;
        HaidiaMura_OpenVillagerLane();
        actor->unknown_22 = 3;
        actor->motion_flags = 0;
        Engine_TaskAddCallback(SceneActor_SetFlagByteBySlotZeroPosition, 0xc80);
        if (!Engine_GameFlagIsSet(0x87a)) {
            if (Value1(Engine_GameFlagIsSet, 0x815)) {
                actor = Object_GetById(21);
                Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
                actor->scale_x = 0x28f;
                actor->scale_y = 0x28f;
            }
            if (Engine_GameFlagIsSet(0x808)) {
                Engine_ActorSetPosition(15, 0, 0);
                Engine_ActorSetPosition(16, 0, 0);
                Engine_ActorSetPosition(17, 0, 0);
            }
            set = Engine_GameFlagIsSet(0x815);
            if (set == 0) {
                if (!Engine_GameFlagIsSet(0x109)) {
                    if (Engine_GameFlagIsSet(0x823)) {
                        Call3(Engine_ActorSetPosition, 22, 0x1000000, 0x1c80000);
                        Object_GetById(22)->update = (void *)SceneActor_RunStep18WhenTargetSet;
                        Engine_ActorEnableActionCallback(22, gHaidiaMuraActor22Actions);
                    }
                } else {
                    Object_GetById(22)->unknown_5b = set;
                    Engine_GameFlagClear(0x241);
                }
                if (gGameState.entrance != 16 && !Engine_GameFlagIsSet(0x87a)) {
                    Engine_TaskAddCallback(FieldScene_RunScene373SequenceB, 0xc80);
                }
            }
            if (!Value1(Engine_GameFlagIsSet, 0x308) && gGameState.entrance == 17) {
                Scene_RepairTheHouse();
                Engine_GameFlagSet(0x308);
            }
        }
        if (Engine_GameFlagIsSet(0x109)) {
            if (Engine_GameFlagIsSet(0x204)) {
                Call6(Engine_MapCopyCellAttributes, 49, 53, 8, 4, 20, 50);
            }
            if (Engine_GameFlagIsSet(0x210)) {
                SceneState_Send210AndApplyRectAt40x84();
            }
        }
        BattleFx_SetQueuedSoundAndPlay(170);
        Engine_MapRedraw();
        Engine_TaskWait(1);
    }
    return 0;
}
