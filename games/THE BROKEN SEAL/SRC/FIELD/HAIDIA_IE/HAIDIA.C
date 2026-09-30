/* Facing, scene tables and the villagers' first lines. */
#include "HAIDIA.H"
extern u8 MsgHaidiaADifficultTimeThreeYears[];
extern u8 MsgHaidiaCheckedThePsynergyStone[];
extern u8 MsgHaidiaDidTheTravelersMeetThe[];
extern u8 MsgHaidiaMeditateOnMtAlephDaily[];
extern u8 MsgHaidiaPartyPpRestored[];

s32 Object_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 facing_delta;
    u16 old_facing;
    s32 target_facing;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        target_facing = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old_facing = object->facing;
        facing_delta = (s16)(target_facing - old_facing);
        if (facing_delta != 0) {
            if (facing_delta > 0x1000) {
                facing_delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (facing_delta < -0x1000) {
                facing_delta = -0x1000;
            }
            object->facing = (u16)(old_facing + facing_delta);
        }
    }
    return 1;
}

u8 *SceneData_GetTableAfa0(void)
{
    return Data_0200afa0;
}

s32 Func_02000090(void)
{
    return 0;
}

void *SceneData_SelectTableByFlag834(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}

void *SceneData_SelectTableByFlags834And87a(void)
{
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200b380;
    }
    if (gGameState.entrance == 12) {
        return Data_0200b560;
    }
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

void Scene_CheckPsynergyStone(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgHaidiaCheckedThePsynergyStone, 1);
    Audio_PlayCue(126);
    BattleParty_ApplyDrain(0x3e7, 0);
    Event_Wait(10);
    Message_ShowCentered((s32)MsgHaidiaPartyPpRestored, 1);
    UiWork_FinalizePendingCore();
    GameFlag_Clear(322);
    Event_End();
}

void *SceneData_SelectTableByFlags87a_815_834(void)
{
    if (GameFlag_IsSet(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return Data_0200bb3c;
    }
    if (gGameState.entrance == 12) {
        return Data_0200bb30;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

void Villager_AskAboutMeditation(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaMeditateOnMtAlephDaily);
    Actor_FaceEachOther(23, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(23, 0);
    Event_End();
}

void Villager_RecallThreeYearsAgo(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaADifficultTimeThreeYears);
    Actor_FaceEachOther(24, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(24, 0);
    Event_End();
}

void Villager_AskAboutTheTravelers(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaDidTheTravelersMeetThe);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Event_AskYesNo(15, 0);
    Event_End();
}
