#include "TYPES.H"

s32 Engine_ActorGet();
void Engine_ActorSetSpeed();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ObjectMotionSetPositionAndCommit();
void Engine_EventWait();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

void HaidiaMura_RunWalkScene03380(s32 a0, s32 a1, s32 a2, s32 a3)
{
    u32 i;
    s32 p10;
    u8 *p11;
    s32 p11b;
    s32 p8;
    s32 p9;
    s32 p9b;
    s32 rec8;
    s32 record;
    u8 *p5;

    p9 = a3;
    p8 = a1;
    p10 = a2;
    rec8 = Engine_ActorGet();
    p5 = *(s32 *)(rec8 + 80);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, a0, 0x188, 0x35b);
    Call3(Engine_ActorFaceDirection, a0, 0xc000, 10);
    ((struct Flags85 *)rec8)->flags = 0;
    ((struct Flags38 *)p5)->flags = 0;
    Engine_ActorSetAnimation(a0, p8);
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ObjectMotionSetPositionAndCommit, a0, 0x188, 0x36b);
    Engine_EventWait(10);
    Call3(Engine_ActorSetSpeed, a0, 0x4ccc, 0x2666);
    Engine_ActorSetAnimation(a0, p10);
    Call3(Engine_ObjectMotionSetPositionAndCommit, a0, 0x188, 0x37a);
    *(s32 *)(rec8 + 40) = 0x20000;
    p5[38] = 1;
    if (p9 != 0) {
        ((struct Flags85 *)rec8)->flags = 3;
    }
    Engine_ActorSetAnimation(a0, 1);
    p9b = (s32)p5 + 38;
    p11b = a0;
}
