#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "effect_0809b11c.h"
#include "object_lookup.h"

/* battle/effects/runtime/upd/update_spin_angle.c */
void BattleFx_UpdateSpinAngle(void *object)
{
    u32 angle_step;

    angle_step = FIELD_AT_OFFSET(object, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(object, u16 *, 6) = (u16)(FIELD_AT_OFFSET(object, u16 *, 6) + angle_step + 0x1000);
    if (angle_step < 0x1000U) {
        FIELD_AT_OFFSET(object, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(object, s16 *, 0x64) + 1);
    }
}

/* battle/effects/runtime/upd/update_descending_object.c */
struct DescendingObjectState {
  u8 padding_00[6];
  u16 angle;
  u8 padding_08[4];
  u32 scale;
  u8 padding_10[8];
  u32 primary_position;
  u32 secondary_position;
  u8 padding_20[0x34];
  s8 active;
};
void BattleFx_UpdateDescendingObject(struct DescendingObjectState *state)
{
  u32 next_position;
  state->secondary_position -= 0x280;
  next_position = state->primary_position - 0x280;
  state->angle = state->angle + 0x2000;
  state->scale = state->scale + 0x10000;
  state->primary_position = next_position;
  if ((s32)next_position < 0x3000) {
    state->active = 0;
  }
}

/* battle/effects/particles/update_radial_launch.c */
struct Output {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_0809b11c {
    u8 padding00[8];
    struct Output position;
};

struct RuntimeState_0809b11c {
    u8 padding00[0x10];
    struct PositionSource_0809b11c *position_source;
};

extern struct RuntimeState_0809b11c *gIw;
extern u32 gIw2;

void NormalizeVector(struct Output *);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, struct Output *);
void Audio_PlayCue(s32);

void BattleFx_UpdateRadialLaunch(struct EffectSlot *effect)
{
    struct Output position;
    struct PositionSource_0809b11c *source;
    s32 state;
    u32 first_random;

    source = gIw->position_source;
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;
        NormalizeVector(&position);

        effect->x = position.x;
        effect->z = (s32)((u32)position.z + 0x80000);
        effect->origin_z = effect->z;
        effect->origin_x = effect->x;
        position.z = effect->origin_z;
        position.x = effect->origin_x;

        first_random = Rand();
        RotateVectorByMagnitude(
            0x780000,
            ((first_random << 13) >> 16)
                - ((Rand() << 13) >> 16)
                + 0xc000,
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x50000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((gIw2 & 2) != 0)
            Audio_PlayCue(0xf6);
    } else if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            Battle_Run(effect);
    }
}

/* battle/effects/run/run_effect15.c */
struct EffectPosition { s32 x, y, z; };
struct EffectObject {
    u8 pad00[100];
    s16 timer;
    u8 pad66[6];
    void (*callback)(void);
};

extern u8 *gIw;
extern u8 gCell[];

void BattleFx_RunEffect15(void)
{
    u8 *scene = gIw;
    u8 *main_object = *(u8 **)(scene + 16);
    struct EffectObject *effect = (struct EffectObject *)main_object;
    struct EffectPosition position;
    u8 *particle;
    u8 *entry;
    u8 stopped;
    u32 index;

    Battle_Run();
    Battle_SetMode(-1, -1, -1, 0);
    Battle_unk2_4();
    Battle_Do(10);
    Battle_Place(*(s16 *)(scene + 24), 0x4000, 0);
    Battle_Do(30);
    *(void (**)(void))(main_object + 108) = Battle_unk3_4;
    Battle_unk2_2(0x83);
    Battle_Apply(main_object, 28);
    Battle_Do(40);
    Battle_unk2_2(0xdc);
    Battle_Apply2(main_object, 0);
    Battle_Apply(main_object, 3);
    *(void (**)(void))(main_object + 108) = Battle_unk4_4;
    effect->timer = 0;
    Battle_Do(70);
    Battle_Apply3(main_object, 0);
    *(u8 *)(main_object + 85) = 0;
    *(void (**)(void))(main_object + 108) = Battle_unk5_2;
    *(u32 *)(main_object + 56) = 0x80000000;
    position.x = *(s32 *)(main_object + 8);
    position.y = *(s32 *)(main_object + 12);
    position.z = *(s32 *)(main_object + 16);
    Battle_unk3_2(&position);
    index = 0;
    particle = scene;
    particle += 88;
    do {
        s32 speed;
        Battle_SetMode2(particle, 0x11c, position.x, position.z);
        Battle_Apply4(particle, Battle_unk6_2);
        Battle_Apply5(particle, 7);
        Battle_Apply6(*(void **)particle, (Battle_unk7_2() * 7) >> 16);
        speed = (Battle_unk7_2() >> 1) + 0x13333;
        *(s32 *)(particle + 44) = speed;
        *(s32 *)(particle + 40) = speed;
        index++;
        Battle_Do(1);
        particle += 72;
    } while (index <= 23);
    Battle_Do(70);
    index = 0;
    entry = scene;
    stopped = 2;
    entry += 152;
    do {
        if (*(s8 *)(entry + 5) != 0) {
            entry[0] = stopped;
        }
        index++;
        entry += 72;
    } while (index <= 23);
    Battle_Do(40);
    Battle_unk8_2();
    Battle_Do(10);
}

/* battle/effects/particles/update_descending_positive_arc.c */
extern s16 gCell[];
extern u8 Value_00000001;

void Object_Destroy(void);

void BattleFx_UpdateDescendingParticlePositiveArc(void *arg0)
{
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (gCell[237] == (s32)&Value_00000001)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Object_Destroy();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) + ((0x10000 - speed) * 5) + 0x90000;
}

/* battle/effects/particles/update_descending_negative_arc.c */
extern s16 gCell[];
extern u8 Value_00000001;

void Object_Destroy(void);

void BattleFx_UpdateDescendingParticleNegativeArc(void *arg0)
{
    u8 *object;
    u8 *source;
    s32 position;
    s32 threshold;
    s32 speed;

    object = arg0;
    threshold = *(s32 *)(object + 0x14) + 0xA0000;
    source = *(u8 **)(object + 0x68);
    if (gCell[237] == (s32)&Value_00000001)
        threshold = *(s32 *)(object + 0x14) + 0x40000;

    position = *(s32 *)(object + 0x0C);
    if (position <= threshold) {
        Object_Destroy();
        return;
    }

    speed = *(s32 *)(object + 0x18) + 0xC00;
    if (speed > 0x10000)
        speed = 0x10000;
    *(s32 *)(object + 0x18) = speed;
    *(s32 *)(object + 0x1C) = -speed;
    *(s32 *)(object + 8) = *(s32 *)(source + 8);
    *(s32 *)(object + 0x0C) = position + (s32)0xFFFE0000;
    *(s32 *)(object + 0x10) =
        *(s32 *)(source + 0x10) - ((0x10000 - speed) * 5) + 0x100000;
}

/* battle/effects/misc/update_effect16_state.c */
struct BattleEffect16GlobalState {
    u8 unknown_000[0x1F4];
    u32 active_object_id;
};

extern struct BattleEffect16GlobalState gCell;
extern u32 gIw;
s32 UnsignedModulo(u32, s32);

void BattleFx_UpdateEffect16State(void)
{
    s32 phase;
    u8 *effect_state;
    u8 *child_state;
    register u8 *state_byte;
    register u8 state_value;

    effect_state = *(u8 **)(ObjectTable_Get(gCell.active_object_id) + 0x50);
    child_state = *(u8 **)(effect_state + 0x28);
    phase = UnsignedModulo(gIw, 5);
    if (phase == 0) {
        state_byte = effect_state + 0x25;
        *state_byte = 1;
        state_byte++;
        state_value = 3;
        goto write_value;
    }
    if (phase == 2) {
        state_byte = effect_state + 0x25;
        child_state[5] = 0;
        state_value = 1;
        *state_byte = state_value;
        state_byte++;
write_value:
        *state_byte = state_value;
    }
}

/* battle/effects/particles/update_paired_arc_spawner.c */
extern s16 gCell[];
extern u8 Value_00000001;

s32 Modulo(s32, s32);
void BattleFx_SpawnPairedArcObjects(void *);

void BattleFx_UpdatePairedArcSpawner(void *arg0)
{
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)arg0 + 0x64);
    counter = (*(u16 *)((u8 *)arg0 + 0x66))++;

    if (gCell[237] == (s32)&Value_00000001) {
        if (Modulo(counter, 7) == 0)
            BattleFx_SpawnPairedArcObjects(arg0);
    } else if (Modulo(counter, 5) == 0) {
        BattleFx_SpawnPairedArcObjects(arg0);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)arg0 + 6) += 0xC00;
}
