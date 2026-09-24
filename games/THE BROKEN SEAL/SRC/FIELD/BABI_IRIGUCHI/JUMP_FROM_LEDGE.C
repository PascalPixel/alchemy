#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_EventBegin();
void Engine_ActorShowEmote();
void Engine_ActorSetSpeed();
void Engine_ActorJump();
void Engine_ActorSetDestinationOffset();
void Engine_ActorWaitForMove();
void Engine_ActorFaceDirection();
void SceneState_ApplyRectsAtActors8And9();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void BabiIriguchi_JumpFromLedge(void)
{
    s32 rec7;

    rec7 = Value1(Engine_ActorGet, 0);
    Engine_EventBegin();
    if ((*(s32 *)(rec7 + 8) >> 20) != 6) {
        if ((*(s32 *)(rec7 + 8) >> 20) != 18) {
            goto done;
        }
    }
    if ((*(s32 *)(rec7 + 16) >> 20) == 20) {
        *(s32 *)(rec7 + 56) = -0x80000000;
        *(s32 *)(rec7 + 64) = -0x80000000;
        Call3(Engine_ActorShowEmote, 0, 0x100, 20);
        Call3(Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
        Call3(Engine_ActorJump, 0, 4, 0);
{ u16 dir = *(u16 *)(rec7 + 6); if ((u16)(dir + 0x4fff) > 0x1fff && (u16)(dir - 0x3001) > 0x1fff) goto step_down; }
        Engine_ActorSetDestinationOffset(0, 16, 0);
        Engine_ActorWaitForMove(0);
        Call3(Engine_ActorFaceDirection, 0, 0x8000, 20);
        goto done;
        step_down:;
        Call3(Engine_ActorSetDestinationOffset, 0, 0, -16);
        Engine_ActorWaitForMove(0);
        Call3(Engine_ActorFaceDirection, 0, 0x4000, 20);
    }
    done:;
    SceneState_ApplyRectsAtActors8And9();
    Engine_EventEnd();
}
