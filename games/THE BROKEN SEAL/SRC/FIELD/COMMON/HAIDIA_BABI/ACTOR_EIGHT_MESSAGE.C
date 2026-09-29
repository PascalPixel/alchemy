#include "TYPES.H"
extern u8 MsgHaidiaDontWorryBest[];
extern u8 MsgHaidiaUnnOhhKyle[];

/* Actor 8's two lines, depending on whether he has already left. */

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_MessageShowCentered();
void Engine_EventEnd();
void Engine_ActorRunRepeatedMotion();
void Object_SetTargetAndCallback();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();

/* Actor 8's departure, in the overlay's read-only data. */
extern s32 gHaidiaBabiActor8Departure[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* When flag 0x203 is set, actor 8 walks off and MsgHaidiaDontWorryBest plays;
 * otherwise it turns, shows MsgHaidiaUnnOhhKyle and then 0x1c7a. */
void HaidiaBabi_RunActorEightMessageScene(void)
{
    s32 record;
    s32 base5_1c79;

    Engine_EventBegin();
    if (Value1(Engine_GameFlagIsSet, 0x203) != 0) {
        Call3(Object_SetTargetAndCallback, 8, 0x10000, (s32)gHaidiaBabiActor8Departure);
        Engine_EventWait(20);
        Call1(Engine_EventSetMessage, (s32)MsgHaidiaDontWorryBest);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        do {
            base5_1c79 = (s32)MsgHaidiaUnnOhhKyle;
        } while (0); /* FAKEMATCH: the wrap keeps the message id load after the wait. */
        Engine_EventSetMessage(base5_1c79);
        Engine_EventShowMessageAndWait(8, 0, 40);
        Engine_MessageShowCentered((base5_1c79 + 1), 1);
    }
    Engine_EventEnd();
}