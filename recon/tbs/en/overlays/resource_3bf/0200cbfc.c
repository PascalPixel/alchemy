/* Draft of RunActor20SceneSequence, resource_3bf at 0x0200cbfc, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines (Engine_ActorJump,
 * Object_RefreshSelectorById, Engine_EventOpenMessage,
 * Engine_EventChooseYesNo).
 * The listing keeps these rows. */
#include "FORTRESS.H"
extern u8 MsgRunpaDontLookNearly[];
extern u8 MsgRunpaWhWhWho[];
extern u8 MsgRunpaWontTellAnyone[];

void RunActor20SceneSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (GameFlag_IsSet(0x226) != 0) {
        Event_SetMessage((s32)MsgRunpaWontTellAnyone);
        Event_ShowMessage(20, 0);
    } else {
        Event_Begin();
        Actor_FaceActor(20, ACTOR_PARTY_LEADER, 0);
        if (GameFlag_IsSet(0x227) == 0) {
            Actor_Jump(20, 4, 0);
            Actor_Stop(20);
            Object_RefreshSelectorById(20);
            Event_Wait(20);
            base5_242e = (s32)MsgRunpaWhWhWho;
            Event_SetMessage(base5_242e);
            Event_ShowMessage(20, 0);
            Actor_ShowEmote(20, 0x102, 30);
            Event_SetMessage((base5_242e + 1));
            Event_ShowMessage(20, 0);
            Event_Wait(30);
            Actor_SetAnimation(20, 4);
            Event_Wait(30);
        }
        base5_2430 = (s32)MsgRunpaDontLookNearly;
        Event_SetMessage(base5_2430);
        Event_ShowMessage(20, 0);
        Actor_ShowEmote(20, 0x101, 40);
        Event_SetMessage((base5_2430 + 1));
        Event_OpenMessage(20, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_2430 + 2));
            Event_OpenMessage(20, 0);
            GameFlag_Set(0x226);
        } else {
            Event_SetMessage((base5_2430 + 3));
            Event_OpenMessage(20, 0);
        }
        GameFlag_Set(0x227);
        Event_End();
    }
}
