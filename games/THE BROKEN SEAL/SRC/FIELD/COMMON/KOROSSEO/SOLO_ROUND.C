#include "TYPES.H"
#include "CALL.H"
#include "FIELDOBJ.H"
#include "GAME_FLAGS.H"
extern u8 MsgKorosseoCheering[];
extern u8 MsgKorosseoCheeringChoices[];
extern struct EventWork *gEventWork;

u8 *Object_GetById();
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_EventShowMessage();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_EventRequestExit();
void Engine_GameFlagSet();
void Korosseo_SelectSoloCompetitor();
void Engine_EventOpenScreen();
void Engine_EventEnd();

/* The game state, read here as words: word 125 holds the solo competitor. */
extern s32 gGameState[];

/* Colosso: actor id asks whether the party is done cheering, its two answers
 * starting at MsgKorosseoCheeringChoices. On yes the solo competitor's
 * position is saved and the next one is chosen, or the party leaves after
 * the fourth; on no they are sent back to cheer. The same function sits in
 * each of the three Colosso trial overlays. */
void Korosseo_FinishSoloRound(s32 id)
{
    struct EventWork *work;
    s32 slot;
    struct FieldActor *obj;
    s32 msg;
    s32 res;

    work = gEventWork;
    /* FAKEMATCH: the speaker lookup is retained twice with both results unused. */
    Object_GetById(id);
    Object_GetById(id);
    slot = gGameState[125];
    obj = Object_GetById(slot);
    Engine_EventBegin();
    msg = (s32)MsgKorosseoCheering;
    Engine_EventSetMessage(msg);
    ((s32 (*)())Engine_EventOpenMessage)(id, 0);
    /* FAKEMATCH: one work pointer and word locals preserve the pool-load and halfword-store order. */
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
