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

/* When flag 0x203 is set, actor 8 walks off and MsgHaidiaDontWorryBest plays;
 * otherwise it turns, shows MsgHaidiaUnnOhhKyle and then 0x1c7a. */
void HaidiaBabi_RunActorEightMessageScene(void)
{
    s32 record;
    s32 dream;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x203) != 0) {
        Object_SetTargetAndCallback(8, 0x10000, (s32)gHaidiaBabiActor8Departure);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaDontWorryBest);
        Engine_EventShowMessage(8, 0);
    } else {
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        do {
            dream = (s32)MsgHaidiaUnnOhhKyle;
        } while (0); /* FAKEMATCH: the wrap keeps the message id load after the wait. */
        Engine_EventSetMessage(dream);
        Engine_EventShowMessageAndWait(8, 0, 40);
        Engine_MessageShowCentered((dream + 1), 1);
    }
    Engine_EventEnd();
}