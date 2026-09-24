#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_ShopOpen();
void Engine_EventBegin();
void Engine_ActorFaceActor();
void Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Main_0808a0b0();



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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void ImiruMura_Func020007a4(void)
{
    s32 dir;

    dir = *(s16 *)(Engine_ActorGet(0) + 6);
    if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(10, 12);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(12, 0, 0);
        Engine_EventWait(10);
        Call1(Engine_EventSetMessage, 0x164b);
        Engine_EventShowMessage(12, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Call2(Engine_CameraSetSpeed, 0x60000, 0xc000);
            Call4(Engine_CameraMoveTo, 0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Call2(Main_0808a0b0, 12, 0x200a5ec);
            Call1(Engine_EventSetMessage, 0x153e);
            Engine_EventShowMessage(12, 0);
            Call4(Engine_CameraMoveTo, 0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}
