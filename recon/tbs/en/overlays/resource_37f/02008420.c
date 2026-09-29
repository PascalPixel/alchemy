/* Draft of resource_37f 0x02008420 (FieldScene_RunSupplementalSequenceTwo):
 * it matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgSoruSetSmallGem). The listing keeps
 * these rows until the draft is adopted. */
#include "SORU.H"
extern u8 MsgSoruSetSmallGem[];

void FieldScene_RunSupplementalSequenceTwo(void)
{

    s32 byte_pair_addr;

    if (GameFlag_IsSet(3842) == 0) {
    } else {
        if (GameFlag_IsSet(GATE_ID)!= 0) {
        } else {
            Event_Begin();
            Value0(Battle_ResetEffectCounter);
            Value1(Engine_AudioPlayCue, 182);
            Value6(Engine_MapCopyCellsTo, 0, 71, 100, 71, 1, 1);
            Value0(Engine_MapRedraw);
            Value1(Engine_EventWait, 40);
            /* The message number is too wide for an immediate. */
            byte_pair_addr = (s32)MsgSoruSetSmallGem;
            Value2(Engine_MessageShowCentered, byte_pair_addr, 1);
            Event_Wait(20);
            Value1(Engine_AudioPlayCue, 183);
            Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
            Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
            Call0(Engine_MapRedraw);
            Value3(Engine_WorkSetValuesIfNonNegative, 65536, 65536, 65536);
            Value1(Engine_EventWait, 20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
            Value3(Engine_WorkSetValuesIfNonNegative, 131072, 131072, 65536);
            Value1(Engine_EventWait, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 32768, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 10);
            Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
            Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
            Value3(Engine_WorkSetValuesIfNonNegative, -1, -1, 58982);
            Value1(Engine_EventWait, 40);
            Value2(Engine_MessageShowCentered, byte_pair_addr + 1, 1);
            GameFlag_Set(0x143);
            GameFlag_Set(GATE_ID);
            Event_End();
        }
    }
}

