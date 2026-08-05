#include "layout_guard.h"
#include "types.h"

struct Particle_080e823c {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

struct Point_080e823c {
    s32 x;
    s32 y;
    s32 z;
};

typedef void (*Renderer_080e823c)(
    void *, const void *, s32, s32, s32, s32);

LAYOUT_SIZE_GUARD(Particle080e823c, struct Particle_080e823c, 0x1c);
LAYOUT_SIZE_GUARD(Point080e823c, struct Point_080e823c, 0x0c);

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void *Func_08009030(s32);
void Func_08009008(void *, struct Point_080e823c *, const s32 *, s32);
void Func_08009020(void *, s32);
void Func_08009038(void *);
void Func_080b5040(s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080c9048(void);
void Func_080cd104(s32, s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6750(void *);
void Func_080d67dc(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080dbb24(s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080e823c *, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e823c(void *argument)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    Renderer_080e823c renderers[2];
    struct Particle_080e823c *primary =
        (struct Particle_080e823c *)(runtime + 0x7080);
    struct Particle_080e823c *secondary =
        (struct Particle_080e823c *)(runtime + 0x7128);
    struct Particle_080e823c *bursts =
        (struct Particle_080e823c *)(runtime + 0x73c8);
    struct Particle_080e823c *field =
        (struct Particle_080e823c *)0x02010000;
    void **projection_objects = (void **)(runtime + 0x7128);
    void **actor_objects = (void **)(runtime + 0x77d8);
    s32 transform[2];
    struct Point_080e823c projected[7];
    struct Point_080e823c point;
    s32 path_x = 0xa00000;
    s32 path_z = 0x5c0000;
    u32 frame;
    u32 index;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    Func_080c9048();
    *(volatile u16 *)0x04000050 = 0x7828;
    *(volatile u16 *)0x04000052 = 0x7828;
    *(s32 *)(runtime + 0x7780) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080cd104(1, 0);
    Func_080d6750(argument);
    Func_080dbb24(9, 0x17b, 2);

    for (index = 0; index < 6; index++) {
        void *object = Func_08009030(390);

        /*
         * This is the object's original packed owner table.  It is separate
         * from the projection-object table populated by the scene setup.
         */
        *(void **)(runtime + 0x2737 + index * 4) = object;
        if (object != 0) {
            ((u8 *)object)[38] = 0;
            Func_08009020(object, Func_080022fc(index, 3));
            ((u8 *)object)[9] =
                (((u8 *)object)[9] & (u8)-13) | 4;
        }
    }

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080e823c)header[7];
    Func_080ed408(47, 7, 7, 3, 3);
    renderers[1] = (Renderer_080e823c)header[8];

    *(volatile u16 *)0x04000048 = 0xf0;
    *(volatile u16 *)0x04000040 = 0x1088;
    *(volatile u16 *)0x04000046 = 0x77fc;
    Func_080030f8(1);
    Func_080b5040(1, 0x3c, 0);
    Func_080cd104(1, 1);
    Func_080e0524(0x73, effect_tiles, 0, 0);
    Func_080e0524(0xc0, runtime, 1, 1);

    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000050 = 0x3f44;
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;

    for (index = 0; index < 6; index++) {
        primary[index].x = (Func_08004458() & 0x7f) << 16;
        primary[index].y = -index * 0x100000;
        primary[index].velocity_x = 0;
        primary[index].velocity_y = 0;
        primary[index].timer = 0;
    }
    for (index = 6; index < 64; index++)
        primary[index].timer = 24;
    for (index = 0; index < 1024; index++)
        field[index].timer = -1;

    *(s32 *)(runtime + 0x77b4) = 24;
    *(s32 *)(runtime + 0x77b8) = 0;
    transform[0] = *(const s32 *)0x080edac8;
    transform[1] = *(const s32 *)0x080edacc;

    for (frame = 0;
         frame < 320 && (*(volatile u32 *)0x03001b04 & 3) == 0;
         frame++) {
        if (frame == 94 || frame == 136 || frame == 178)
            Func_080f9010(156);
        if (frame == 260)
            Func_080f9010(145);

        if ((u32)(frame - 96) <= 155 ||
            (u32)(frame - 260) <= 3)
            *(s32 *)(runtime + 0x77a8) = 1;

        for (index = 0; index < 7; index++) {
            projected[index].x =
                (((const u8 *)0x080eeed8)[index] << 16) +
                0xffe00000;
            projected[index].z =
                (((const u8 *)0x080eeee1)[index] << 16) +
                0xffe00000;
            Func_08009008(
                projection_objects[index], &projected[index],
                transform, 0);
        }

        if (frame <= 90) {
            s32 angle = frame << 9;

            path_x = (Func_08002322(angle) << 4) + 0x9c0000;
            path_z = (Func_0800231c(angle) << 4) + 0x5c0000;
        }

        if (frame <= 196) {
            for (index = 0; index < 3; index++) {
                s32 trigger = 91 + index * 40;
                s32 particle_index = 30 + index * 8;

                if (frame >= trigger && frame < trigger + 4)
                    path_z += 0x80000;
                if (frame == trigger + 3) {
                    u32 subindex;

                    for (subindex = 0; subindex < 4; subindex++) {
                        struct Particle_080e823c *particle =
                            &primary[particle_index + subindex];

                        particle->x = 0x400000;
                        particle->y = 0x600000;
                        particle->velocity_x =
                            ((s32)(Func_08004458() & 0xff) - 127)
                            << 10;
                        particle->velocity_y =
                            ((s32)(Func_08004458() & 0xff) - 127)
                            << 10;
                        particle->timer = Func_08004458() & 15;
                    }
                }
                if (frame >= trigger + 20 &&
                    frame < trigger + 36)
                    path_z -= 0x20000;
            }
        }

        if ((u32)(frame - 244) <= 7)
            path_x -= 0x10000;
        if ((u32)(frame - 252) <= 23)
            path_x -= (frame - 250) << 16;

        if (frame <= 259) {
            point.x = path_x;
            point.y = 0xff000000;
            point.z = path_z;
            Func_08009008(
                *(void **)(runtime + 0x77f4), &point, transform, 0);
            point.x = path_x + 0x200000;
            Func_08009008(
                *(void **)(runtime + 0x77f8), &point, transform, 0);
        }

        point.y = 0;
        for (index = 0; index < 6; index++) {
            struct Particle_080e823c *particle = &primary[index];

            if (particle->timer == 2)
                continue;
            point.x = particle->x;
            point.z = particle->y;
            Func_08009008(
                actor_objects[index], &point, transform, 0);
            particle->x += particle->velocity_x;
            particle->y += particle->velocity_y;
            if (frame > 96)
                particle->velocity_y += 0x10000;
            if (particle->y > 0x780000) {
                particle->timer++;
                if (particle->timer == 1) {
                    u32 subindex;

                    particle->velocity_y =
                        -particle->velocity_y / 2;
                    for (subindex = 0; subindex < 2; subindex++) {
                        struct Particle_080e823c *spawn =
                            &bursts[index * 2 + subindex];

                        spawn->x = particle->x / 2;
                        spawn->y = particle->y - 0x200000;
                        spawn->velocity_x =
                            ((s32)(Func_08004458() & 0xff) - 127)
                            << 10;
                        spawn->velocity_y =
                            ((s32)(Func_08004458() & 0xff) - 127)
                            << 10;
                        spawn->timer = Func_08004458() & 15;
                    }
                } else if (frame <= 199) {
                    particle->y = 0;
                    particle->velocity_y = 0;
                    particle->timer = 0;
                }
            }
        }

        for (index = 0; index < 56; index++) {
            struct Particle_080e823c *particle = &secondary[index];

            if (particle->timer >= 0) {
                s32 image;
                s32 width;

                if ((u32)particle->timer <= 23) {
                    image = Func_080022ec(particle->timer, 6) + 3;
                    width = ((const u16 *)0x080eeef8)[image];
                    renderers[0](
                        render_context,
                        runtime + ((const u16 *)0x080eeeea)[image],
                        *(s16 *)((u8 *)particle + 2) - width / 2,
                        *(s16 *)((u8 *)particle + 6) - width / 2,
                        width, width);
                }
                Func_080e3908(particle, 60, -0x4000);
                particle->timer++;
            }
        }

        if (frame == 260) {
            void *owner = *(void **)(runtime + 0x7828);
            u8 *actor_list = (u8 *)actor_objects[0];
            s32 count = *(s32 *)((u8 *)owner + 20);

            for (index = 0; index < count; index++) {
                s16 id = *(s16 *)(actor_list + 36 + index * 2);

                Func_080b5088(id, 4);
                Func_080d6888(id, 7, -1, index, 8);
            }
            *(s32 *)(runtime + 0x77a8) = 8;

            for (index = 0; index < 1024; index++) {
                s32 magnitude =
                    (Func_08004458() & 0x3ff) + 32;
                s32 angle = Func_08004458() & 0xffff;

                field[index].x = 0x200000;
                field[index].y = 0x5c0000;
                field[index].velocity_x =
                    magnitude * Func_08002322(angle) >> 7;
                field[index].velocity_y =
                    -(magnitude * Func_0800231c(angle) * 2) >> 7;
                field[index].timer =
                    (Func_08004458() & 15) + 32;
            }
        }

        for (index = 0; index < 1024; index++) {
            struct Particle_080e823c *particle = &field[index];

            if (particle->timer >= 0) {
                s32 size = (particle->timer >> 3) + 1;

                renderers[index & 1](
                    render_context,
                    effect_tiles +
                        ((const u16 *)0x080ede48)[size - 1],
                    *(s16 *)((u8 *)particle + 2) -
                        (size + (size < 0)) / 2,
                    *(s16 *)((u8 *)particle + 6) - size,
                    size, size * 2);
                Func_080e3908(particle, 62, 0x1000);
                particle->timer--;
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_080b50e8(134);
    Func_080d67dc();
    for (index = 0; index < 15; index++)
        Func_08009038(projection_objects[index]);
    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
