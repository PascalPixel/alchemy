#include "HASHIRA.H"

void FieldScene_RunScene3b3_02001fd4(void)
{
    Event_Begin();
    if (FieldScene_RunScene3b3SequenceD() == 0) {
        *((u8 *)Engine_ActorGet(0) + 85) &= 254;
        *((u8 *)Engine_ActorGet(0) + 35) &= 254;
        StagedActor_AdvancePair();
        TakaraHashira_UpdatePillarActors();
        {
            u8 bits = 1;
            u8 *flags = (u8 *)Actor_Get(ACTOR_PARTY_LEADER) + 85;
            u8 value = *flags;

            value |= bits;
            *flags = value;
            flags = (u8 *)Actor_Get(ACTOR_PARTY_LEADER) + 35;
            bits |= *flags;
            *flags = bits;
        }
    }
    Event_End();
}

/* Complete one-call wrapper through interworking return and alignment. */
void FieldScene_RunSingleStep(void)
{
    battle_owner_69();
}
