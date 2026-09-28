#include "VILLAGE.H"

void WestGuard_Talk(void)
{
    Event_Begin();
    Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(MSG_WEST_GUARD_ASKS_ABOUT_ENTERING);
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
        Event_SetMessage(MSG_EAST_GUARD_DEMANDS_AUTHORIZATION);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage(MSG_EAST_GUARD_ASKS_IF_FROM_KALAY);
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
            thoughts = MSG_EAST_GUARD_FEARS_BLAME;
        } else {
            thoughts = MSG_EAST_GUARD_TRUSTS_CAVE_GATE;
        }
        Event_SetMessage(thoughts);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage(MSG_EAST_GUARD_THINKS_MERCHANT_HARMLESS);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

void VillagerA_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_A_ASKS_HOW_LONG);
    Event_AskYesNo(ACTOR_VILLAGER_A, 0);
    Event_End();
}

void VillagerC_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_C_ASKS_ABOUT_KIDNAPPING);
    Event_AskYesNo(ACTOR_VILLAGER_C, 0);
    Event_End();
}

void VillagerG_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_G_ASKS_ABOUT_DONPA);
    Event_AskYesNo(ACTOR_VILLAGER_G, 0);
    Event_End();
}

void VillagerB_Shivers(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(ACTOR_VILLAGER_B, 3);
    Event_SetMessage(MSG_VILLAGER_B_SHIVERS);
    Event_ShowMessage(ACTOR_VILLAGER_B, 0);
    Event_End();
}

void VillagerD_Talk(void)
{
    Event_Begin();
    Event_SetMessage(MSG_VILLAGER_D_ASKS_ABOUT_COMMOTION);
    Event_AskYesNo(ACTOR_VILLAGER_D, 0);
    Event_End();
}
