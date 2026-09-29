/* Bunza will not wait for the party any longer. */
#include "CAVE.H"
extern u8 MsgRunpaMeaningWontRide[];

/* Bunza cannot wait any longer, and Mia asks whether the party stays. */
u8 Bunza_CannotWait(void)
{
    s32 warning;

    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)MsgRunpaMeaningWontRide;
    Event_SetMessage(warning + WARNING_IVAN_ASKS_IF_STAYING);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH - FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_BUNZA, ANIM_SHAKE_HEAD);
    Event_SetMessage(warning + WARNING_BUNZA_CANNOT_WAIT);
    Event_ShowMessage(ACTOR_BUNZA, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage(warning + WARNING_MIA_ASKS_ABOUT_BUSINESS);
    Event_OpenMessage(ACTOR_MIA, 0);
    return Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0;
}
