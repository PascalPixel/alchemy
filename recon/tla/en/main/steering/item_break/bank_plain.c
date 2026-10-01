/* TLA-EN item-break bank: 776 emitted bytes versus the complete
   780-byte current native bank; 544 differing byte positions after excluding
   external relocation words in the native templates. Calls and address pools
   are not linked in this comparison. The bank remains uncredited. */

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
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(void *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(
    struct Object_08096bec *object, s32 magnitude, s32 angle);
extern void Object_SetMode(void *, s32);
extern void Object_SetCallback(void *, void *);
extern void Func_080dcf8c(void *);

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
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;

        position.x = effect->origin_x;
        position.z = effect->origin_z;
        angle = Random16();
        Vector_AddPolarOffset(0x1e0000, (u16)angle, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x40000;
        effect->max_speed = 0x40000;
        effect->flag42 = state;
        (*state_pointer)++;
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
        effect->target_x = effect->origin_x;
        effect->target_z = effect->origin_z;
        {
            u32 value = 0x400;

            effect->max_turn_step = value;
        }
        effect->flag42 = 1;
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

void *BattleFx_StartItemBreak(void *source)
{
    s32 angle;
    s32 fragment_height;
    s32 fragment_scale;
    s32 fragment_count;
    u32 vel;
    u32 rotation_jitter;
    s32 zero;
    void *parent;
    void *child;

    angle = (*(u16 *)((s8 *)source + 6) + 0x2000) & 0xc000;
    parent = Object_Spawn(0x116, *(s32 *)((s8 *)source + 8),
                           *(s32 *)((s8 *)source + 12) + 0x100000,
                           *(s32 *)((s8 *)source + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = (s32)Func_080dcf8c;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Object_SetMode(parent, 3);
    Motion_SetTargetPositionFromMagnitudeAngle(parent, 0x100000, angle);

    fragment_count = 7;
    do {
        child = Object_Spawn(0x2a1, *(s32 *)((s8 *)source + 8),
                              *(s32 *)((s8 *)source + 12) + 0x100000,
                              *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Object_SetCallback(child, &Data_080f0e78);
            fragment_scale = Random16() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = fragment_scale;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            fragment_height = Random16() * 0x18 + 0x80000;
            rotation_jitter = Random16();
            Motion_SetTargetPositionFromMagnitudeAngle(
                child, fragment_height,
                          ((rotation_jitter - Random16()) >> 3) +
                          *(u16 *)((s8 *)source + 6));
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(138);
    return parent;
}

void BattleFx_SnapScaleToFull(void *obj)
{
    s32 next;
    s32 scale;

    if (obj != NULL) {
        scale = FIELD_AT_OFFSET(obj, s32 *, 0x18);
        if (scale <= 0xFFFF) {
            do {
                next = scale + 0x1000;
                scale = next;
            } while (next <= 0xFFFF);
            FIELD_AT_OFFSET(obj, s32 *, 0x18) = next;
            FIELD_AT_OFFSET(obj, s32 *, 0x1C) = next;
        }
        Object_CommitPosition();
    }
}

/* UpdateRisingParticleBurst: lift and spin the source for 31 frames, then
   burst eight item-break fragments with random speeds and headings. */
void UpdateRisingParticleBurst(void *source)
{
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    /* FAKEMATCH: retained scale-carrier scheduling trial in the uncredited bank. */
    register s32 unit asm("sl");

    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
        *(s32 *)((s8 *)source + 12) += 0x10000;
        *(u16 *)((s8 *)source + 6) += 0x2000;
        *(s32 *)((s8 *)source + 24) += -0x800;
        *(s32 *)((s8 *)source + 28) += -0x800;
        WaitFrames(1);
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
