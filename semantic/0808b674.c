#include "types.h"

struct SpawnedObject_0808b674 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 display_y;
    u8 unknown_18[0x0a];
    u8 variant;
    u8 unknown_23[0x2d];
    void *resource;
    u8 unknown_54;
    u8 motion_state;
};

struct SpawnDescriptor_0808b674 {
    s16 object_index;
    s16 related_index;
    u32 unknown_04;
    s32 x;
    s32 y;
    s32 z;
    s16 type;
    u8 unknown_16[0x1a];
};

struct SceneState_0808b674 {
    u32 scratch[4];
    u8 unknown_10[4];
    struct SpawnedObject_0808b674 *objects[98];
    u8 unknown_19c[2];
    s16 scene_mode;
    u8 unknown_1a0[0x40];
    struct SpawnedObject_0808b674 *spawned_object;
    u8 unknown_1e4[0x1c];
    struct SpawnDescriptor_0808b674 descriptor;
};

struct SpawnParameters_0808b674 {
    s32 x;
    s32 use_floor_adjustment;
    s32 z;
    s32 type;
    u16 variant;
    u8 unknown_12[4];
    u8 floor_adjusted;
    u8 unknown_17;
    s32 object_index;
};

void Func_0808b9f8(void);
void Func_0808b3ec(void *event, s32 state);
s32 Func_080091a8(s32 layer, s32 x, s32 z);
void Func_080091e0(struct SpawnedObject_0808b674 *object, s32 mode);
void Func_08009080(struct SpawnedObject_0808b674 *object, s32 mode);
struct SpawnedObject_0808b674 *Func_080090c8(
    s32 kind, s32 x, s32 y, s32 z);
void Func_080090e0(
    struct SpawnedObject_0808b674 *destination,
    struct SpawnedObject_0808b674 *source);
u8 *Func_08009048(void *resource, s32 type);

/*
 * Build an object-spawn descriptor from the pending scene parameters, apply
 * the optional floor correction, and create the visible companion object.
 */
void Func_0808b674(s32 event_id)
{
    struct SceneState_0808b674 *scene =
        *(struct SceneState_0808b674 **)0x03001ebc;
    struct SpawnParameters_0808b674 *parameters =
        (struct SpawnParameters_0808b674 *)0x0200041c;
    const u32 *descriptor_template = (const u32 *)0x0809f810;
    struct SpawnDescriptor_0808b674 *descriptor = &scene->descriptor;
    struct SpawnedObject_0808b674 *source;
    struct SpawnedObject_0808b674 *spawned;
    s32 tile_x;
    s32 tile_z;
    s32 tile_offset;
    u32 index;

    for (index = 0; index < 12; index++) {
        ((u32 *)descriptor)[index] = descriptor_template[index];
    }
    for (index = 0; index < 4; index++) {
        scene->scratch[index] = 0;
    }

    Func_0808b9f8();

    descriptor->object_index = (s16)parameters->object_index;
    descriptor->related_index = -1;
    descriptor->x = parameters->x;
    descriptor->y = 0;
    descriptor->z = parameters->z;
    descriptor->type = (s16)parameters->type;

    Func_0808b3ec(descriptor, parameters->object_index);
    Func_0808b3ec((void *)event_id, 8);

    source = scene->objects[parameters->object_index];
    source->variant = (u8)parameters->variant;

    tile_x = source->x;
    if (tile_x < 0) {
        tile_x += 0x000fffff;
    }
    tile_z = source->z;
    if (tile_z < 0) {
        tile_z += 0x000fffff;
    }
    tile_offset = ((tile_x >> 20) + ((tile_z >> 20) << 7)) * 4;

    if (parameters->use_floor_adjustment != 0 &&
        *(u8 *)(0x02010002 + tile_offset) == 0xfd &&
        *(u8 *)(0x0200fe02 + tile_offset) == 0xfd) {
        parameters->floor_adjusted = 1;
        source->y += Func_080091a8(
            0, source->x, source->z - 0x00100000) - 0x00200000;
        source->display_y = source->y;
        source->motion_state = 0;
        Func_080091e0(source, 0);
        Func_08009080(source, 12);
    } else {
        parameters->floor_adjusted = 0;
    }

    spawned = Func_080090c8(0x8000, source->x, source->y, source->z);
    spawned->display_y = source->display_y;
    Func_080090e0(spawned, source);

    if (scene->scene_mode == 3) {
        u8 *animation = Func_08009048(source->resource, 0x17);

        animation[5] = 15;
        animation[6] = 9;
    }

    **(u8 ***)0x03001e70 = (u8 *)spawned + 8;
    scene->spawned_object = spawned;
}
