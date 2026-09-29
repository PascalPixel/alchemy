/* Draft of resource_395 0x02008088..0x02008158 (208 bytes with pool),
 * KorimaKi_RunMessageScene; the listing keeps the rows. Remaining
 * difference: the reference loads message 0x1520 from its literal pool as a
 * link-time value; the integer message is scheduled differently (208 bytes,
 * 78 differ from +0xe). */
#include "TYPES.H"

void Engine_EventBegin();
void Local_020012f4();
s32 Engine_GameFlagIsSet();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();
void Engine_EventWait();
void Engine_ActorSetAnimationAndWait();
void Engine_GameFlagSet();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void KorimaKi_RunMessageScene(void)
{
    Engine_EventBegin();
    Local_020012f4(11, 1);
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage(0x151d);
        Engine_EventShowMessage(9, 0);
    } else if (Value1(Engine_GameFlagIsSet, 0x84c) != 0) {
        Engine_EventSetMessage(0x1525);
        Engine_EventShowMessage(9, 0);
    } else {
        Engine_EventSetMessage(0x1520);
        Engine_EventShowMessageAndWait(9, 0, 20);
        Local_020012f4(11, 0);
        Engine_EventWait(60);
        Local_020012f4(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(40);
        Engine_EventShowMessage(9, 0);
        Local_020012f4(11, 0);
        Engine_EventWait(80);
        Engine_EventShowMessageAndWait(9, 0, 20);
        Local_020012f4(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 20);
        Call1(Engine_GameFlagSet, 0x84c);
    }
    Local_020012f4(11, 0);
    Engine_EventEnd();
}
