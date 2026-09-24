#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3a5:02001180):
 *   Engine_ActorGet = 0x02009d74 (thumb)
 *   Engine_GameFlagSet = 0x02009d4c (thumb)
 *   Engine_ActorSetAnimation = 0x02009dac (thumb)
 *   Engine_ActorFaceActor = 0x02009dcc (thumb)
 *   Engine_EventWait = 0x02009d5c (thumb)
 *   Engine_ActorSetAttachedEffect = 0x02009df4 (thumb)
 *   Engine_ActorStartRepeatedMotion = 0x02009dbc (thumb)
 *   Main_0808a108 = 0x02009db4 (thumb)
 *   Engine_ActorSetSpeed = 0x02009d7c (thumb)
 *   Engine_ActorSetDestination = 0x02009d84 (thumb)
 *   Engine_ActorShowEmote = 0x02009dec (thumb)
 *   Engine_Import0808a250 = 0x02009e04 (thumb)
 */
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

s32 Engine_ActorGet();
void Engine_GameFlagSet();
void Engine_ActorSetAnimation();
void Engine_ActorFaceActor();
void Engine_EventWait();
void Engine_ActorSetAttachedEffect();
void Engine_ActorStartRepeatedMotion();
void Main_0808a108();
void Engine_ActorSetSpeed();
void Engine_ActorSetDestination();
void Engine_ActorShowEmote();
void Engine_Import0808a250();



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

/* NONMATCHING: 382 of 380 bytes, 36 halfword edits (2026-09-24). The
 * reference adds 0x1f4 to the Data_02000240 base at run time; every spelling
 * tried folds it into one literal, and the nearest-actor loop spills min. */
void Func_02001180(void)
{
    u8 *target;
    u8 *actor;
    u8 *record;
    s32 id;
    s32 best;
    s32 min;
    s32 dx;
    s32 dz;

    target = Value1(Engine_ActorGet, *(s32 *)&Data_02000240_t[250][0]);
    best = 9;
    Call1(Engine_GameFlagSet, 0x200);
    min = 0x100000;
    for (id = 9; id <= 12; id++) {
        actor = Value1(Engine_ActorGet, id);
        if (actor != 0) {
            dx = (*(s32 *)(target + 8) - *(s32 *)(actor + 8)) / 0x10000;
            dz = (*(s32 *)(target + 16) - *(s32 *)(actor + 16)) / 0x10000;
            if (dx < 0) {
                dx = -dx;
            }
            if (dz < 0) {
                dz = -dz;
            }
            if (dx + dz < min) {
                best = id;
                min = dx + dz;
            }
        }
    }
    Engine_ActorSetAnimation(0, 1);
    *(u8 *)(Engine_ActorGet(0) + 90) &= 254;
    Engine_ActorFaceActor(0, best, 0);
    Engine_EventWait(20);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_ActorStartRepeatedMotion(0, 2);
    Engine_EventWait(60);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x101);
    actor = Value1(Engine_ActorGet, 0);
    record = Engine_ActorGet(0);
    *(u16 *)(actor + 6) = (*(u16 *)(record + 6) + 0x8000) & 0xfffff000;
    Engine_ActorSetAnimation(0, 5);
    Main_0808a108(0, 24);
    Call3(Engine_ActorSetSpeed, 0, 0x1999, 0xccc);
    record = Engine_ActorGet(0);
    *(s32 *)(record + 108) = 0x2008345;
    record = Value1(Engine_ActorGet, best);
    if (record != 0) {
        Engine_ActorSetDestination(0, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, best, 0x104, 0);
    Engine_EventWait(60);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x100);
    *(u8 *)(Engine_ActorGet(0) + 90) |= 1;
    record = Engine_ActorGet(0);
    *(s32 *)(record + 108) = 0;
    Engine_Import0808a250(53, 4);
}
