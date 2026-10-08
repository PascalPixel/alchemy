#include "TYPES.H"
#include "CALL.H"
#include "FIELDOBJ.H"
#include "GAME_FLAGS.H"
#include "text/MSG_IDS.H"
#include "FIELD_EVENT.H"
#include "GAME_STATE.H"

void Korosseo_SelectSoloCompetitor(s32 id);

void Korosseo_FinishSoloRound(s32 id)
{
    struct EventWork *work;
    s32 slot;
    struct FieldActor *obj;
    s32 msg;
    s32 res;

    work = gEventWork;
    /* FAKEMATCH: needed to match: the speaker lookup is retained twice with both results unused. */
    Object_GetById(id);
    Object_GetById(id);
    slot = gGameState.selected_actor;
    obj = Object_GetById(slot);
    Engine_EventBegin();
    msg = (s32)MsgKorosseoCheering;
    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(id, 0);
    /* FAKEMATCH: needed to match: one work pointer and word locals preserve the pool-load and halfword-store order. */
    {
        u8 *w = *(u8 **)&gEventWork;
        s32 v = (s32)MsgKorosseoCheeringChoices;

        *(u16 *)(w + 0xcc2) = v;
        {
            u16 *q = (u16 *)(w + 0xcc4);
            s32 f = 4;

            *q = f;
        }
    }
    res = Engine_EventChooseYesNo(slot, 0);
    if (res == 0) {
        Engine_EventSetMessage(msg + 1);
        Engine_EventShowMessage(id, 0);
        work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        work->transition_frames = 15;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Call2(GameFlag_SetByte, (slot << 4) + 0x370, obj->x.fixed >> 20);
        Call2(GameFlag_SetByte, (slot << 4) + 0x378, obj->z.fixed >> 20);
        slot++;
        if (slot > 3) {
            Engine_EventRequestExit(10);
            Engine_GameFlagSet(0x11a);
        } else {
            Korosseo_SelectSoloCompetitor(slot);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            work->start_transition = 0;
        }
    } else {
        Engine_EventSetMessage(msg + 2);
        Engine_EventShowMessage(id, 0);
    }
    Engine_EventEnd();
}
