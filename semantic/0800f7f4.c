#include "types.h"

struct MotionObject_0800f7f4 {
    u8 unknown_00[4];
    u16 step;
    s16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0xE];
    u8 map_layer;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
};

struct ProbePosition_0800f7f4 {
    s32 x;
    s32 y;
    s32 z;
};

struct MapCell_0800f7f4 {
    u16 tile;
    u8 collision;
    u8 attribute;
};

struct MotionMonitor_0800f7f4 {
    u8 unknown_000[0x19C];
    u16 obstruction_frames;
};

void Func_0800447c(s32, s32, struct ProbePosition_0800f7f4 *);
void Func_0800c300(void *, s32);
s32 Func_0800d14c(void *, s32, s32, s32);
s32 Func_08011f54(u8, s32, s32);
s32 Func_0801219c(struct ProbePosition_0800f7f4 *);

/*
 * Probe and schedule one directional movement step.
 *
 * A direction table supplies the vector and command variant.  The candidate
 * must remain on a compatible collision cell and within the permitted height
 * discontinuity; otherwise motion is cancelled and its target is invalidated.
 */
s32 Func_0800f7f4(void *arg0) {
    struct MotionObject_0800f7f4 *object =
        (struct MotionObject_0800f7f4 *)arg0;
    volatile u32 *input = (volatile u32 *)0x03001AE8;
    const u16 *direction_table = (const u16 *)0x08013254;
    struct MotionMonitor_0800f7f4 *monitor;
    struct ProbePosition_0800f7f4 candidate;
    u16 direction;
    s32 command = 12;
    s32 obstruction = 4;

    object->acceleration = 0x4000;
    object->speed_limit = 0x8000;
    direction = direction_table[(*input >> 4) & 0xF];

    if (direction != 0xFFFF) {
        u16 direction_class = direction & 0xF000;
        s32 delta_z;
        s32 current_x;
        s32 candidate_x;
        s32 cell_z;
        struct MapCell_0800f7f4 *map =
            (struct MapCell_0800f7f4 *)0x02010000;

        command = 14;
        if (direction_class != 0) {
            command = 15;
            if (direction_class != 0x8000) {
                command = 10;
            }
        }
        obstruction = 0;

        candidate.x = 0;
        candidate.y = 0;
        candidate.z = 0;
        Func_0800447c(0x80000, direction, &candidate);

        delta_z = candidate.z;
        candidate.x =
            (s32)((u32)candidate.x + (u32)object->x);
        if (delta_z < 0) {
            object->facing = (s16)0xC000;
        }
        if (delta_z > 0) {
            object->facing = 0x4000;
        }
        candidate.y =
            (s32)((u32)object->y - (u32)delta_z);
        candidate.z = object->z;

        current_x = object->x;
        if (current_x < 0) {
            current_x += 0xFFFFF;
        }
        current_x >>= 20;

        cell_z = object->z;
        if (cell_z < 0) {
            cell_z += 0xFFFFF;
        }
        cell_z = (cell_z >> 20) << 7;

        candidate_x = candidate.x;
        if (candidate_x < 0) {
            candidate_x += 0xFFFFF;
        }
        candidate_x >>= 20;

        if (Func_0801219c(&candidate) != 0 ||
            map[current_x + cell_z].collision !=
                map[candidate_x + cell_z].collision) {
            obstruction = 4;
            command = 12;
        } else if ((*input & 0x40) != 0) {
            s32 lower_height = Func_08011f54(
                object->map_layer,
                candidate.x,
                (s32)((u32)candidate.z - 0x100000U));
            s32 height_delta =
                (s32)((u32)lower_height - (u32)object->y);

            if (height_delta < 0x100000) {
                obstruction = 1;
                command = 12;
            }
        } else if ((*input & 0x80) != 0) {
            s32 upper_height = Func_08011f54(
                object->map_layer,
                candidate.x,
                candidate.z);
            s32 height_delta =
                (s32)((u32)upper_height - (u32)object->y);

            if (height_delta > -0x80000) {
                obstruction = 1;
                command = 12;
            }
        }
    }

    monitor = *(struct MotionMonitor_0800f7f4 **)0x03001EBC;
    if (monitor != 0) {
        if ((obstruction & 3) != 0) {
            monitor->obstruction_frames++;
        } else {
            monitor->obstruction_frames = 0;
        }
    }

    Func_0800c300(object, command);
    if (obstruction != 0) {
        object->target_x = (s32)0x80000000U;
        object->target_y = (s32)0x80000000U;
        object->target_z = (s32)0x80000000U;
        object->velocity_x = 0;
        object->velocity_y = 0;
        object->velocity_z = 0;
    } else {
        Func_0800d14c(
            object,
            candidate.x,
            candidate.y,
            candidate.z);
    }

    object->step++;
    return 1;
}
