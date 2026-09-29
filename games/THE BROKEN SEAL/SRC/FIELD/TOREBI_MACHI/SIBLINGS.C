#include "MACHI.H"
extern u8 MsgTorebiBrotherMeanDuring[];
extern u8 MsgTorebiHaHaHa[];
extern u8 MsgTorebiHoorayFinalsSeven[];
extern u8 MsgTorebiMainStreetTolbi[];
extern u8 MsgTorebiMamaToldShare[];
extern u8 MsgTorebiMamaWhyListen[];
extern u8 MsgTorebiOldestShouldntShare[];
extern u8 MsgTorebiWaahBigBrother[];
extern u8 MsgTorebiWaahSaidMoney[];
extern u8 MsgTorebiWheeFestivalColosso[];
extern u8 MsgTorebiWinBigUsed[];
extern u8 MsgTorebiYayEasyRun[];

void FieldScene_RunSupplementalSequenceTwo(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

    /* FAKEMATCH: the event work is read into a local before the actor
     * lookup, where the reference loads it. */
    work = gEventWork;
    actor = Engine_ActorGet(16);
    facing = actor->facing;
    Engine_EventBegin();
    actor->proximity_flags |= 2;
    if (work->psynergy_request == 0) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiYayEasyRun;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiHoorayFinalsSeven;
        } else {
            msg = (s32)MsgTorebiWheeFestivalColosso;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = (s32)MsgTorebiMainStreetTolbi;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = (s32)MsgTorebiWinBigUsed;
        } else {
            msg = (s32)MsgTorebiOldestShouldntShare;
        }
    }
    Engine_EventSetMessage(msg);
    Engine_ActorSetAnimation(16, 0);
    Engine_ActorFaceEachOther(16, 0, 2);
    Engine_EventShowMessageAndWait(16, 0, 10);
    actor->facing = facing;
    Engine_TaskWait(1);
    actor->proximity_flags &= 1;
    Engine_EventEnd();
}

void FieldScene_RunSiblingsTalk(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

    /* FAKEMATCH: the event work is read into a local before the actor
     * lookup, where the reference loads it. */
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
