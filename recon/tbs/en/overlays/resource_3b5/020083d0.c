/* Draft of resource_3b5 0x020083d0, built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H.
 * Remaining difference: as 0x020082f0, GCC hoists each branch's message constant
 * above its flag test, where the ROM loads it after the branch; 100 bytes differ.
 * The listing keeps these rows. */
#include "MACHI.H"

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
            msg = 0x2366;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = 0x21e3;
        } else {
            msg = 0x1f96;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = 0x2372;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = 0x21f6;
        } else {
            msg = 0x1fab;
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
