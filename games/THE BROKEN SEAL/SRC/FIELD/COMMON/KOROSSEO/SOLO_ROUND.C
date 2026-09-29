#include "TYPES.H"
extern u8 MsgKorosseoCheering[];
extern u8 MsgKorosseoCheeringChoices[];
extern struct EventWork *gEventWork;

u8 *Engine_ActorGet();
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_EventShowMessage();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void GameFlag_SetByteFar();
void Engine_EventRequestExit();
void Engine_GameFlagSet();
void Korosseo_SelectSoloCompetitor();
void Engine_EventOpenScreen();
void Engine_EventEnd();

/* The game state, read here as words: word 125 holds the solo competitor. */
extern s32 gGameState[];

/* FAKEMATCH: call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a value-returning call also sets r0 last of
 * its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) { return f(a0, a1); }

/* Colosso: actor id asks whether the party is done cheering, its two answers
 * starting at MsgKorosseoCheeringChoices. On yes the solo competitor's
 * position is saved and the next one is chosen, or the party leaves after
 * the fourth; on no they are sent back to cheer. The same function sits in
 * each of the three Colosso trial overlays. */
void Korosseo_FinishSoloRound(s32 id)
{
    s32 work;
    s32 slot;
    u8 *obj;
    s32 msg;
    s32 res;

    work = *(s32 *)&gEventWork;
    Engine_ActorGet(id);
    Engine_ActorGet(id);
    slot = gGameState[125];
    obj = Engine_ActorGet(slot);
    Engine_EventBegin();
    msg = (s32)MsgKorosseoCheering;
    Engine_EventSetMessage(msg);
    Value2((s32 (*)())Engine_EventOpenMessage, id, 0);
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
        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 15;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Call2(GameFlag_SetByteFar, (slot << 4) + 0x370, *(s32 *)(obj + 8) >> 20);
        Call2(GameFlag_SetByteFar, (slot << 4) + 0x378, *(s32 *)(obj + 16) >> 20);
        slot++;
        if (slot > 3) {
            Engine_EventRequestExit(10);
            Engine_GameFlagSet(0x11a);
        } else {
            Korosseo_SelectSoloCompetitor(slot);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            *(s32 *)(work + 0x1c0) = res;
        }
    } else {
        Engine_EventSetMessage(msg + 2);
        Engine_EventShowMessage(id, 0);
    }
    Engine_EventEnd();
}
