/* resource_3b8:0200be40..0200bf84 (324 bytes), still linked from the
 * listing. Remaining difference: its messages have catalogue names now; 147 halfwords
 * still differ from the ROM, and it names symbols no link defines (Call1,
 * Call0, Call3, Value2, ...); it also lacks declarations it needs to compile. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgTorebiArent[];
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Call1(Engine_GameFlagSet, 2412);
    Call0(Engine_EventBegin);
    Call0(Battle_ResetEffectCounterFar);
    Call3(Engine_ActorFaceDirection, 8, 20480, 0);
    Call3(Engine_ActorFaceDirection, 9, 12288, 0);
    Call3(Engine_ActorWalkToAndWait, 0, 200, 272);
    Call3(Engine_ActorFaceDirection, 0, 49152, 0);
    Call1(Engine_EventWait, 20);
    p = (s32)MsgTorebiArent;
    Engine_EventSetMessage(p);
    Value2(Engine_EventOpenMessage, 8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Call1(Engine_EventWait, 20);
        Engine_EventSetMessage(p + 1);
        Call2(Engine_EventShowMessage, 8, 0);
    } else {
        Value1(Engine_EventWait, 20);
        Event_SetMessage(p + 2);
        Call2(Engine_EventShowMessage, 8, 0);
        Call1(Engine_EventWait, 20);
        Call3(Engine_ActorFaceEachOther, 8, 9, 60);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Call1(Engine_EventWait, 40);
        Actor_RunRepeatedMotion(9, 2);
        Call1(Engine_EventWait, 30);
        Call3(Engine_ActorFaceEachOther, 8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Call1(Engine_EventWait, 30);
        Actor_ShowEmote(8, 258, 50);
        Call3(Engine_ActorFaceDirection, 8, 20480, 0);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Call1(Engine_EventWait, 20);
        Call2(Engine_ActorSetAnimationAndWait, 8, 4);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 8, 0);
        Call1(Engine_EventWait, 10);
        Call2(Engine_ActorRunRepeatedMotion, 8, 2);
        Call1(Engine_EventWait, 20);
        Call2(Engine_EventShowMessage, 8, 0);
    }
    Call0(Engine_EventEnd);
}
