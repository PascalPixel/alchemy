/* Near miss: score 460: one shift and four reordered instructions in the
   spread's setup. ⚓️ reads the current owner from gPartyState and the
   effect slot from the shared EFFECT_SLOT.H. */
#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
u32 BattleFx_HasReachedTarget(struct EffectSlot *);

/* Object updates of the radial spread page effect. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

struct Triple08095fcc {
    s32 x;
    s32 y;
    s32 z;
};

struct Object08095fcc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[80];
    u16 timer;
    s16 angle;
};

struct Output_08096048 {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_08096048 {
    u8 padding00[8];
    struct Output_08096048 position;
};

#include "PARTY_STATE.H"
extern u32 gFrameTick;

s32 Object_GetById(u32);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
void Vector_AddPolarOffset(s32, s32, void *);
void Camera_WorldToScreen(void *);
void Audio_PlayCue(s32);
void Object_Destroy();

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect)
{
    struct Output_08096048 position;
    struct PositionSource_08096048 *source;
    s32 state;
    u32 random;

    source = (struct PositionSource_08096048 *)
        Object_GetById(gPartyState.current_owner);
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;

        random = Random16() * 10 + 0xa0000;
        Vector_AddPolarOffset(
            random,
            Random16(),
            &position);
        Camera_WorldToScreen(&position);

        effect->origin_x = position.x;
        effect->origin_z = position.z;
        effect->x = position.x;
        effect->z = position.z;
        position.x = effect->x;
        position.z = effect->z;

        Vector_AddPolarOffset(0x780000, 0xc000, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x10000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((gFrameTick & 1) != 0)
            Audio_PlayCue(0x90);
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
    }
}
