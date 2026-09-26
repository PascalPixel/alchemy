/* Draft: complete 432-byte owner; 2 differing halfwords.
 * Only mov fp,r0 / mov sl,r8 setup order remains; typed bitfields and
 * branch-local positions close the prior 207-halfword structural mismatch.
 * An allocator-guided explicit buffer and scoped counter did not close it.
 * A pre-decrement 24-count loop canonicalizes to the same 432-byte body
 * and leaves the two setup moves reversed (2026-09-26).
 */
#include "TYPES.H"


struct BurstParticleVisual {
    u8 reserved_00[5];
    u8 flags_a_low : 5;
    u8 flip : 1;
    u8 flags_a_high : 2;
    u8 reserved_06;
    u8 flags_b_low : 6;
    u8 flags_b_high : 2;
    union {
        struct { u16 shape : 10; u16 unused : 6; } bits;
        struct { u8 lo; u8 high_low : 4; u8 palette : 4; } bytes;
    } attributes;
};

struct BurstParticleVisualGroup {
    struct BurstParticleVisual primary;
    struct BurstParticleVisual child;
};

struct BurstParticleObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[16];
    s32 velocity_x;
    s32 velocity_y;
    u8 reserved_38[24];
    struct BurstParticleVisualGroup *visuals;
    u8 reserved_54;
    u8 mode;
};

struct BurstParticleState {
    s32 variant;
    s32 x;
    s32 y;
    s32 z;
    struct BurstParticleObject *target;
};

extern struct BurstParticleState *Data_03001f30;
extern u8 Data_0809f0b4;

void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32, u32, s32 *);
struct BurstParticleObject *Object_Spawn(s32, s32, s32, s32);
void Func_08009240(struct BurstParticleObject *, s32);
void Func_08009080(struct BurstParticleObject *, s32);
void Func_08009098(struct BurstParticleObject *, void *);
void Func_080091e0(struct BurstParticleObject *, s32);
u32 Random16(void);
void Func_08009150(struct BurstParticleObject *, s32, s32, s32);
void Func_080f9010(s32);
void WaitFrames(s32);

void BattleEffect_SpawnBurstParticleField(void)
{
    struct BurstParticleState *state;
    struct BurstParticleObject *target;
    s32 position[3];
    s32 remaining;

    state = Data_03001f30;
    target = state->target;
    BattleEffect_InitializeSharedScene();
    remaining = 24;
    while (--remaining >= 0) {
        struct BurstParticleObject *object;
        struct BurstParticleVisual *visual;
        struct BurstParticleVisual *child;
        s32 random_distance;

        if (state->variant == 0x4000) {
            position[0] = target->x;
            position[1] = target->y + 0xa0000;
            position[2] = target->z;
        } else if (state->variant == 0xc000) {
            position[0] = target->x;
            position[1] = target->y + 0x180000;
            position[2] = target->z;
        } else {
            position[0] = target->x;
            position[1] = target->y + 0xa0000;
            position[2] = target->z;
            Vector_AddPolarOffset(0xa0000, state->variant, position);
        }

        object = Object_Spawn(
            0x11c, position[0], position[1], position[2]);
        visual = &object->visuals->primary;
        child = &object->visuals->child;
        child->flip = visual->flip;
        child->flags_a_high = visual->flags_a_high;
        child->flags_b_high = visual->flags_b_high;
        child->attributes.bits.shape = visual->attributes.bits.shape;
        child->attributes.bytes.palette = visual->attributes.bytes.palette;

        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->velocity_y = 0x18000;
            object->velocity_x = 0x18000;
            object->mode = 0;
            Func_08009240(object, 11);
            Func_08009080(object, 7);
            Func_08009098(object, &Data_0809f0b4);
            Func_080091e0(object, 1);

            position[0] = state->x;
            position[1] = state->y;
            position[2] = state->z;
            if (state->variant == 0xc000)
                Vector_AddPolarOffset(0xe0000, state->variant, position);
            random_distance = Random16() * 6 + 0x40000;
            Vector_AddPolarOffset(random_distance, Random16(), position);
            Func_08009150(
                object, position[0], position[1], position[2]);
        }
        Func_080f9010(0x83);
        WaitFrames(2);
    }
    WaitFrames(8);
}
