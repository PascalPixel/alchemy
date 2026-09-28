/* Draft of FieldScene_RunSupplementalSequenceTwo, resource_3b5 at 0x020082f0, built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H.
 * Remaining difference: the ROM loads each branch's message number into r0 and
 * joins at one call; with the numbers as C constants GCC hoists each load above
 * its flag test (it only hoists constants, not link-time values), 100 bytes differ.
 * The listing keeps these rows. */
#include "MACHI.H"

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
            msg = 0x2365;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = 0x21e2;
        } else {
            msg = 0x1f95;
        }
    } else {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            msg = 0x2371;
        } else if (Engine_GameFlagIsSet(0x962) != 0) {
            msg = 0x21f5;
        } else {
            msg = 0x1faa;
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
