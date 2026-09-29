#include "VILLAGE.H"
extern u8 MsgRunpaEastGuardAsksIfFrom[];
extern u8 MsgRunpaEastGuardDemandsAuthorization[];
extern u8 MsgRunpaEastGuardFearsBlame[];
extern u8 MsgRunpaEastGuardThinksMerchantHarmless[];
extern u8 MsgRunpaEastGuardTrustsCaveGate[];
extern u8 MsgRunpaVillagerAAsksHowLong[];
extern u8 MsgRunpaVillagerBShivers[];
extern u8 MsgRunpaVillagerCAsksAboutKidnapping[];
extern u8 MsgRunpaVillagerDAsksAboutCommotion[];
extern u8 MsgRunpaVillagerGAsksAboutDonpa[];
extern u8 MsgRunpaWestGuardAsksAboutEntering[];
extern u8 MsgRunpaLeftGuardHearsSomeone[];
extern u8 MsgRunpaLeftGuardRecognizesHammet[];
extern u8 MsgRunpaLeftGuardResentsDodonpa[];

void WestGuard_Talk(void)
{
    Event_Begin();
    Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage((s32)MsgRunpaWestGuardAsksAboutEntering);
    Event_AskYesNo(ACTOR_WEST_GUARD, 0);
    Event_End();
}

/*
 * Once the guards suspect Kalay, the east guard questions the party; each
 * reply draws a different line from him.
 */
void EastGuard_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
        Event_SetMessage((s32)MsgRunpaEastGuardDemandsAuthorization);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaEastGuardAsksIfFrom);
        Event_OpenMessage(ACTOR_EAST_GUARD, 0);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            gEventWork->message++;
            Event_OpenMessage(ACTOR_EAST_GUARD, 0);
            if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
                gEventWork->message++;
            }
        }
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
    Event_End();
}

void EastGuard_MindRead(void)
{
    s32 thoughts;

    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) == 0) {
        thoughts = GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY);
        if (thoughts == 0) {
            thoughts = (s32)MsgRunpaEastGuardFearsBlame;
        } else {
            thoughts = (s32)MsgRunpaEastGuardTrustsCaveGate;
        }
        Event_SetMessage(thoughts);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaEastGuardThinksMerchantHarmless);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

void VillagerA_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerAAsksHowLong);
    Event_AskYesNo(ACTOR_VILLAGER_A, 0);
    Event_End();
}

void VillagerC_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerCAsksAboutKidnapping);
    Event_AskYesNo(ACTOR_VILLAGER_C, 0);
    Event_End();
}

void VillagerG_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerGAsksAboutDonpa);
    Event_AskYesNo(ACTOR_VILLAGER_G, 0);
    Event_End();
}

void VillagerB_Shivers(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(ACTOR_VILLAGER_B, 3);
    Event_SetMessage((s32)MsgRunpaVillagerBShivers);
    Event_ShowMessage(ACTOR_VILLAGER_B, 0);
    Event_End();
}

void VillagerD_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerDAsksAboutCommotion);
    Event_AskYesNo(ACTOR_VILLAGER_D, 0);
    Event_End();
}

void LeftGuard_Talk(void)
{
    s32 recognition;

    if (gGameState.cloaked != 0) {
        Event_SetMessage((s32)MsgRunpaLeftGuardHearsSomeone);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
               && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
        recognition = (s32)MsgRunpaLeftGuardRecognizesHammet;
        Event_SetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage(recognition + RECOGNITION_IMPOSSIBLE);
        GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
    } else {
        Event_SetMessage((s32)MsgRunpaLeftGuardResentsDodonpa);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}
