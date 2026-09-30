#include "FIXED_MATH.H"
#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
extern u8 Data_03001f30[];
s32 BattleEffect_RunFallbackObjectTransition();

/* battle/effects/radial_camera/update.c */
struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectCamera {
    s32 filler_00;
    s32 x;
    s32 y;
    s32 z;
};

struct RadialCameraEffect {
    u8 filler_00[0xC];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    s32 filler_1c;
    s32 velocity;
    s32 acceleration;
    u8 filler_28[0xA];
    u16 field_32;
    u8 filler_34[0xC];
    s8 state;
    u8 filler_41;
    u8 flag;
};

extern struct EffectCamera *gEffectWork;

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Vector_AddPolarOffset(s32, s32, struct EffectPosition *);
extern void Camera_WorldToScreen(struct EffectPosition *);
extern s32 BattleFx_HasReachedTarget(struct RadialCameraEffect *);

void BattleFx_UpdateRadialCamera(struct RadialCameraEffect *effect)
{
    struct EffectCamera *camera;
    struct EffectPosition position;
    s8 *state_pointer;
    s16 angle;
    s32 state;

    camera = gEffectWork;
    state_pointer = &effect->state;
top:
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->source_x;
        position.z = effect->source_z;
        angle = Random16();
        Vector_AddPolarOffset(Random16() * 30 + 0x280000, (u16)angle, &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->acceleration = 0x40000;
        effect->velocity = 0x40000;
        effect->flag = state;
        goto advance;
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(effect)!= 0)
            return;
        *state_pointer = (u8)*state_pointer + 1;
        goto top;
    } else if (state == 2) {
        position.x = camera->x;
        position.y = camera->y + 0x80000;
        position.z = camera->z;
        Camera_WorldToScreen(&position);
        Vector_AddPolarOffset(0x40000, Random16(), &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->field_32 = 0x1000;
        effect->flag = 1;
advance:
        *state_pointer = (u8)*state_pointer + 1;
        return;
    } else if (state == 3) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
        return;
    } else {
        return;
    }
}
