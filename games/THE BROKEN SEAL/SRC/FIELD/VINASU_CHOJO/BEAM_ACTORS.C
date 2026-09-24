#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetChildValue();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Venus Lighthouse top: once actor 19 has stopped, reset both beam actors
 * to face north, grow them while flag 0x235 is set (else pick the visible
 * one by display bank) and slide them while flag 0x234 is set. */
void VinasuChojo_UpdateBeamActors(void)
{
    s32 rec7;
    s32 rec8;
    s32 v1;

    rec7 = Value1(Engine_ActorGet, 20);
    rec8 = Engine_ActorGet(19);
    /* FAKEMATCH: the dead first test and the word temporaries below reproduce the
     * reference's leftover loads and stores. */
    v1 = *(s32 *)(rec7 + 56) == -0x80000000 && *(s32 *)(rec7 + 60) == *(s32 *)(rec7 + 56) && *(s32 *)(rec7 + 64) == *(s32 *)(rec7 + 60);
    if (*(s32 *)(rec8 + 56) == -0x80000000 && *(s32 *)(rec8 + 60) == *(s32 *)(rec8 + 56) && *(s32 *)(rec8 + 64) == *(s32 *)(rec8 + 60)) {
        v1 = 1;
    } else {
        v1 = 0;
    }
    if (v1 != 0) {
        {
            s32 shown = 0;
        
            *(u16 *)(rec7 + 6) = shown;
        }
        {
            s32 shown = 0;
        
            *(u16 *)(rec8 + 6) = shown;
        }
        if (Value1(Engine_GameFlagIsSet, 0x235) != 0) {
            Engine_ActorSetChildValue(20, 7);
            Engine_ActorSetChildValue(19, 7);
            if (*(s32 *)(rec7 + 24) >= 0x14000) {
                goto L_02003874;
            }
            *(s32 *)(rec7 + 24) += 0x200;
            *(s32 *)(rec7 + 28) += 0x200;
            *(s32 *)(rec8 + 24) += 0x200;
            *(s32 *)(rec8 + 28) += 0x200;
        } else {
            if ((*(s32 *)0x03001e40 & 2) != 0) {
                Engine_ActorSetChildValue(20, 15);
                Engine_ActorSetChildValue(19, 0);
            } else {
                Engine_ActorSetChildValue(20, 0);
                Engine_ActorSetChildValue(19, 15);
            }
        }
        L_02003874:;
        if (Value1(Engine_GameFlagIsSet, 0x234) != 0) {
            if (*(s32 *)(rec7 + 8) < 0x1380000) {
                *(s32 *)(rec7 + 8) += 0x1000;
                *(s32 *)(rec8 + 8) += 0x1000;
            }
            if (*(s32 *)(rec7 + 16) > 0xb60000) {
                *(s32 *)(rec7 + 16) += -0x1000;
                *(s32 *)(rec8 + 16) += -0x1000;
            }
        }
    }
}
