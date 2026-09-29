/* Draft of resource_3b5 0x020083d0 (Func_020003d0): it matches the ROM byte
 * for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgTorebiBrotherMeanDuring, MsgTorebiHaHaHa,
 * MsgTorebiMamaToldShare, MsgTorebiMamaWhyListen, MsgTorebiWaahBigBrother,
 * MsgTorebiWaahSaidMoney). The listing keeps these rows until the draft is
 * adopted. */
#include "MACHI.H"
extern u8 MsgTorebiBrotherMeanDuring[];
extern u8 MsgTorebiHaHaHa[];
extern u8 MsgTorebiMamaToldShare[];
extern u8 MsgTorebiMamaWhyListen[];
extern u8 MsgTorebiWaahBigBrother[];
extern u8 MsgTorebiWaahSaidMoney[];

void Func_020003d0(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

    work = gEventWork;
    actor = Engine_ActorGet(17);
    facing = actor->facing;
    Engine_EventBegin();
    actor->proximity_flags |= 2;
    if (work->psynergy_request == 0) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiHaHaHa;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiWaahBigBrother;
        } else {
            msg = (s32)MsgTorebiWaahSaidMoney;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiBrotherMeanDuring;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiMamaWhyListen;
        } else {
            msg = (s32)MsgTorebiMamaToldShare;
        }
    }
    Engine_EventSetMessage(msg);
    Engine_ActorSetAnimation(17, 0);
    Engine_ActorFaceEachOther(17, 0, 2);
    Engine_EventShowMessageAndWait(17, 0, 10);
    actor->facing = facing;
    Engine_TaskWait(1);
    actor->proximity_flags &= 1;
    Engine_EventEnd();
}
