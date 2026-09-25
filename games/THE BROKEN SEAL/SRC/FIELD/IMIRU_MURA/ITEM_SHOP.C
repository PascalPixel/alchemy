#include "TYPES.H"

s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
s32 Engine_ShopOpen();
void Engine_EventBegin();
void Engine_ActorFaceActor();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Imil item shop: with the village cured the mother keeps shop 12; while she is
 * sick her daughter sells across the counter, and the father talks about Kolima
 * when spoken to off-angle. */
void ImiruMura_RunItemShop(void)
{
    s32 dir;

    dir = *(s16 *)(Engine_ActorGet(0) + 6);
    if (Value1(Engine_GameFlagIsSet, 0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(12, 15);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(15, 0, 0);
        Call1(Engine_EventSetMessage, 0x164f);
        Engine_EventShowMessage(15, 0);
        Call3(Engine_ActorFaceDirection, 15, 0x4000, 0);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Call1(Engine_EventSetMessage, 0x1546);
            Engine_EventShowMessage(14, 0);
            Value2(Engine_ShopOpen, 12, 14);
            Engine_EventEnd();
        } else {
            Engine_ActorFaceActor(14, 0, 10);
            Call1(Engine_EventSetMessage, 0x1547);
            Engine_EventShowMessage(14, 0);
            Call3(Engine_ActorFaceDirection, 14, 0x5000, 10);
        }
    }
}
