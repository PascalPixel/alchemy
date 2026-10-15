/* Draft of overlay 670's Func_02004294, differ 25 of 151: everything but the wait loop matches.
 * The native's loop is rotated without a copied test (b to the test, which reuses the pointer
 * loaded for the zero store) and reloads Data_02007950's address and 0xffffc000 in the body;
 * here jump.c copies the test ahead of the loop and the address stays in r5. A pointer variable
 * reloaded at the end of the body changes nothing (differ 29). Func_020047e4 waits in the same
 * loop, there rotated with the test copied. */
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

void Func_02004294(void)
{
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner), 0);
    Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
    Kibombo_CamMove(-1, -1, -1, 0);
    Func_02005bbc()->motionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(Object_GetById(27), 0);
    Object_GetById(27)->motionFlags = 0;
    Kibombo_ObjSetPos(Object_GetById(27), 0x1090000, 0x180000, 172 << 17);
    Func_02003640(1, 1);
    WaitFrames(1);
    if (Data_02007950 != 0) {
        Data_02007950->y += 0x200000;
        WaitFrames(1);
        Data_02007950->motionFlags = 4;
    }
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
    Kibombo_Wait(40);
    Kibombo_SePlay(161);
    Func_02003640(0, 1);
    Kibombo_Wait(20);
    Kibombo_ObjEmoteWait(17, 0x102);
    Kibombo_Wait(20);
    Func_02005b4c((s32)MsgYouAreStill);
    Kibombo_MessagePut(17, 0);
    if (Data_02007950 != 0) {
        Data_02007950->motionFlags = 0;
        while (Data_02007950->y > 0x240000) {
            Kibombo_ObjArm(17, 0xd000, 0);
            Data_02007950->y += 0xffffc000;
            WaitFrames(1);
        }
    }
    Motion_SetModeAndWaitAnimation(17, 4);
    Kibombo_MessagePut(17, 0);
    Kibombo_ObjTurn(17, 0x3000);
    Kibombo_MessagePut(0x4000 + 17, 0);
    GameFlag_ClearBit(0x8f7);
    Kibombo_SePlay(11);
    Kibombo_Wait(80);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005bc4(11);
}
