/* Draft of resource_380 0x0200a27c Func_0200227c (Sol Sanctum star actors),
 * to link as FIELD/SORU_STAR/ORBIT_ACTORS.C. Remaining difference: register
 * allocation only; the reference keeps the leader in r10, the angle script in
 * r8, zero in r9 and the flag bit 1 in r6, and ends with orrs r6, r3. The
 * listing keeps these rows. */
#include "TYPES.H"

extern u8 SoruStar_AngleScript[];

void Engine_ObjectSetScript();
void Engine_EventBegin();
void Engine_EventEnd();
u8 *Object_GetById();
void Engine_ActorEnableActionCallback();
void Engine_ActorSetPosition();

/* FAKEMATCH: call sites spelled through these wrappers pass their constants
 * straight into the argument registers; a value-returning call also sets r0
 * last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Sol Sanctum's star: place the five light actors and the pedestal round the
 * party leader, bind each to the leader and start its angle script. */
void Func_0200227c(void)
{
    s32 leader;
    u8 *actor;
    u8 *script;
    s32 zero;
    s32 one;

    leader = Value1((s32 (*)())Object_GetById, 0);
    Engine_EventBegin();
    Engine_ActorEnableActionCallback(5, 1);
    Engine_ActorEnableActionCallback(9, 1);
    Engine_ActorEnableActionCallback(11, 1);
    Engine_ActorEnableActionCallback(10, 1);
    Engine_ActorEnableActionCallback(14, 1);
    Engine_ActorEnableActionCallback(13, 1);
    Call3(Engine_ActorSetPosition, 5, 0x1db0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 9, 0x1fa0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 11, 0x1cb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 10, 0x1fb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1680000);
    Call3(Engine_ActorSetPosition, 13, 0x1d70000, 0x1320000);
    actor = Object_GetById(5);
    *(s32 *)(actor + 104) = leader;
    one = 1;
    actor[90] |= one;
    script = SoruStar_AngleScript;
    zero = 0;
    Engine_ObjectSetScript(actor, script);
    actor = Object_GetById(9);
    *(s32 *)(actor + 104) = leader;
    actor[90] |= one;
    Engine_ObjectSetScript(actor, script);
    actor = Object_GetById(11);
    *(s32 *)(actor + 104) = leader;
    actor[90] |= one;
    Engine_ObjectSetScript(actor, script);
    actor = Object_GetById(10);
    *(s32 *)(actor + 104) = leader;
    actor[90] |= one;
    Engine_ObjectSetScript(actor, script);
    actor = Object_GetById(14);
    *(s32 *)(actor + 104) = leader;
    actor[90] |= one;
    *(s32 *)(actor + 24) = 0x10000;
    *(s32 *)(actor + 28) = 0x10000;
    actor[85] = Object_GetById(11)[85];
    *(s32 *)(actor + 12) = zero;
    Engine_ObjectSetScript(actor, script);
    actor = Object_GetById(13);
    *(s32 *)(actor + 104) = leader;
    actor[90] |= one;
    Engine_ObjectSetScript(actor, script);
    Engine_EventEnd();
}
