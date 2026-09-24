#include "TYPES.H"

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
u8 * Engine_ActorGet();
void Engine_WorkSetValuesIfNonNegative();
void Engine_AudioPlayCue();
void Engine_TaskWait();
void Engine_ActorSetPosition();
void Engine_ActorWalkToAndWait();
void Engine_EventWait();
void Engine_MapWaitWorkValuesBelow256();
void Main_0808a5e8();
void Engine_GameFlagSet();
void FieldScene_RunScene372_02000ec4();
void SceneActor_RunActor22PlacementSequence();
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

void HaidiaArashi_RunScene00D5C(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 v3;
    s32 base5_396;
    u8 *p6;

    if (Value1(Engine_GameFlagIsSet, 0x310) != 0) {
    } else {
        Engine_EventBegin();
        if (Value1(Engine_GameFlagIsSet, 0x830) == 0) {
            rec7 = Engine_ActorGet(11);
            p6 = *(s32 *)((s32)rec7 + 80);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
            Engine_AudioPlayCue(141);
            Engine_TaskWait(40);
            Engine_AudioPlayCue(145);
            ((struct Flags35 *)rec7)->flags &= 254;
            ((struct Flags9 *)p6)->mode = 1;
            Call3(Engine_ActorSetPosition, 11, 0x1d90000, 0x3a40000);
            *(s32 *)((s32)rec7 + 48) = 0x18000;
            *(s32 *)((s32)rec7 + 52) = 0x18000;
            v3 = (*(s32 *)((s32)rec7 + 12) + 0xf00000);
            *(s32 *)((s32)rec7 + 12) += 0xf00000;
            *(s32 *)((s32)rec7 + 60) = v3;
            *(s32 *)((s32)rec7 + 68) = 0x6666;
            Call3(Engine_ActorWalkToAndWait, 11, 0x158, 0x3a4);
            ((struct Flags9 *)p6)->mode = 3;
            ((struct Flags35 *)rec7)->flags |= 1;
            Engine_EventWait(40);
            Call1(Engine_AudioPlayCue, 0x121);
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
            Engine_MapWaitWorkValuesBelow256();
            Main_0808a5e8();
            Call1(Engine_GameFlagSet, 0x830);
        }
        FieldScene_RunScene372_02000ec4();
        Call1(Engine_GameFlagSet, 0x310);
        if (Value1(Engine_GameFlagIsSet, 0x837) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x841) == 0) {
                if (Value1(Engine_GameFlagIsSet, 0x30c) == 0) {
                    record = Engine_ActorGet(0);
                    if (*(s32 *)(record + 12) > 0x800000) {
                        base5_396 = 0x396;
                        Call2(SceneActor_RunActor22PlacementSequence, 0x146, base5_396);
                        Call3(Engine_ActorWalkToAndWait, 0, 0x123, base5_396);
                    } else {
                        Call2(SceneActor_RunActor22PlacementSequence, 0x14f, 0x3bd);
                    }
                    Call1(Engine_GameFlagSet, 0x30c);
                }
            }
        }
        Engine_EventEnd();
    }
}
