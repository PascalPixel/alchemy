#include "layout_guard.h"
#include "types.h"

enum {
    WAVE_SAMPLE_COUNT_08097644 = 160,
    WAVE_BUFFER_SIZE_08097644 = 0x144,
};

typedef struct WaveBuffer_08097644 {
    s16 samples[WAVE_SAMPLE_COUNT_08097644];
    u8 padding140[4];
} WaveBuffer_08097644;

typedef struct WaveState_08097644 {
    WaveBuffer_08097644 buffers[2];
    u16 phase;
    u8 active_buffer;
    s8 map_x;
    s8 map_y;
    s8 map_layer;
    u8 padding28e[2];
    u16 target_id;
    u16 source_id;
    u8 delay;
    u8 spawn_timer;
} WaveState_08097644;

typedef struct Position_08097644 {
    s32 x;
    s32 y;
    s32 z;
} Position_08097644;

typedef struct ObjectDisplay_08097644 {
    u8 padding00[9];
    u8 attribute2_high;
    u8 padding0a[0x1c];
    u8 motion_mode;
} ObjectDisplay_08097644;

typedef void (*ObjectCallback_08097644)(void *);

typedef struct SceneObject_08097644 {
    u8 padding00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[0x1c];
    s32 speed_limit;
    s32 acceleration;
    u8 padding38[0x18];
    ObjectDisplay_08097644 *display;
    u8 padding54;
    u8 flags;
    u8 padding56[0x16];
    ObjectCallback_08097644 callback;
} SceneObject_08097644;

typedef struct Placement_08097644 {
    s16 resource_id;
} Placement_08097644;

typedef struct ResourceMetadata_08097644 {
    u8 padding00[8];
    s8 anchor_y;
} ResourceMetadata_08097644;

LAYOUT_SIZE_GUARD(
    WaveBuffer08097644_Size,
    WaveBuffer_08097644,
    WAVE_BUFFER_SIZE_08097644);
LAYOUT_OFFSET_GUARD(
    WaveState08097644_Phase,
    WaveState_08097644,
    phase,
    0x288);
LAYOUT_OFFSET_GUARD(
    WaveState08097644_TargetId,
    WaveState_08097644,
    target_id,
    0x290);
LAYOUT_OFFSET_GUARD(
    WaveState08097644_Delay,
    WaveState_08097644,
    delay,
    0x294);
LAYOUT_OFFSET_GUARD(
    ObjectDisplay08097644_MotionMode,
    ObjectDisplay_08097644,
    motion_mode,
    0x26);
LAYOUT_OFFSET_GUARD(
    SceneObject08097644_X,
    SceneObject_08097644,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject08097644_Display,
    SceneObject_08097644,
    display,
    0x50);
LAYOUT_OFFSET_GUARD(
    SceneObject08097644_Callback,
    SceneObject_08097644,
    callback,
    0x6c);
LAYOUT_SIZE_GUARD(
    SceneObject08097644_Size,
    SceneObject_08097644,
    0x70);
LAYOUT_OFFSET_GUARD(
    ResourceMetadata08097644_AnchorY,
    ResourceMetadata_08097644,
    anchor_y,
    8);

extern WaveState_08097644 *Data_03001ea8;

s32 Func_080022f4(s32, s32);
s32 Func_08002322(s32);
s16 Func_080044d0(s32, s32);
SceneObject_08097644 *Func_080090c8(s32, s32, s32, s32);
void Func_08009150(SceneObject_08097644 *, s32, s32, s32);
void Func_0808e0b0(SceneObject_08097644 *, s32);
Placement_08097644 *Func_0808d394(s32);
void Func_08091200(u32, s32);
void Func_08091254(s32);
SceneObject_08097644 *Func_08092054(u32);
void Func_080978c4(void);
ResourceMetadata_08097644 *Func_08185000(s32);
void Func_080f9010(s32);

#define TRAVEL_CALLBACK_08097644 \
    ((ObjectCallback_08097644)0x08097a55)

static s32 ObjectAnchor_08097644(u16 object_id)
{
    Placement_08097644 *placement = Func_0808d394((s16)object_id);
    ResourceMetadata_08097644 *metadata =
        Func_08185000(placement->resource_id);

    return (s32)((u32)(s32)metadata->anchor_y << 16) - 0x20000;
}

static void ReadEffectPosition_08097644(
    SceneObject_08097644 *object,
    u16 object_id,
    Position_08097644 *position)
{
    position->x = object->x;
    position->y = object->y + ObjectAnchor_08097644(object_id);
    position->z = object->z;
}

/*
 * Advance the double-buffered wave and periodically launch its traveling map
 * effect from source to target.  Wave buffers alternate every update; effect
 * launches occur at timer phases 0, 8, and 16, then wrap after phase 60.
 */
void Func_08097644(void)
{
    WaveState_08097644 *state = Data_03001ea8;
    WaveBuffer_08097644 *next_buffer;
    s32 index;

    if (state->delay != 0) {
        state->delay--;
        return;
    }

    next_buffer = &state->buffers[state->active_buffer ^ 1];
    for (index = 0; index < WAVE_SAMPLE_COUNT_08097644; index++) {
        s32 angle = (state->phase + index * 8) << 16;
        s32 wave = Func_08002322(Func_080022f4(angle, 160));

        next_buffer->samples[index] = (s16)(wave >> 14);
    }
    state->phase += 4;
    state->active_buffer ^= 1;

    if (state->active_buffer != 0) {
        u32 map_effect =
            ((u32)(s32)state->map_layer << 10) |
            ((u32)(s32)state->map_y << 5) |
            (u32)(s32)state->map_x |
            0x200000;

        Func_08091200(map_effect, 1);
        Func_08091254(1);
        Func_080978c4();
    }

    Func_0808e0b0(Func_08092054(state->target_id), 0);

    if (state->spawn_timer == 0 ||
        state->spawn_timer == 8 ||
        state->spawn_timer == 16) {
        SceneObject_08097644 *target = Func_08092054(state->target_id);
        SceneObject_08097644 *source = Func_08092054(state->source_id);

        if (target != 0 && source != 0) {
            Position_08097644 target_position;
            Position_08097644 source_position;
            SceneObject_08097644 *effect;

            ReadEffectPosition_08097644(
                target,
                state->target_id,
                &target_position);
            ReadEffectPosition_08097644(
                source,
                state->source_id,
                &source_position);
            effect = Func_080090c8(
                0x119,
                source_position.x,
                source_position.y,
                source_position.z);

            if (effect != 0) {
                effect->flags = 0;
                effect->speed_limit = 0xa3d7;
                effect->acceleration = 0xa3d7;
                effect->angle = Func_080044d0(
                    target_position.z - source_position.z,
                    target_position.x - source_position.x);
                effect->callback = TRAVEL_CALLBACK_08097644;
                effect->display->motion_mode = 0;
                effect->display->attribute2_high =
                    (effect->display->attribute2_high & (u8)~0x0c) | 4;
                Func_08009150(
                    effect,
                    target_position.x,
                    target_position.y,
                    target_position.z);
            }
        }
    }

    if (state->spawn_timer == 0)
        Func_080f9010(0x82);

    state->spawn_timer++;
    if (state->spawn_timer > 60)
        state->spawn_timer = 0;
}
