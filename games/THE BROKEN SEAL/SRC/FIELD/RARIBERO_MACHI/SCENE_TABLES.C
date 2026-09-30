#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgRariberoOhhTheyTookShebaHeaded[];

/* Lalivero's scene tables and the first actor setups that precede
   FLAGGED_CUE.C in the overlay. */

void BattleFx_RunPageEffectForSlot(s32, s32, s32);

/* The scene's tables, laid out after the code; the actor table has an
 * alternate once flag 0x9a7 is set. */
extern u8 Placement_Scripts[];
extern u8 Placement_Messages[];
extern u8 Placement_Actors[];
extern u8 Placement_Actors9a7[];
/* The lamenting villager's action table. */
extern const u8 RariberoMachi_LamentActions[];

void SceneActor_SetActor23Params2And6(void)
{
    BattleFx_RunPageEffectForSlot(0x17, 2, 6);
}

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

s32 SceneData_SelectTableByFlag9a7(void)
{
    if (GameFlag_IsSet(0x9A7) != 0) {
        return (s32)Placement_Actors9a7;
    }
    return (s32)Placement_Actors;
}

void SceneActor_StartLament(s32 actor)
{
    void Actor_FaceDirection();

    struct FieldActor *object;

    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_x = 0x10000;
    object = (struct FieldActor *)Actor_Get(actor);
    object->scale_y = 0x10000;
    Event_SetMessage((s32)MsgRariberoOhhTheyTookShebaHeaded);
    Event_ShowMessage(actor, 0);
    Actor_FaceDirection(actor, 0xc000, 0);
    Event_Wait(20);
    Engine_ActorEnableActionCallback(actor, RariberoMachi_LamentActions);
}
