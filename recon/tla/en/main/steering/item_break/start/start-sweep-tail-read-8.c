/* TLA-EN BattleFx_StartItemBreak: 276 emitted bytes versus the complete
   276-byte current native template; 26 differing byte positions after excluding
   the template's external relocation words. Calls and address pools are not
   linked in this comparison. This trial is uncredited and is not an adoption. */

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

void *BattleFx_StartItemBreak(struct ItemBreakObject *source)
{
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

    angle = (source->angle + 0x2000) & 0xc000;
    parent = Object_Spawn(0x116, source->x,
                           source->y + 0x100000,
                           source->z);
    if (parent == 0)
        return 0;
    parent->scale_y = 0x4000;
    parent->scale_x = 0x4000;
    parent->update = Func_080dcf8c;
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
                u32 other = Random16();
                u32 heading = source->angle;
    /* FAKEMATCH: retained empty-asm value dependency to measure instruction scheduling. */
                asm volatile("" : : "r"(heading), "r"(other));
                Motion_SetTargetPositionFromMagnitudeAngle(child, fragment_height, ((rotation_jitter - other) >> 3) + heading);
            }
        }
        fragment_count--;
    } while (fragment_count >= 0);
    Audio_PlayCue(138);
    return parent;
}
