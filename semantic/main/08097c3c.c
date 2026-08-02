#include "layout_guard.h"
#include "types.h"

#define NULL ((void *)0)

struct Position_08097c3c {
    s32 x;
    s32 y;
    s32 z;
};

struct FieldObject_08097c3c {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[0x0e];
    u8 direction;
    u8 padding23;
    s32 motion_x;
    s32 motion_y;
    s32 motion_z;
    s32 scale_x;
    s32 scale_y;
    u8 padding38[0x1d];
    s8 animation;
    u8 padding56[4];
    u8 flags;
    u8 movement_state;
    u8 padding5c[0x0c];
    struct FieldObject_08097c3c *camera_target;
    s32 callback;
};

struct MovementScene_08097c3c {
    s32 direction_reference;
    u8 padding04[0x0c];
    struct FieldObject_08097c3c *camera;
    struct FieldObject_08097c3c *actor;
    s16 actor_id;
    u8 padding1a[0x1e];
    s32 actor_callback;
    const void *actor_resource;
    u8 padding40[4];
    u8 actor_mode;
};

LAYOUT_SIZE_GUARD(Position08097c3c_Size, struct Position_08097c3c, 0x0c);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Position,
    struct FieldObject_08097c3c,
    x,
    0x08);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Direction,
    struct FieldObject_08097c3c,
    direction,
    0x22);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Motion,
    struct FieldObject_08097c3c,
    motion_x,
    0x24);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Scale,
    struct FieldObject_08097c3c,
    scale_x,
    0x30);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Animation,
    struct FieldObject_08097c3c,
    animation,
    0x55);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Flags,
    struct FieldObject_08097c3c,
    flags,
    0x5a);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_MovementState,
    struct FieldObject_08097c3c,
    movement_state,
    0x5b);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_CameraTarget,
    struct FieldObject_08097c3c,
    camera_target,
    0x68);
LAYOUT_OFFSET_GUARD(
    FieldObject08097c3c_Callback,
    struct FieldObject_08097c3c,
    callback,
    0x6c);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_Camera,
    struct MovementScene_08097c3c,
    camera,
    0x10);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_Actor,
    struct MovementScene_08097c3c,
    actor,
    0x14);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_ActorId,
    struct MovementScene_08097c3c,
    actor_id,
    0x18);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_ActorCallback,
    struct MovementScene_08097c3c,
    actor_callback,
    0x38);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_ActorResource,
    struct MovementScene_08097c3c,
    actor_resource,
    0x3c);
LAYOUT_OFFSET_GUARD(
    MovementScene08097c3c_ActorMode,
    struct MovementScene_08097c3c,
    actor_mode,
    0x44);

extern struct MovementScene_08097c3c *Data_03001f30;
extern u32 Data_03001ae8;
extern u32 Data_03001c94;
extern u32 Data_03001e40;

void Func_080030f8(u32 frames);
void Func_0800447c(s32 distance, s32 direction, struct Position_08097c3c *position);
void Func_08009080(struct FieldObject_08097c3c *object, s32 animation);
void Func_08009098(struct FieldObject_08097c3c *object, const void *resource);
void Func_08009150(struct FieldObject_08097c3c *object, s32 x, s32 y, s32 z);
void Func_08009158(struct FieldObject_08097c3c *object);
s32 Func_080091d8(
    struct FieldObject_08097c3c *object,
    struct Position_08097c3c *position);
struct FieldObject_08097c3c *Func_080092a0(
    struct FieldObject_08097c3c *object,
    struct Position_08097c3c *position);
s32 Func_080092a8(u8 direction);
void Func_08009240(struct FieldObject_08097c3c *object, u8 mode);
void Func_08097174(void);
void Func_08097384(void);
void Func_0809748c(void);
u16 Func_08097b54(u32 held_keys);
struct FieldObject_08097c3c *Func_08098070(struct FieldObject_08097c3c *camera);
void Func_08098184(struct FieldObject_08097c3c *object);
void Func_080981b0(struct FieldObject_08097c3c *object);
struct FieldObject_08097c3c *Func_08092054(s32 actor_id);
void Func_080f9010(s32 sound);

static void ActorPosition_08097c3c(
    const struct FieldObject_08097c3c *actor,
    s32 y_offset,
    struct Position_08097c3c *position
) {
    position->x = actor->x;
    position->y = actor->y + y_offset;
    position->z = actor->z;
}

static void PlaceMarkerAtActor_08097c3c(
    struct FieldObject_08097c3c *marker,
    const struct FieldObject_08097c3c *actor,
    s32 reference_direction
) {
    struct Position_08097c3c position;

    ActorPosition_08097c3c(actor, 0x100000, &position);
    Func_0800447c(0x100000, reference_direction, &position);
    Func_08009150(marker, position.x, position.y, position.z);
}

static s32 SameMapCell_08097c3c(
    const struct FieldObject_08097c3c *object,
    const struct Position_08097c3c *position
) {
    return (object->x & 0xfff00000) == (position->x & 0xfff00000) &&
        (object->z & 0xfff00000) == (position->z & 0xfff00000);
}

static s32 DestinationBlocked_08097c3c(
    struct FieldObject_08097c3c *actor,
    struct FieldObject_08097c3c *camera,
    struct Position_08097c3c *destination,
    struct Position_08097c3c *beyond,
    s32 *move_camera
) {
    struct FieldObject_08097c3c *occupant;

    if (Func_080091d8(actor, destination) > 0) {
        return 1;
    }

    occupant = Func_080092a0(actor, destination);
    if (occupant == NULL) {
        return 0;
    }
    if (occupant != camera || SameMapCell_08097c3c(camera, destination)) {
        return 1;
    }
    if (!SameMapCell_08097c3c(camera, beyond)) {
        return 0;
    }
    if (Func_080092a8(camera->direction) != 0) {
        return 1;
    }

    *move_camera = 1;
    return 0;
}

static void RejectDestination_08097c3c(struct FieldObject_08097c3c *marker) {
    Func_08009080(marker, 4);
    if ((Data_03001e40 & 15) == 0) {
        Func_080f9010(0x72);
    }
}

/* Runs the interactive destination-selection and movement scene. */
void Func_08097c3c(void) {
    static const u8 *const direction_animations = (const u8 *)0x0809F118;
    struct MovementScene_08097c3c *scene = Data_03001f30;
    struct FieldObject_08097c3c *camera = scene->camera;
    struct FieldObject_08097c3c *actor = scene->actor;
    struct FieldObject_08097c3c *marker;
    s32 direction_reference = scene->direction_reference + 0x8000;
    s32 move_camera = 0;

    if (actor == NULL) {
        return;
    }

    Func_08097384();
    camera->camera_target = actor;
    Func_08009098(camera, (const void *)0x0809F0BC);
    marker = Func_08098070(camera);
    if (marker == NULL) {
        Func_0809748c();
        return;
    }

    marker->camera_target = actor;
    PlaceMarkerAtActor_08097c3c(marker, actor, direction_reference);
    Func_08098184(marker);
    marker->scale_x = 0x40000;
    marker->scale_y = 0x8000;
    marker->animation = 4;

    actor->callback = 0x08096B89;
    actor->scale_x = 0x6666;
    actor->scale_y = 0x3333;
    actor->flags = 0;
    actor->direction = 2;

    for (;;) {
        struct Position_08097c3c destination;
        struct Position_08097c3c beyond;
        s32 chosen_x;
        s32 chosen_z;
        u16 selected_direction;

        Func_080030f8(1);
        if (Data_03001c94 & 0x303) {
            break;
        }

        selected_direction = Func_08097b54(Data_03001ae8);
        if (selected_direction == 0xffff) {
            PlaceMarkerAtActor_08097c3c(marker, actor, direction_reference);
            Func_08009080(marker, 1);
            marker->motion_x = 0;
            marker->motion_y = 0;
            marker->motion_z = 0;
            continue;
        }

        ActorPosition_08097c3c(actor, 0x100000, &destination);
        Func_0800447c(0x100000, direction_reference, &destination);
        Func_0800447c(0x20000, selected_direction, &destination);
        Func_08009150(marker, destination.x, destination.y, destination.z);
        Func_08009158(marker);

        ActorPosition_08097c3c(actor, 0, &destination);
        Func_0800447c(0x100000, selected_direction, &destination);
        ActorPosition_08097c3c(actor, 0, &beyond);
        Func_0800447c(0x200000, selected_direction, &beyond);

        if (DestinationBlocked_08097c3c(
                actor, camera, &destination, &beyond, &move_camera)) {
            RejectDestination_08097c3c(marker);
            continue;
        }

        Func_080f9010(0xaf);
        chosen_x = destination.x;
        chosen_z = destination.z;
        Func_08009080(
            marker,
            direction_animations[
                ((u32)(direction_reference - selected_direction) << 16) >> 30]);
        Func_080030f8(15);

        actor->movement_state = 0;
        actor->scale_x = 0x3333;
        actor->scale_y = 0x3333;
        Func_08009150(actor, destination.x, destination.y, destination.z);

        marker->animation = 0;
        marker->scale_x = 0x3333;
        marker->scale_y = 0x3333;
        Func_0800447c(0x100000, selected_direction, &destination);
        Func_08009150(
            marker,
            destination.x,
            destination.y + 0x100000,
            destination.z);

        if (move_camera) {
            struct FieldObject_08097c3c *linked_actor =
                Func_08092054(scene->actor_id);

            linked_actor->flags &= (u8)~1;
            camera->scale_x = 0x3333;
            camera->scale_y = 0x3333;
            Func_08009150(camera, beyond.x, beyond.y, beyond.z);
        }

        Func_08009158(actor);
        actor->x = chosen_x;
        actor->z = chosen_z;
        actor->motion_x = 0;
        actor->motion_z = 0;
        break;
    }

    Func_08009240(actor, scene->actor_mode);
    Func_08009098(actor, scene->actor_resource);
    actor->callback = scene->actor_callback;
    Func_08097174();
    if (move_camera) {
        Func_08092054(scene->actor_id)->flags |= 1;
    }
    Func_0809748c();
    Func_080981b0(marker);
}
