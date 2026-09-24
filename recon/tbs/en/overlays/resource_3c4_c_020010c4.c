/* NONMATCHING: 284 of 284 bytes, 11 halfword edits (2026-09-24). Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* at
 * their import veneers plus advance_effect_motion = 0x02009068 (thumb, the
 * effect update in MAP_INIT.C). Matched: the spray velocities (vz multiplies
 * by -0x1999 so it is synthesised by shifts; m - 5 is its own temporary so
 * the 0x3332 product is not distributed). Remaining: the motion-flag OR loads
 * the byte into r3 and the 2 into r2 (the reference swaps them), and the
 * block after the six-frame wait: the reference takes &options into r8 first
 * and stores the zero flag byte from r5 after loading the update address;
 * this draft stores the zero from r3 first. */
#include "TYPES.H"
#include "FIELD_EFFECT.H"

s32 Engine_MathModulo(s32 value, s32 divisor);
void Main_0808a118(s32 mode);
void advance_effect_motion(union FieldObject *object);

void Local_020010c4(void)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    u8 *flags;
    struct EffectOptions options;
    struct EffectOptions *o;
    u32 i;

    Engine_EventBegin();
    Engine_ObjectSetAnimation(leader, 6);
    Main_0808a118(0);
    Engine_ObjectSetAnimation(leader, 1);
    Engine_ActorSetSpriteFlags(leader, 0);
    flags = &leader->motion_flags;
    *flags |= 2;
    Engine_AudioPlayCue(152);
    leader->velocity_y = 0x40000;
    Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + 0xc0000);
    Engine_TaskWait(6);
    o = &options;
    *flags = 0;
    o->update = advance_effect_motion;
    Engine_AudioPlayCue(127);
    for (i = 0; i < 8; i++) {
        leader->y.fixed -= 0x20000;
        leader->target_y = leader->y.fixed;
        Engine_TaskWait(1);
        if (i & 1) {
            s32 m = Engine_MathModulo(Engine_RandomNext(), 10) - 5;
            s32 vx = m * 0x3332;
            s32 vz;

            vz = Engine_MathModulo(Engine_RandomNext(), 10) * -0x1999 - 0x7ffd;

            Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, vx, 0, vz, 0x1000001, o);
        }
    }
    Engine_ActorSetSpriteFlags(leader, 1);
    *flags = 3;
    Engine_EventEnd();
}
