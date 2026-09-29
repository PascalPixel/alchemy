/* Draft of resource_3a4 0x0200af10 (FieldScene_RunLateAuxiliarySequence), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: the ROM loads scene number 0x56 from the literal pool as a link-time value (no source defines it) and adds 0x22b to the game state's address at run time; the C folds gGameState+0x22b into one pool word.
 * The listing keeps these rows. */
#include "YAMA.H"

void Func_020069d4();
void Func_02006c72();
extern u8 Value_00000056;
void Func_02006c6a_a();

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void FieldScene_RunLateAuxiliarySequence(void)
{
    extern u8 ArutinYama_RiseTimer[];

    extern u8 Data_02000240[];

    Event_Begin();
    Camera_SetSpeed(39321, 4915);
    Camera_MoveTo(21495808, -1, 5701632, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 39321, 19660);
    Value3(Engine_ActorWalkToAndWait, 0, 328, 116);
    Value1(Engine_AudioPlayCue, 148);
    Value2(Func_020069d4, 33599213, 3200); /* main:080000d0 */
    Work_SetValuesIfNonNegative(65536, 65536, 65536); /* main:080091f0 */
    Actor_SetSpeed(8, 6553, 3276);
    Actor_SetSpeed(9, 6553, 3276);
    Actor_SetAnimation(8, 2);
    Actor_SetDestination(8, 328, 104);
    Value3(Engine_ActorSetDestination, 9, 328, 108);
    Value1(Engine_EventWait, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
    Value2(Engine_ActorStartRepeatedMotion, 0, 2);
    Value1(Engine_ActorWaitForMove, 8);
    do {
        ((u8 *)&gGameState)[0x22B] = 3;
    } while (0);
    Call2(Func_02006c72, (s32)&Value_00000056, 99);
    Call2(Func_02006c6a_a, 53, 3);
}
