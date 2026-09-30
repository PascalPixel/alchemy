/* Setting the small gem into the empty socket opens the gate. */
#include "SORU.H"
#include "CALL.H"
extern u8 MsgSoruSetSmallGem[];

void SoruIriguchi_SetSmallGem(void)
{
    s32 message;

    if (GameFlag_IsSet(3842) != 0 && GameFlag_IsSet(GATE_ID) == 0) {
        Event_Begin();
        Battle_ResetEffectCounter();
        Engine_AudioPlayCue(182);
        Engine_MapCopyCellsTo(0, 71, 100, 71, 1, 1);
        Engine_MapRedraw();
        Engine_EventWait(40);
        message = (s32)MsgSoruSetSmallGem;
        Engine_MessageShowCentered(message, 1);
        Event_Wait(20);
        Engine_AudioPlayCue(183);
        Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
        Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
        Engine_MapRedraw();
        Value3(Engine_WorkSetValuesIfNonNegative, 65536, 65536, 65536);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
        Value3(Engine_WorkSetValuesIfNonNegative, 131072, 131072, 65536);
        Engine_EventWait(20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
        Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
        Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
        Value3(Engine_WorkSetValuesIfNonNegative, -1, -1, 58982);
        Engine_EventWait(40);
        Engine_MessageShowCentered(message + 1, 1);
        GameFlag_Set(0x143);
        GameFlag_Set(GATE_ID);
        Event_End();
    }
}

