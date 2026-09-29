/* Draft of resource_3b5 0x020082f0 (FieldScene_RunSupplementalSequenceTwo):
 * it matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgTorebiHoorayFinalsSeven,
 * MsgTorebiMainStreetTolbi, MsgTorebiOldestShouldntShare,
 * MsgTorebiWheeFestivalColosso, MsgTorebiWinBigUsed, MsgTorebiYayEasyRun).
 * The listing keeps these rows until the draft is adopted. */
#include "MACHI.H"
extern u8 MsgTorebiHoorayFinalsSeven[];
extern u8 MsgTorebiMainStreetTolbi[];
extern u8 MsgTorebiOldestShouldntShare[];
extern u8 MsgTorebiWheeFestivalColosso[];
extern u8 MsgTorebiWinBigUsed[];
extern u8 MsgTorebiYayEasyRun[];

void FieldScene_RunSupplementalSequenceTwo(void)
{

    struct EventWork *work;
    struct SceneActor *actor;
    s16 facing;
    s32 msg;

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
