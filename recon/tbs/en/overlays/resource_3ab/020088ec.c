/* Draft of resource_3ab 0x020088ec (LeftGuard_Talk): it matches the ROM byte
 * for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgRunpaLeftGuardHearsSomeone,
 * MsgRunpaLeftGuardRecognizesHammet, MsgRunpaLeftGuardResentsDodonpa). The
 * listing keeps these rows until the draft is adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"
extern u8 MsgRunpaLeftGuardHearsSomeone[];
extern u8 MsgRunpaLeftGuardRecognizesHammet[];
extern u8 MsgRunpaLeftGuardResentsDodonpa[];

/*
 * While Hammet travels with the party, the left guard thinks he has seen
 * him before.
 */
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
