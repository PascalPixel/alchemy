#include "TYPES.H"

extern u8 Data_00000e85[];
extern u8 Data_02009ce0[];

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
s32 Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventEnd();
void Engine_ActorStop();
void Engine_EventShowMessageAndWait();
void Engine_ActorShowEmote();
void Engine_CameraMoveTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_EventWait();
s32 Engine_EventOpenMessage();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_GameFlagSet();
void Engine_ActorFaceActor();
void Engine_ActorSetAnimation();
void Engine_ActorJump();
void Engine_ActorSetAnimationAndWait();
void Main_0808a168();
void Main_0808a0b0();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();



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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void HaidiaSukureta_RunActorSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_e85;
    s32 base5_2009ce0;

    if (Value1(Engine_GameFlagIsSet, 0x839) != 0) {
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x82f) != 0) {
            Engine_EventBegin();
            ((void (*)())Engine_ActorRunRepeatedMotion)(11, 2);
            Call1(Engine_EventSetMessage, 0xe8b);
            Engine_EventShowMessage(11, 0);
            Engine_EventEnd();
        } else {
            Engine_EventBegin();
            Engine_ActorStop(11);
            Engine_ActorRunRepeatedMotion(11, 1);
            /* FAKEMATCH: message 0xe85 comes from a link symbol so GCC keeps the
             * message base in one register instead of folding each offset. */
            base5_e85 = (s32)Data_00000e85;
            Engine_EventSetMessage(base5_e85);
            Engine_EventShowMessageAndWait(11, 0, 20);
            Call3(Engine_ActorShowEmote, 0, 0x100, 30);
            Call4(Engine_CameraMoveTo, 0x620000, -1, 0x11b0000, 1);
            Call3(Engine_ActorWalkToAndWait, 0, 94, 0x125);
            Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
            record = Value1(Engine_ActorGet, 0);
            if (record != 0) {
                Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 1, 110, 0x117);
            Call3(Engine_ActorFaceDirection, 1, 0xa000, 40);
            Engine_ActorRunRepeatedMotion(11, 2);
            Engine_EventWait(40);
            Value2(Engine_EventOpenMessage, 11, 0);
            if (Value2(Engine_UiWorkWaitThenFinalizeCapacity, 0, 0) == 0) {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base5_e85 + 2));
                Engine_EventShowMessage(11, 0);
                Call1(Engine_GameFlagSet, 0x82f);
            } else {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base5_e85 + 3));
                Engine_EventShowMessageAndWait(11, 0, 40);
                Engine_ActorFaceActor(11, 0, 0);
                Engine_ActorSetAnimation(11, 1);
                Engine_ActorJump(11, 4, 40);
                Engine_ActorSetAnimation(11, 6);
                Call3(Engine_ActorShowEmote, 11, 0x101, 40);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimation(11, 1);
                Engine_EventWait(10);
                Engine_ActorSetAnimationAndWait(11, 3);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimationAndWait(11, 3);
                base5_2009ce0 = (s32)Data_02009ce0;
                Call3(Main_0808a168, 0, 0x1000b, base5_2009ce0);
                Call3(Main_0808a168, 1, 0x1000b, base5_2009ce0);
                Call2(Main_0808a0b0, 11, 0x2009bdc);
                Engine_ActorStop(0);
                Engine_ActorStop(1);
                Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
                Call3(Engine_ActorFaceDirection, 1, 0x4000, 60);
                Call3(Engine_ActorShowEmote, 0, 0x105, 0);
                Call3(Engine_ActorShowEmote, 1, 0x105, 120);
                Call1(Engine_GameFlagSet, 0x839);
            }
            Engine_ActorSetAnimation(1, 2);
            record = Value1(Engine_ActorGet, 0);
            if (record != 0) {
                Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Engine_ActorWaitForMove(1);
            Engine_ActorSetPosition(1, 0, 0);
            Engine_EventEnd();
        }
    }
}
