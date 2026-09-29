#include "SORU.H"
extern u8 MsgSoruMinotaurReliefBothEyes[];
extern u8 MsgSoruMinotaurReliefOneEye[];
extern u8 MsgSoruSetSmallGem[];

void FieldScene_RunScene37fSequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Event_Begin();
    v5 = 3;
    v6 = 2;
    Audio_PlayCue(181);
    Map_CopyCellsTo(16, 28, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 30, 21, 3, v5, v6);
    Task_Wait(10);
    Map_CopyCellsTo(16, 32, 21, 3, v5, v6);
    Task_Wait(10);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
    Event_Wait(10);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(2);
    Event_End();
}

void SceneDialogue_RunFlag81aMessageBranch(void)
{

    Event_Begin();

    if (GameFlag_IsSet(0x81a) != 0) {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
        if (GameFlag_IsSet(0xf01) != 0) {
            u16 *p = (u16 *)(gWork + 370);
            u16 val = 1;
            *p = val;
        }
    }

    Event_End();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 id;
    s32 v5;
    s32 v6;

    if (GameFlag_IsSet(0xf01) == 0) {
    } else {
        if (GameFlag_IsSet(0x81a) != 0) {
        } else {
            Event_Begin();
            Battle_ResetEffectCounter();
            v5 = 1;
            Audio_PlayCue(182);
            Map_CopyCellsTo(0, 70, 30, 42, v5, v5);
            Map_Redraw();
            Event_Wait(40);
            id = (s32)MsgSoruSetSmallGem;
            Message_ShowCentered(id, 1);
            Event_Wait(20);
            v6 = 3;
            Audio_PlayCue(183);
            Map_CopyCellsTo(0, 29, 3, 1, v6, 2);
            Map_CopyCellAttributes(0, 29, 3, 2, v6, v5);
            Map_CopyCellsTo(1, 109, 4, 81, v5, v5);
            Map_Redraw();
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Event_Wait(20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
            Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
            Event_Wait(20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
            Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(40);
            Message_ShowCentered(id + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(0x81a);
            Event_End();
        }
    }
}

void FieldScene_RunFlag821Dialogue(void)
{

    u8 *work;

    Event_Begin();

    if (GameFlag_IsSet(0x821) != 0) {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefBothEyes, 1);
    } else if (GameFlag_IsSet(0xf02) != 0) {
        work = gWork;
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an
             * s32 value local, in that order.  Storing the literal straight
             * into the halfword builds the constant in HImode and fetches it
             * from the literal pool, which costs a pool word; splitting the
             * address out first also fixes which register holds it.
             */
            u16 *frame = (u16 *)(work + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Message_ShowCentered((s32)MsgSoruMinotaurReliefOneEye, 1);
    }

    Event_End();
}

