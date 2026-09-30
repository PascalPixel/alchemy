/* Actor 8's question once flag 2412 is set: the leader walks over, and a
 * no brings actors 8 and 9 into a longer exchange. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgTorebiArent[];
#include "KYUDEN.H"
#include "CALL.H"

void RunSupplementalSequenceOne(void)
{
    s32 p;
    Engine_GameFlagSet(2412);
    Engine_EventBegin();
    Battle_ResetEffectCounterFar();
    Call3(Engine_ActorFaceDirection, 8, 20480, 0);
    Call3(Engine_ActorFaceDirection, 9, 12288, 0);
    Call3(Engine_ActorWalkToAndWait, 0, 200, 272);
    Engine_ActorFaceDirection(0, 49152, 0);
    Engine_EventWait(20);
    p = (s32)MsgTorebiArent;
    Engine_EventSetMessage(p);
    Engine_EventOpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_EventSetMessage(p + 1);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_EventWait(20);
        Event_SetMessage(p + 2);
        Engine_EventShowMessage(8, 0);
        Engine_EventWait(20);
        Engine_ActorFaceEachOther(8, 9, 60);
        Call3(Engine_ActorFaceDirection, 9, 12288, 0);
        Engine_EventWait(40);
        Actor_RunRepeatedMotion(9, 2);
        Engine_EventWait(30);
        Engine_ActorFaceEachOther(8, 9, 30);
        Actor_SetAnimationAndWait(9, 3);
        Engine_EventWait(30);
        Actor_ShowEmote(8, 258, 50);
        Call3(Engine_ActorFaceDirection, 8, 20480, 0);
        Engine_ActorFaceDirection(9, 12288, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 4);
        Engine_EventWait(20);
        Engine_EventShowMessage(8, 0);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(8, 0);
    }
    Engine_EventEnd();
}
