/* NONMATCHING (number-bound): Korosseo_FinishSoloRound, resource_3ba at
 * 0x02009b5c (288 bytes with its pool); twins resource_3bb:0x02009df4 and
 * resource_3bc:0x0200a88c. Formerly FIELD/COMMON/KOROSSEO/SOLO_ROUND.C,
 * which no script linked.
 *
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names symbols no link defines
 * (Engine_UiWorkWaitThenFinalizeCapacity, Main_080770e8). */
#include "TYPES.H"
extern u8 MsgKorosseoCheering[];
extern struct EventWork *gEventWork;

u8 * Engine_ActorGet();
void Engine_EventBegin();
void Engine_EventSetMessage();
void Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_EventShowMessage();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Main_080770e8();
void Engine_EventRequestExit();
void Engine_GameFlagSet();
void Korosseo_SelectSoloCompetitor();
void Engine_EventOpenScreen();
void Engine_EventEnd();


extern s32 gGameState[];


/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a value-returning call also sets r0 last of
 * its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1) { f(a0, a1); }
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1) { return f(a0, a1); }

/* Colosso solo round result: report the round, then either leave after the
 * fourth competitor or select the next one. */
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
        s32 v = 0x2089;

        *(u16 *)(w + 0xcc2) = v;
        {
            u16 *q = (u16 *)(w + 0xcc4);
            s32 f = 4;

            *q = f;
        }
    }
    res = Engine_UiWorkWaitThenFinalizeCapacity(slot, 0);
    if (res == 0) {
        Engine_EventSetMessage(msg + 1);
        Engine_EventShowMessage(id, 0);
        *(s32 *)(work + 0x1c0) = 0x200;
        *(s32 *)(work + 0x1c8) = 15;
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Call2(Main_080770e8, (slot << 4) + 0x370, *(s32 *)(obj + 8) >> 20);
        Call2(Main_080770e8, (slot << 4) + 0x378, *(s32 *)(obj + 16) >> 20);
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
