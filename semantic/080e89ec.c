#include "layout_guard.h"
#include "types.h"

#define REG_BLDALPHA_080E89EC (*(volatile u16 *)0x04000052)
#define CAMERA_080E89EC (*(struct Camera_080e89ec **)0x03001e80)

typedef void (*Renderer_080e89ec)(s32, void *, s32, s32, u32, s32);

struct Particle_080e89ec {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct SceneContext_080e89ec {
    u8 reserved00[4];
    s32 side;
    u8 reserved08[0x1c];
    s16 actor_id;
};

struct SceneWork_080e89ec {
    u8 graphics[0x3c00];
    u8 trail_graphics[0x3480];
    struct Particle_080e89ec particles[0x40];
    s32 scene_phase;
    s32 scene_timer;
    u8 reserved7788[0x20];
    s32 burst_phase;
    u8 reserved77ac[0x78];
    s32 frame_ready;
    struct SceneContext_080e89ec *context;
};

struct RuntimeHeader_080e89ec {
    struct SceneWork_080e89ec *work;
    s32 render_target;
    u8 *sprite_sheet;
};

struct Camera_080e89ec {
    u8 reserved00[0x36];
    s16 horizontal_offset;
};

LAYOUT_SIZE_GUARD(Particle080e89ec_Size, struct Particle_080e89ec, 0x1c);
LAYOUT_OFFSET_GUARD(Particle080e89ec_Timer, struct Particle_080e89ec, timer, 0x18);
LAYOUT_OFFSET_GUARD(SceneContext080e89ec_Side, struct SceneContext_080e89ec, side, 0x04);
LAYOUT_OFFSET_GUARD(SceneContext080e89ec_Actor, struct SceneContext_080e89ec, actor_id, 0x24);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_TrailGraphics, struct SceneWork_080e89ec, trail_graphics, 0x3c00);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_Particles, struct SceneWork_080e89ec, particles, 0x7080);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_Phase, struct SceneWork_080e89ec, scene_phase, 0x7780);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_BurstPhase, struct SceneWork_080e89ec, burst_phase, 0x77a8);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_FrameReady, struct SceneWork_080e89ec, frame_ready, 0x7824);
LAYOUT_OFFSET_GUARD(SceneWork080e89ec_Context, struct SceneWork_080e89ec, context, 0x7828);
LAYOUT_OFFSET_GUARD(Camera080e89ec_Offset, struct Camera_080e89ec, horizontal_offset, 0x36);

static const struct RuntimeHeader_080e89ec *const Runtime_080e89ec =
    (const struct RuntimeHeader_080e89ec *)0x03001eec;
static const u16 *const FrameOffsets_080e89ec = (const u16 *)0x080edeb2;
static const u8 *const FrameWidths_080e89ec = (const u8 *)0x080ede9f;
static const u8 *const FrameHeights_080e89ec = (const u8 *)0x080edea5;
static const u8 *const FrameY_080e89ec = (const u8 *)0x080edeab;
static const u16 *const ParticleTiles_080e89ec = (const u16 *)0x080ede48;

void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
u32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_08004278(void *);
void Func_080049ac(void);
void Func_080051d8(s32, s32);
void Func_080b5028(s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e89ec renderers[2]);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e3944(s32, s32 *);
void Func_080f9010(s32);

static s32 HalfSigned_080e89ec(s32 value) {
    u32 bits = value;
    return (s32)(bits + (bits >> 31)) >> 1;
}

/* Run the darkened trail, actor sweep, and two projected particle banks. */
void Func_080e89ec(struct SceneContext_080e89ec *context) {
    struct SceneWork_080e89ec *work = Runtime_080e89ec->work;
    s32 render_target = Runtime_080e89ec->render_target;
    u8 *sprite_sheet = Runtime_080e89ec->sprite_sheet;
    Renderer_080e89ec renderers[2];
    s32 trail_positions[20][2];
    s32 projected[3];
    u8 *motion_stream;
    s32 motion_x = 0;
    s32 motion_y = 0;
    s32 vertical_speed = 0;
    s32 frame;
    s32 index;

    work->context = context;
    Func_080cd594(0);
    REG_BLDALPHA_080E89EC = 0x1010;
    Func_080e0524((void *)0xc2, work, 1, 1);

    /* Build nineteen progressively darker 0x3a8-byte trail frames. */
    for (index = 1; index < 0x14; index++) {
        s32 byte;
        u8 *source = (u8 *)work + 0x1680;
        s8 *destination = (s8 *)work + index * 0x3a8 - 0xe10;

        for (byte = 0; byte < 0x3a8; byte++) {
            u8 sample = source[byte];

            if (index > 0x0a) {
                s32 darkened = sample - index * 4 + 0x28;
                if (darkened < 0) darkened = 0;
                destination[byte] = darkened;
            }
        }
    }

    Func_080e0524((void *)0x73, sprite_sheet, 0, 0);
    Func_080e0524((void *)0xb4, &work->trail_graphics[0], 1, 1);
    Func_080e0524((void *)0x7d, (void *)0x02010000, 1, 0);
    Func_080cef64(context->side, renderers);
    work->scene_phase = 2;
    work->scene_timer = 0x4b;
    Func_080041d8(0x080cd261, 0x480);

    for (index = 8; index < 0x30; index++) {
        struct Particle_080e89ec *particle = &work->particles[index];

        particle->x = context->side == 0 ? -0x380000 : 0x380000;
        particle->y = 0;
        particle->z = 0;
        particle->velocity_x = ((Func_08004458() & 0x3f) - 0x20) << 14;
        particle->velocity_y = (Func_08004458() & 0x3f) << 13;
        particle->velocity_z = ((Func_08004458() & 0x3f) - 0x20) << 14;
        particle->timer = 1;
    }
    for (index = 0x30; index < 0x40; index++) {
        struct Particle_080e89ec *particle = &work->particles[index];

        particle->x = context->side == 0 ? -0x380000 : 0x380000;
        particle->y = 0x140000;
        particle->z = 0;
        particle->velocity_x = ((Func_08004458() & 0x3f) - 0x20) << 14;
        particle->velocity_y = (Func_08004458() & 0x3f) << 12;
        particle->velocity_z = ((Func_08004458() & 0x3f) - 0x20) << 14;
        particle->timer = 0;
    }
    {
        s32 angle = -0x4000;
        for (index = 0; index < 8; index++, angle += 0x1000) {
            struct Particle_080e89ec *particle = &work->particles[index];

            if (context->side == 1) {
                particle->x = (Func_08002322(angle) * 0x18 >> 16) + 0x58;
            } else {
                particle->x = (-(Func_08002322(angle) * 0x18) >> 16) + 0x10;
            }
            particle->y = (Func_0800231c(angle) * 0x10 >> 16) + 0x28;
            particle->timer = -index * 2;
        }
    }

    motion_stream = Func_08002f40(0xd3);
    for (frame = 0; frame < 0x96; frame++) {
        struct Camera_080e89ec *camera = CAMERA_080E89EC;
        u32 motion_window = frame - 0x12;
        u32 reset_window = frame - 0x4e;

        if (frame == 0x53) Func_080b50e8(0x86);
        if (frame == 0 || frame == 0x32) Func_080f9010(0x88);
        if (frame <= 0x3f) {
            camera->horizontal_offset += context->side == 0 ? -0x100 : 0x100;
        }
        Func_080b5028(0, 0, 0, 0x64);

        if (frame <= 0x11) {
            s32 image = Func_080022ec(frame, 3);
            void *source = &work->trail_graphics[FrameOffsets_080e89ec[image]];
            s32 y = FrameY_080e89ec[image] + 0x3c;

            renderers[0](render_target, source, 0x30, y,
                         FrameWidths_080e89ec[image], FrameHeights_080e89ec[image]);
            renderers[1](render_target, source, 0x38, y,
                         FrameWidths_080e89ec[image], FrameHeights_080e89ec[image]);
        }

        if (motion_window <= 0x28) {
            if (frame == 0x12) {
                motion_x = ((s8)motion_stream[0] << 8) + motion_stream[1];
                motion_y = ((s8)motion_stream[2] << 8) + motion_stream[3] + 0x10;
                motion_stream += 4;
            } else {
                motion_x += (s8)motion_stream[0];
                motion_y += (s8)motion_stream[1];
                motion_stream += 2;
            }
        }
        if (reset_window <= 0x28) {
            if (frame == 0x4e) {
                motion_x = -0x38;
                motion_y = 0x30;
            } else {
                motion_y -= 0x10;
            }
        }

        for (index = 0x13; index > 0; index--) {
            if (frame > index + 0x12 && frame <= index + 0x53) {
                trail_positions[index][0] = trail_positions[index - 1][0];
                trail_positions[index][1] = trail_positions[index - 1][1];
                renderers[0](
                    render_target,
                    index > 0x0a
                        ? (u8 *)work + index * 0x3a8 - 0xe10
                        : (u8 *)work + 0x1680,
                    trail_positions[index][0],
                    trail_positions[index][1],
                    0x18,
                    0x27);
            }
        }

        Func_080049ac();
        Func_080051d8((s32)camera, (s32)((u8 *)camera + 0x0c));
        if (motion_window <= 0x41) {
            s32 head_x = context->side == 1
                ? 0x40 - HalfSigned_080e89ec(motion_x)
                : HalfSigned_080e89ec(motion_x) + 0x40;
            s32 head_y = 0x3c - motion_y;
            s32 acceleration = HalfSigned_080e89ec(
                head_y - trail_positions[0][1] - 0x18);

            if (acceleration > 2) acceleration = 2;
            if (acceleration < -2) acceleration = -2;
            vertical_speed += acceleration;
            if (vertical_speed > 8) vertical_speed = 8;
            if (vertical_speed < -8) vertical_speed = -8;
            trail_positions[0][0] = head_x - 0x0c;
            trail_positions[0][1] = head_y - 0x14;
            renderers[0](render_target,
                         (u8 *)work + ((vertical_speed / 4) + 2) * 0x480,
                         head_x - 0x12, head_y - 0x16, 0x18, 0x30);
        }

        if (frame == 0x53) {
            work->burst_phase = 8;
            Func_080d6888(context->actor_id, 7, 5, 0, 8);
            Func_080b5088(context->actor_id, 1);
        }
        if (frame > 0x53) {
            for (index = 8; index < 0x40; index++) {
                struct Particle_080e89ec *particle = &work->particles[index];

                if (particle->y >= 0) {
                    s32 depth;
                    s32 size;

                    Func_080e3944((s32)particle, projected);
                    projected[0] >>= 1;
                    depth = projected[2];
                    if (depth <= 0x9f) depth = 0xa0;
                    if (depth > 0x31f) depth = 0x31f;
                    size = 9 - (depth - 0xa0) / 0x40;
                    if (index - 8 > 0x2f) {
                        if (particle->timer <= 0x0b) {
                            renderers[0](render_target,
                                (u8 *)0x02010000 + (particle->timer / 2) * 0x800,
                                projected[0] - 0x10, projected[1] - 0x20,
                                0x20, 0x40);
                            particle->timer++;
                        }
                    } else {
                        renderers[0](render_target,
                            sprite_sheet + ParticleTiles_080e89ec[size - 1],
                            projected[0] - size / 2,
                            projected[1] - size,
                            size, size * 2);
                    }
                    particle->x += particle->velocity_x;
                    particle->y += particle->velocity_y;
                    particle->z += particle->velocity_z;
                    particle->velocity_y -= 0x2000;
                }
            }
        }

        if (frame == 0x32) {
            work->burst_phase = 0x0c;
            Func_080d6888(context->actor_id, 7, 5, 0, 8);
        }
        if (frame > 0x31) {
            for (index = 0; index < 8; index++) {
                struct Particle_080e89ec *particle = &work->particles[index];

                if ((u32)particle->timer <= 0x0b) {
                    s32 image = particle->timer / 2;
                    u8 width = FrameWidths_080e89ec[image];

                    renderers[1](render_target,
                        &work->trail_graphics[FrameOffsets_080e89ec[image]],
                        particle->x - width / 2,
                        particle->y + FrameY_080e89ec[image],
                        width,
                        FrameHeights_080e89ec[image]);
                }
                particle->timer++;
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        work->frame_ready = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}
