#include "types.h"
#include "b5_context.h"
#include "motion_object.h"

/*
 * Draft for the battle-presentation sub-effect at 0x080cb1a4.
 *
 * Assigned from the member_orbit/run.c compiler-family cluster
 * (template-main-080ce85c, games/gs1/src/battle/effects/member_orbit/run.c),
 * but the real callee set and constants match the 0x03001eec "battle work"
 * subsystem documented there and in games/gs1/src/battle/effects/puff_arc/
 * run.c and games/gs1/recon/en/main/080e01e4.c: same Func_080cd594(mode) /
 * Func_080e0524((s32)&Value_XXXXXXXX, work, f, f) / work-offsets
 * 0x7780/0x7784/0x7824/0x7828 shape, the same 96-pass outer loop as
 * 080e01e4.c, and the same 0x7780=2 / 0x7784=75 pair as puff_arc's
 * WORK_EFX->layers==2 branch.
 *
 * Unlike any of those siblings, this owner also drives a live
 * `struct MotionObject` pair through Object_ResetMotion/Object_SetPosition/
 * Object_SetMode (Func_08009140/Func_08009150/Func_08009080, aliased in
 * types.h) and FixedPoint_Ratio (Func_080022ec) -- the same low-level shape
 * games/gs1/src/battle/motion/set_approach_motion.c uses, but inlined here
 * with its own scale (90, not 80) and without that function's
 * snap_to_target/vertical_motion_strength/acceleration/speed_limit tail,
 * which this owner instead assigns directly on specific frames. The two
 * `struct B5Context` lookups key off the effect state's field 8 (the
 * source actor, named `actor` in puff_arc's Efx struct) and the first
 * entry of its member-id array at field 0x24 (36) -- the field this whole
 * family already uses for its per-member id list.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

extern u8 Value_0000007d;

void Func_080cd594(s32 mode);
void Func_080e0524(s32 effect_id, void *work, s32 flag_a, s32 flag_b);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
s32 Func_080b5070(s32 member_id);
void Func_080e3944(void *source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080e155c(s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);

void Func_080cb1a4(void *object_param)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    s32 status;
    DrawRectangleFn draw;
    struct B5Context *first_context;
    struct B5Context *second_context;
    struct MotionObject *object;
    struct MotionObject *target;
    s32 x;
    s32 z;
    s32 y_offset_actor;
    s32 y_offset_member;
    s32 record[3];
    s32 screen[3];
    s32 *record_ptr;
    s32 *screen_ptr;
    s32 frame;
    s32 cel_phase;
    s32 radius;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object_param;
    Func_080cd594(0);
    Func_080e0524((s32)&Value_0000007d, work, 1, 1);

    status = Func_080ed408(46, 7, 7, 3, 2);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    draw = (DrawRectangleFn)heap_cache[7];
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);

    first_context = Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    second_context = Func_080b5098(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));
    object = first_context->object;
    target = second_context->object;

    x = object->x + FixedPoint_Ratio(90 * (target->x - object->x), 100);
    z = object->z + FixedPoint_Ratio(90 * (target->z - object->z), 100);

    y_offset_actor = Func_080b5070(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    y_offset_member = Func_080b5070(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));

    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);

    object->snap_to_target = 1;
    object->auto_face_motion = 1;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;

    Func_080030f8(20);
    frame = 0;

    record_ptr = record;
    screen_ptr = screen;

    for (cel_phase = -56, radius = -46; frame != 96;
            frame++, cel_phase++, radius++) {
        s32 facing;

        facing = *(s32 *)0x03001E80;
        Func_080049ac();
        Func_080051d8(facing, facing + 12);

        if (frame == 0) {
            target->velocity_y = 0xF0000;
            target->vertical_motion_strength = 0xAB85;
            object->velocity_y = 0xF0000;
            object->vertical_motion_strength = 0xAB85;
        }
        if (frame == 11) {
            M2C_FIELD(target, s32 *, 0x1C) = -M2C_FIELD(target, s32 *, 0x1C);
            M2C_FIELD(object, s32 *, 0x1C) = -M2C_FIELD(object, s32 *, 0x1C);
            object->y += y_offset_actor;
            target->y += y_offset_member;
        }
        if (frame == 54) {
            Func_080d6888(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, 5, 0, 10);
            target->velocity_y = 0x80000;
            target->vertical_motion_strength = 0x91EB;
            object->velocity_y = 0x50000;
            object->vertical_motion_strength = 0x7851;
            object->acceleration = 0x10000;
            object->speed_limit = 0x20000;
            object->auto_face_motion = 0;
            Object_ResetMotion(object);
            Object_SetPosition(object, 0, 0, object->z);
        }

        Func_080049ac();
        Func_080051d8(facing, facing + 12);

        record_ptr[0] = object->x;
        record_ptr[1] = object->y;
        record_ptr[2] = object->z;
        Func_080e3944(record_ptr, screen_ptr);
        screen_ptr[0] = screen_ptr[0] >> 1;

        if (frame == 54 || frame == 55) {
            draw(canvas, work, screen_ptr[0] - 16, screen_ptr[1] - 16,
                32, 64);
        }

        if (cel_phase >= 0 && cel_phase <= 11) {
            s32 i;
            s32 offset;

            offset = (cel_phase / 2) << 11;
            for (i = 0; i != 16; i++) {
                s32 angle;
                s32 rx;
                s32 ry;

                angle = i << 12;
                rx = (screen_ptr[0]
                    + ((radius * Func_08002322(angle)) >> 16)) - 16;
                ry = (radius * Func_0800231c(angle) >> 16) - frame + 100;
                draw(canvas, (u8 *)work + offset, rx, ry, 32, 64);
            }
        }

        if (frame == 64) {
            M2C_FIELD(target, s32 *, 0x1C) = -M2C_FIELD(target, s32 *, 0x1C);
            M2C_FIELD(object, s32 *, 0x1C) = -M2C_FIELD(object, s32 *, 0x1C);
            object->y -= y_offset_actor;
            target->y -= y_offset_member;
            Object_SetMode(object, 0);
        }

        if (frame == 54) {
            Func_080b50e8(134);
        }
        if (frame == 0) {
            Func_080f9010(136);
            M2C_FIELD(work, s32 *, 0x77A8) = 6;
        }
        if (frame == 53) {
            M2C_FIELD(work, s32 *, 0x77A8) = 6;
        }

        Func_080e155c(16, 16);
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
