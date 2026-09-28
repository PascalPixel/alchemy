/* Draft of resource_3ab 0x020088ec..0x02008980 (148 bytes with pool),
 * LeftGuard_Talk; the listing keeps the rows. Remaining difference: the
 * reference loads message 0x24db into r5 after setting up the emote call, as
 * a link-time message symbol is loaded; the constant is forced to the pool at
 * expansion and scheduled before the emote arguments (12 bytes differ, same
 * size). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

/*
 * While Hammet travels with the party, the left guard thinks he has seen
 * him before.
 */
void LeftGuard_Talk(void)
{
    s32 recognition;

    if (gGameState.cloaked != 0) {
        Event_SetMessage(MSG_LEFT_GUARD_HEARS_SOMEONE);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
               && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
        recognition = MSG_LEFT_GUARD_RECOGNIZES_HAMMET;
        Event_SetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage(recognition + RECOGNITION_IMPOSSIBLE);
        GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
    } else {
        Event_SetMessage(MSG_LEFT_GUARD_RESENTS_DODONPA);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}
