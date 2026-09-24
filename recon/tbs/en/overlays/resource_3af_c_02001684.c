#include "TYPES.H"
extern u8 Data_0200c4d8[];

void Engine_ActorSetSpritePriority();
s32 Engine_GameFlagIsSet();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();
s32 Engine_RandomNext();
s32 Local_030003e0();
void Engine_ActorEnableActionCallback();



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

/* NONMATCHING: 284 of 284 bytes, 13 halfword edits (2026-09-24). The
 * actor pointer and the action-callback base swap r5 and r6 against the
 * reference; block-scoped and typed spellings of the base moved nothing. */
void Func_02001684(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 base6_200c4d8;

    Engine_ActorSetSpritePriority(27, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(26, 1);
    Engine_ActorSetSpritePriority(24, 1);
    if (Value1(Engine_GameFlagIsSet, 0x920) != 0) {
        Call3(Engine_ActorSetPosition, 22, 0xa20000, 0x29a0000);
        record = Engine_ActorGet(22);
        {
            s32 shown = 0x8000;
        
            *(u16 *)(record + 6) = shown;
        }
        Engine_ActorSetPosition(23, 0, 0);
        Engine_ActorSetPosition(20, 0, 0);
    }
    rec7 = Value1(Engine_GameFlagIsSet, 0x922);
    if (rec7 != 0) {
        Call3(Engine_ActorSetPosition, 21, 0x1080000, 0x2be0000);
        record = Engine_ActorGet(21);
        {
            s32 shown = 0x5000;
        
            *(u16 *)(record + 6) = shown;
        }
        rec7 = Value1(Engine_ActorGet, 21);
        record = Engine_RandomNext();
        rec7 += 100;
        *(u16 *)rec7 = (Local_030003e0(record, 90) + 60);
        base6_200c4d8 = (s32)Data_0200c4d8;
        Engine_ActorEnableActionCallback(21, base6_200c4d8);
        Call3(Engine_ActorSetPosition, 24, 0xf80000, 0x2a80000);
        rec7 = Value1(Engine_ActorGet, 24);
        record = Engine_RandomNext();
        rec7 += 100;
        *(u16 *)rec7 = (Local_030003e0(record, 90) + 60);
        Engine_ActorEnableActionCallback(24, base6_200c4d8);
        Engine_ActorSetPosition(22, 0, 0);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x923) != 0) {
            Call3(Engine_ActorSetPosition, 20, 0xf60000, 0x2000000);
            record = Value1(Engine_ActorGet, 20);
            *(u16 *)(record + 6) = rec7;
        }
    }
}
