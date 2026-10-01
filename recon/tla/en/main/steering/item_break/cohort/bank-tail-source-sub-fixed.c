/* TLA-EN complete item-break bank source trial.
   Approved game flags emit 780 bytes versus the complete native 780;
   12 differing byte positions in the current-source-linked own-ROM
   comparison, including all calls, pools and natural alignment.
   This is an uncredited preservation draft. The bank's four other routines
   match only when the measured source and native boundaries retain their extents.
   Other editions are not verified: their current raw namespaces lack required
   routine/table labels, so no full six-edition linking or credit is claimed. */

#include "FIXED_MATH.H"
#include "EFFECT_0809B11C.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
extern u8 Data_080f0e78;
#include "RAM_BUFFER.H"

#include "IWRAM_CALL.H"

u32 BattleFx_HasReachedTarget(struct EffectSlot *);

struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, struct Output_08097f80 *);

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *gEffectWork;
void BattleEffect_InitializeSharedScene(void);
struct ItemBreakObject {
    u8 reserved00[6];
    u16 angle;
    s32 x, y, z;
    s32 reserved14;
    s32 scale_x, scale_y;
    s32 reserved20[2];
    u32 velocity;
    s32 reserved2c;
    s32 fragment_scale, fragment_limit;
    u8 reserved38[16];
    s32 decay;
    u8 reserved4c[9];
    u8 mode;
    u8 reserved56[22];
    void (*update)(void *);
};
void *BattleFx_StartItemBreak(struct ItemBreakObject *object);
void BattleFx_SnapScaleToFull(void *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void Object_SetCallback(void *, void *);
extern void BattleFx_UpdateItemBreakFragment(void *);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Audio_PlayCue(s32);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
s32 Object_CommitPosition();

extern void Object_Destroy(void *object);

extern u8 gObjectSlots[];

struct FieldActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct FieldPartyState {
    u8 unknown_000[500];
    s32 leader;
};

extern u8 *gEventWork;
extern struct FieldPartyState gGameState;
struct FieldActor *ObjectTable_Get(s32 index);

void BattleFx_UpdateOrbitAndReturn(struct EffectSlot *effect)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;
        s8 *next;
        s32 x;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Random16();
        Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
        next = state_pointer;
        x = position.x;
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
        asm("" : "+r"(next) : "r"(x));
        effect->target_x = x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*next)++;
        return;
    }

    if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            (*state_pointer)++;
            goto next_state;
        }
        return;
    }

    if (state == 2) {
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
        register s8 *flag asm("r2");
        s32 x;
        do { x = effect->origin_x; } while (0);
        flag = (s8 *)effect;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(flag) : "r"(x));
        effect->target_x = x;
        {
            s32 z;
            do { z = effect->origin_z; } while (0);
            flag += 66;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
            asm volatile("" : "+r"(flag) : "r"(z));
            effect->target_z = z;
        }
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        *flag = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && BattleFx_HasReachedTarget(effect) == 0)
        BattleFx_ClearOwnedSlot(effect);
}

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = Ram_HeapSlots->effect_work;
    object = scene->main_object;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
    do {
        BattleEffect_InitializeSharedScene();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}

void *BattleFx_StartItemBreak(struct ItemBreakObject *initial)
{
    struct ItemBreakObject *source;
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 vel;
    u32 rotation_jitter;
    s32 zero;
    struct ItemBreakObject *parent;
    struct ItemBreakObject *child;

    {
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    register u32 heading asm("r3");
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    register u32 quarter asm("r0");
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    register s32 x asm("r1");
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    register s32 y asm("r2");
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    register struct ItemBreakObject *position asm("r2");
    heading = initial->angle;
    /* FAKEMATCH: reserves the later parent register only at the source capture to preserve their distinct native lifetimes. */
    asm volatile("mov %0, %1" : "=h"(source) : "l"(initial), "r"(heading) : "r10");
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    asm volatile("mov %0, %1" : "=l"(position) : "r"(source), "r"(heading));
    quarter = 0x2000;
    /* FAKEMATCH: preserves the low pointer read-then-overwrite lifetime across the two actual position reads. */
    asm volatile("ldr %0, [%2, #8]\nldr %1, [%2, #12]" : "=&l"(x), "=l"(y) : "l"(position), "m"(position->x), "m"(position->y), "r"(heading), "r"(quarter));
    angle = (heading + quarter) & 0xc000;
    parent = Object_Spawn(0x116, x, y + 0x100000, source->z);
    }
    if (parent == 0)
        return 0;
    parent->scale_y = 0x4000;
    parent->scale_x = 0x4000;
    parent->update = BattleFx_UpdateItemBreakFragment;
    parent->fragment_scale = 0x20000;
    parent->fragment_limit = 0x20000;
    zero = 0;
    parent->mode = zero;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x2a1, source->x,
                              source->y + 0x100000,
                              source->z);
        if (child != 0) {
            Object_SetCallback(child, &Data_080f0e78);
            fragment_scale = Random16() + 0x10000;
            child->fragment_limit = 0x10000;
            child->fragment_scale = fragment_scale;
            child->mode = 2;
            child->decay = 0x51e;
            vel = Random16();
            child->velocity = vel - Random16();
            fragment_height = Random16() * 0x18 + 0x80000;
            rotation_jitter = Random16();
            {
                u32 last = Random16();
    /* FAKEMATCH: keeps the known parent alias low beside the callback without creating an unknown pointer value. */
                register u32 heading asm("r3") = source->angle;
    /* FAKEMATCH: retains the live heading before subtracting the final random angle; the plain subtraction moves before the read. */
                asm volatile("sub %0, %0, %2" : "+l"(rotation_jitter) : "l"(heading), "l"(last));
                Motion_SetTargetPositionFromMagnitudeAngle(child, fragment_height, (rotation_jitter >> 3) + heading);
            }
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(138);
    return parent;
}

void BattleFx_SnapScaleToFull(void *obj)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    s32 next;
    s32 scale;
    s32 bound;

    if (obj != NULL) {
        {
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
            register s32 high asm("r3") = 255;
            scale = FIELD_AT_OFFSET(obj, s32, 0x18);
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
            asm volatile("" : "+r"(high) : "r"(scale));
            bound = (high << 8) + 255;
        }
        if (scale <= bound) {
            s32 step = 0x1000;
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
            register s32 limit asm("ip");
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
            asm volatile("" : "+r"(step) : "r"(bound));
            limit = bound;
            do {
                next = scale + step;
                scale = next;
            } while (next <= limit);
            FIELD_AT_OFFSET(obj, s32, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}

void UpdateRisingParticleBurst(void *source)
{
    /* FAKEMATCH: retained ordinary-source and scheduling trial; it remains an uncredited draft. */
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
    register s32 unit asm("sl");

    s32 step = -0x800;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
    asm volatile("" : "+r"(step));
    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
    /* FAKEMATCH: retained item-break scheduling trial; see the measured limitation above. */
        register s32 angle asm("r3");
        s32 turn;
        s32 wait, scale;
        *(s32 *)((s8 *)source + 12) += 0x10000;
        do { angle = *(u16 *)((s8 *)source + 6); } while (0);
        turn = 128;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(turn), "+r"(angle));
        *(u16 *)((s8 *)source + 6) = angle + (turn << 6);
        scale = *(s32 *)((s8 *)source + 24);
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(scale));
        wait = 1;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
        asm volatile("" : "+r"(wait));
        *(s32 *)((s8 *)source + 24) = scale + step;
        *(s32 *)((s8 *)source + 28) += step;
        WaitFrames(wait);
    }
    count = 7;
    unit = 0x10000;
    for (; count >= 0; count--) {
        child = Object_Spawn(0x2a1, *(s32 *)((s8 *)source + 8),
                             *(s32 *)((s8 *)source + 12),
                             *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Object_SetCallback(child, &Data_080f0e78);
            {
                s32 speed = Random16();

                *(s32 *)((s8 *)child + 0x34) = unit;
                *(s32 *)((s8 *)child + 0x30) = speed + unit;
            }
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0xa3d;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            dist = Random16() * 0x18;
            dist += 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(source);
}
