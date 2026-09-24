#include "TYPES.H"

void Engine_GameFlagSet();
void Engine_EventWait();
void Main_0808a5e0();
void Engine_ActorSetAnimation();
s32 Engine_ActorGet();
s32 Engine_GameFlagIsSet();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetPosition();
void Engine_ActorSetSpriteFlags();



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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

s32 GomaIriguchi_RestoreEntryState(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    Call1(Engine_GameFlagSet, 0x144);
    Engine_EventWait(10);
    Main_0808a5e0(170);
    Engine_ActorSetAnimation(11, 2);
    ((struct Flags35 *)Engine_ActorGet(11))->flags = 2;
    {
        u8 *record = Engine_ActorGet(8);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 16);
    }
    {
        u8 *record = Engine_ActorGet(15);
        u8 value = *(volatile u8 *)&record[89];
    
        record[89] = (u8)(value | 8);
    }
    if (Value1(Engine_GameFlagIsSet, 0x865) != 0) {
        Call6(Engine_MapCopyCellAttributes, 74, 11, 1, 1, 73, 11);
    }
    if (Value1(Engine_GameFlagIsSet, 0x860) != 0) {
        Call3(Engine_ActorSetPosition, 8, 0x880000, 0xc40000);
        *(u8 *)(Engine_ActorGet(8) + 35) |= 2;
        v5 = 12;
        Engine_ActorSetAnimation(8, 2);
        Call6(Engine_MapCopyCellAttributes, 39, 12, 3, 1, 8, v5);
        Call6(Engine_MapCopyCellAttributes, 43, 11, 3, 1, v5, 11);
    }
    if (Value1(Engine_GameFlagIsSet, 0x861) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x1080000, 0x1380000);
        Call6(Engine_MapCopyCellAttributes, 48, 18, 1, 2, 16, 18);
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x862) != 0) {
            Call3(Engine_ActorSetPosition, 9, 0x1180000, 0x1380000);
            Call6(Engine_MapCopyCellAttributes, 47, 18, 1, 2, 16, 18);
        }
    }
    if (Value1(Engine_GameFlagIsSet, 0x863) != 0) {
        Call3(Engine_ActorSetPosition, 10, 0x1780000, 0x1180000);
        ((struct Flags35 *)Engine_ActorGet(10))->flags = 2;
        v5 = 0;
        *(u8 *)(Engine_ActorGet(10) + 85) = v5;
        record = Engine_ActorGet(10);
        Engine_ActorSetSpriteFlags((s32)record, 0);
        Call6(Engine_MapCopyCellAttributes, 54, 17, 1, 1, 23, 17);
    }
    return 0;
}
