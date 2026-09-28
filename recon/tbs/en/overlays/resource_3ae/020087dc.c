/* Draft of resource_3ae 0x020087dc (FieldScene_RunScene3ae_020007dc), built with
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x6b and 0x70 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "KAREI.H"

extern u8 Data_0000006b[];
extern u8 Data_00000070[];

void FieldScene_RunScene3ae_020007dc(void)
{
    u8 *work;

    Event_Begin();
    Audio_PlayCue(158);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 3);
    work = (u8 *)Data_02000240;
    if (*(s16 *)(work + 0x1c0) == (s32)Data_0000006b) {
        Actor_WalkTo(ACTOR_PARTY_LEADER, 0x130, 0x570);
        Map_AnimateCells(0x20096b8, 78, 86);
    } else {
        if (*(s16 *)(work + 0x1c0) == (s32)Data_00000070) {
            Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
            Map_AnimateCells(0x20096ce, 74, 9);
        }
    }
    Event_Wait(16);
    Event_RequestExit(3);
    Event_End();
}
