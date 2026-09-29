/* Draft of FieldScene_DispatchActorUpdate, resource_3bf at 0x0200cda0, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM loads the scene numbers 0xa1-0xa3 it compares
 * from its literal pool, as link-time values; GCC compares immediates.
 * The listing keeps these rows. */
#include "FORTRESS.H"

s32 FieldScene_DispatchActorUpdate(void)
{
    struct ObjectRuntime *actor;

    gRunpaJoRandomPick = (u32)Random_Next() * 7 >> 16;
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == 0xa1) {
        Scene_Call1(Map_SetWorkFlagBits9To11, 0xe00);
        FieldScene_InstallSceneTasks();
    }
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == 0xa1)
        FieldScene_SetupActorsForScene();
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == 0xa2)
        FieldScene_RestoreActorsFromFlags();
    if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.first == 0xa3) {
        ((struct DispatcherEventRuntime*)gEventWork)->value_1c0 = 0x204;
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorFaceDirection(12, 0, 0);
        Object_SetModeById(12, 0);
        ActivateFiveActorGroupFromFlags();
        actor = Object_GetById(8);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        actor = Object_GetById(9);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        actor = Object_GetById(10);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        Scene_Call1(Map_SetWorkFlagBits9To11, 0xe00);
        if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.second == 4) {
            Scene_Call1(Map_SetWorkFlagBits9To11, 0xc00);
            FieldScene_RunMainScriptSequence();
        }
        if ((s16)(*(union DispatcherEventWork *)&gGameState).pair.second == 3) {
            Scene_Call1(Map_SetWorkFlagBits9To11, 0xc00);
            if (Engine_GameFlagIsSet(0x941) != 0) {
                Engine_ActorSetPosition(12, 0, 0);
                Scene_Call3(Engine_ActorSetPosition, 16, 0x1b00000, 0x1580000);
                Scene_Call3(Engine_ActorFaceDirection, 16, 0x5000, 0);
                Scene_Call3(Engine_ActorSetPosition, 13, 0x1c80000, 0x1200000);
                Scene_Call3(Engine_ActorFaceDirection, 13, 0x5000, 0);
                Scene_Call3(Engine_ActorSetPosition, 17, 0x1c80000, 0x1400000);
                Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
            }
        }
        actor = Object_GetById(15);
        if (actor != 0)
            Engine_ActorSetSpriteFlags(actor, 0);
        actor->unknown_23 = 2;
        *(s32 *)&actor->unknown_18[0] = 0xcccc;
    }
    return 0;
}
