#include "types.h"

typedef void (*Transfer_080d6970)(void *, const void *, u32);
typedef void (*Publisher_080d6970)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080d6970 {
    u8 unknown_00[20];
    s32 actor_count;
    u8 unknown_18[12];
    s16 actor_ids[1];
};

struct Object_080d6970 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[16];
    s32 field_24;
    u8 unknown_28[32];
    s32 field_48;
};

struct ObjectContext_080d6970 {
    struct Object_080d6970 *object;
};

struct Particle_080d6970 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 age;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
s32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080072f0(void *, const void *, u32, Transfer_080d6970);
void Func_08009020(void *, s32);
void Func_08009038(void *);
void Func_080b5088(s16);
struct ObjectContext_080d6970 *Func_080b5098(s16);
s32 Func_080b50d8(struct Object_080d6970 *, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6750(struct Scene_080d6970 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080dbb24(s32, void *, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080d6970 *, s32, s32);
s32 Func_080e3944(const s32 *, s32 *);
void Func_080e6d3c(s32, s32, s32);
void Func_080e6eac(s32, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void Publish_080d6970(
    void *publisher,
    void *render_context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height)
{
    ((Publisher_080d6970)publisher)(
        render_context, source, x, y, width, height);
}

/*
 * Play the long multi-actor scene that scatters the shuffled mosaic, sends
 * two waves of particles across it, and then restores the normal renderer.
 * The scratch areas below deliberately name the three independent particle
 * banks used by the original scene rather than treating the runtime as one
 * untyped m2c buffer.
 */
void Func_080d6970(struct Scene_080d6970 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void **graphics_header = (void **)0x03001e50;
    void *tiles_a;
    void *tiles_b;
    struct Particle_080d6970 *ribbons =
        (struct Particle_080d6970 *)(runtime + 0x7080);
    struct Particle_080d6970 *rain =
        (struct Particle_080d6970 *)(runtime + 0x7400);
    u8 *shuffle = (u8 *)0x02010000;
    s32 fade_velocity = 0;
    s32 fade = 0xffc00000;
    s32 frame;
    s32 i;

    *(struct Scene_080d6970 **)(runtime + 0x7828) = scene;
    Func_080dbb24(8, runtime_header, 1);

    for (i = 0; i < 0x400; i++)
        shuffle[i] = i & 0x7f;
    for (i = 0; i < 8; i++) {
        s32 j;
        for (j = 0; j < 128; j++) {
            s32 a = i * 128 + (Func_08004458() & 0x7f);
            s32 b = i * 128 + (Func_08004458() & 0x7f);
            u8 value = shuffle[a];
            shuffle[a] = shuffle[b];
            shuffle[b] = value;
        }
    }

    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0xb2;
    *(volatile u16 *)0x04000050 = 0x177;
    Func_080e0524(0x080cd261, runtime, 1, 1);
    Func_080ed408(46, 7, 7, 3, 1);
    tiles_a = graphics_header[46];
    Func_080ed408(47, 7, 7, 7, 1);
    tiles_b = graphics_header[47];

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7240) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);
    *(void **)0x03001b04 = *(void **)0x03001e80;

    for (i = 0; i < 16; i++) {
        struct Particle_080d6970 *particle = &ribbons[i];
        particle->x = i <= 4 ? i * 20 : -90 + i * 20;
        particle->y =
            (i <= 4 ? 104 : 108) + (Func_08004458() & 7);
        particle->velocity_x = 4 + (Func_08004458() & 7);
        particle->velocity_z = 16 + (Func_08004458() & 15);
    }
    for (i = 0; i < 16; i++) {
        struct Particle_080d6970 *particle =
            (struct Particle_080d6970 *)(runtime + 0x7240) + i;
        particle->x = 32 + (Func_08004458() & 63);
        particle->y = 64 + (Func_08004458() & 31);
        particle->velocity_z = -8 - (Func_08004458() & 7);
    }
    for (i = 0; i < 16; i++) {
        rain[i].x = 0x800000;
        rain[i].y = 0x400000;
        rain[i].velocity_x =
            -(200 + (Func_08004458() & 0xff)) << 9;
        rain[i].velocity_y = 0;
        rain[i].age = 0;
    }

    Func_080d6750(*(struct Scene_080d6970 **)(runtime + 0x7828));

    for (frame = 0; frame < 366; frame++) {
        void *camera = *(void **)0x03001e80;

        if ((*(volatile u32 *)0x03001b04 & 3) != 0 &&
            frame > 190 && frame <= 0x11d) {
            Func_080072f0(
                render_context,
                (const void *)0x4000,
                3,
                (Transfer_080d6970)0x03000164);
            frame = 286;
        }
        if (frame == 224)
            *(s32 *)(runtime + 0x7780) = 0;

        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 12);

        if (frame == 31) {
            *(s32 *)(runtime + 0x77d8) = 8;
            Func_080f9010(157);
            if (scene->actor_count != 0) {
                for (i = 0; i < scene->actor_count; i++)
                    Func_080b5088(scene->actor_ids[i]);
            }
        }
        if (frame == 72)
            Func_080f9010(136);
        if (frame == 140)
            Func_080f9010(156);

        fade_velocity += 0x4000;
        fade += fade_velocity;
        if (fade > 0x400000)
            fade = 0x400000;
        Func_080e6d3c(2, 0x800000, fade);

        if (frame >= 48 && frame <= 96) {
            s32 index;
            index = Func_080022fc(
                Func_080022ec(frame - 48, 24), 3);
            Func_08009020(
                *(void **)(runtime + 0x77e4),
                *(const u8 *)(0x080ee910 + index * 2));
            Func_08009020(
                *(void **)(runtime + 0x77e8),
                *(const u8 *)(0x080ee910 + index * 2 + 1));
        }

        if (frame >= 72 && frame <= 127) {
            for (i = 0; i < 16; i++) {
                struct Particle_080d6970 *particle = &rain[i];
                if (frame >= i + 72 && particle->y <= 0x67ffff) {
                    s32 index = Func_080022fc((frame + i) / 4, 5);
                    s32 width = *(const u8 *)(0x080ee920 + index);
                    s32 height = *(const u8 *)(0x080ee925 + index);
                    Publish_080d6970(
                        tiles_a,
                        render_context,
                        runtime + *(const u16 *)(0x080ee916 + index * 2),
                        *(s16 *)((u8 *)particle + 2) - width / 2,
                        (particle->y >> 16) - height / 2,
                        width,
                        height);
                    Func_080e3908(particle, 64, 0x1000);
                }
            }
        }

        if (frame == 128) {
            for (i = 0; i < 48; i++) {
                rain[i].x = Func_08002304(Func_08004458(), 96) << 16;
                rain[i].y = (88 + (Func_08004458() & 7)) << 16;
                rain[i].velocity_x =
                    ((Func_08004458() & 0xff) - 128) << 11;
                rain[i].velocity_y =
                    -(Func_08004458() & 0xff) << 11;
                rain[i].age = -16 - (Func_08004458() & 15);
            }
            *(s32 *)0x03001e80 = 0;
        }

        if (frame >= 128 && frame <= 224) {
            s32 phase = frame - 128;
            s32 capped = phase > 80 ? 80 : phase;
            if (phase <= 80)
                *(s32 *)(runtime + 0x77d8) = 2;

            for (i = 0; i < 10; i++) {
                struct Particle_080d6970 *particle = &ribbons[i];
                if (capped > particle->age) {
                    s32 displacement =
                        particle->velocity_y * (capped - particle->age);
                    s32 wrapped = displacement;
                    s32 strip;
                    s32 k;

                    while (wrapped > 184)
                        wrapped -= 64;
                    if (wrapped <= 119) {
                        Publish_080d6970(
                            ((void **)&tiles_a)[i & 1],
                            render_context,
                            runtime + (i <= 5 ? 0 : 0x6c0),
                            particle->x,
                            particle->y - wrapped - 8,
                            24,
                            8);
                    }

                    for (k = 0; k < 3; k++) {
                        s32 y = particle->y - wrapped + k * 64;
                        s32 source_y = 0;
                        s32 height = 64;
                        if (y >= -64) {
                            if (y < 0) {
                                source_y = -y * 24;
                                height += y;
                                y = 0;
                            }
                            if (y + height > particle->y)
                                height -= y + height - particle->y;
                            Publish_080d6970(
                                ((void **)&tiles_a)[i & 1],
                                render_context,
                                runtime + (i <= 5 ? 0 : 0x6c0) +
                                    source_y + 192,
                                particle->x,
                                y,
                                24,
                                height);
                        }
                    }

                    if ((i & 1) != 0) {
                        s32 index = Func_080022fc(i, 3);
                        s32 x = (particle->y - displacement) & 127;
                        s32 width = *(const u8 *)(0x080ee930 + index);
                        strip = x - 16;
                        if (strip + width > particle->y)
                            width -= strip + width - particle->y;
                        if (width > 0)
                            Publish_080d6970(
                                tiles_b,
                                render_context,
                                runtime +
                                    *(const u16 *)(0x080ee92a + index * 2),
                                particle->x + 8,
                                strip,
                                *(const u8 *)(0x080ee930 + index),
                                width);
                    }
                }
            }
        }

        if (frame - 128 <= 95) {
            for (i = 0; i < 32; i++) {
                struct Particle_080d6970 *particle = &rain[i];
                if (particle->age >= 0) {
                    s32 index = Func_080022fc(i, 5);
                    s32 width = *(const u8 *)(0x080ee93e + index);
                    s32 height = *(const u8 *)(0x080ee943 + index);
                    Publish_080d6970(
                        tiles_a,
                        render_context,
                        runtime + *(const u16 *)(0x080ee934 + index * 2),
                        *(s16 *)((u8 *)particle + 2),
                        *(s16 *)((u8 *)particle + 6),
                        width,
                        height);
                    particle->x += particle->velocity_x;
                    particle->y += particle->velocity_y;
                    particle->velocity_y += 0x4000;
                    if ((u32)particle->y > 0x780000 && frame <= 159) {
                        particle->x =
                            Func_08002304(Func_08004458(), 96) << 16;
                        particle->y =
                            (88 + (Func_08004458() & 7)) << 16;
                        particle->velocity_x =
                            ((Func_08004458() & 0xff) - 128) << 11;
                        particle->velocity_y =
                            -(Func_08004458() & 0xff) << 11;
                    }
                }
                particle->age++;
            }
        }

        if (frame >= 224 && frame <= 247 && (frame & 3) == 0) {
            volatile u16 *palette = (volatile u16 *)0x05000000;
            for (i = 0; i < 64; i++) {
                u16 color = palette[i];
                s32 red = color & 31;
                s32 green = (color >> 5) & 31;
                s32 blue = (color >> 10) & 31;
                s32 target =
                    Func_080022ec(red + green + blue, 3);
                if (red > target) red--;
                if (red < target) red++;
                if (green > target) green--;
                if (green < target) green++;
                if (blue > target) blue--;
                if (blue < target) blue++;
                palette[i] = red | (green << 5) | (blue << 10);
            }
        }

        if (frame - 128 <= 172 && scene->actor_count != 0) {
            s32 actor;
            for (actor = 0; actor < scene->actor_count; actor++) {
                struct Object_080d6970 *object =
                    Func_080b5098(scene->actor_ids[actor])->object;
                s32 projected[3];
                s32 source[3] = {object->x, object->y, object->z};
                struct Particle_080d6970 *spark =
                    (struct Particle_080d6970 *)
                        (runtime + 0x3f46 + actor * 168);
                Func_080e3944(source, projected);
                for (i = 0; i < 6; i++, spark++) {
                    if (spark->age == 0) {
                        spark->x =
                            projected[0] + (Func_08004458() & 15) - 8;
                        spark->y =
                            projected[1] + (Func_08004458() & 15) - 40;
                    }
                    if ((u32)spark->age <= 4) {
                        s32 size = *(const u8 *)(0x080ee952 + spark->age);
                        Publish_080d6970(
                            tiles_a,
                            render_context,
                            runtime +
                                *(const u16 *)(0x080ee948 + spark->age * 2),
                            spark->x - size / 2,
                            spark->y - size / 2,
                            size,
                            size);
                    }
                    spark->age++;
                    if (frame <= 199 && spark->age == 5)
                        spark->age = -(Func_08004458() & 7);
                }
            }
        }

        if (frame > 232) {
            s32 row;
            s32 start = frame * 2 - 496;
            for (row = 0; row < 32; row++) {
                s32 column;
                for (column = 0; column < 4; column++) {
                    s32 x = start + row;
                    if ((u32)x <= 127) {
                        s8 y = *(const u8 *)
                            (0x02010000 +
                             (((x & 7) * 32 + row) * 4 + column));
                        ((u8 *)render_context)[
                            (((x / 8) * 16 + y / 8) * 8 +
                             (x & 7)) * 8 + (y & 7)] = 0;
                    }
                }
            }
            start++;
            for (row = 0; row < 32; row++) {
                s32 column;
                for (column = 0; column < 4; column++) {
                    s32 x = start + row;
                    if ((u32)x <= 127) {
                        s8 y = *(const u8 *)
                            (0x02010000 +
                             (((x & 7) * 32 + row) * 4 + column));
                        ((u8 *)render_context)[
                            (((x / 8) * 16 + y / 8) * 8 +
                             (x & 7)) * 8 + (y & 7)] = 0;
                    }
                }
            }
        }

        if (frame >= 161 && frame <= 223 && scene->actor_count != 0) {
            for (i = 0; i < scene->actor_count; i++) {
                if (frame > i * 8 + 160) {
                    struct Object_080d6970 *object =
                        Func_080b5098(scene->actor_ids[i])->object;
                    object->y += 0x80000;
                    if (object->y > 0x800000)
                        object->y = 0x800000;
                    object->field_48 = 0;
                    {
                        s32 layer = 0;
                        while (Func_080b50d8(object, layer) != 0) {
                            Func_08009020(object, 5);
                            layer++;
                        }
                    }
                }
            }
        }

        for (i = 0; i < scene->actor_count; i++) {
            if (frame == 286 + i * 5) {
                struct Object_080d6970 *object =
                    Func_080b5098(scene->actor_ids[i])->object;
                object->y = 0x600000;
                object->field_48 = (s32)0x080dab85;
            }
            if (frame == 302 + i * 5) {
                Func_080d6888(scene->actor_ids[i], 7, -1, i, 8);
                Func_080f9010(134);
                *(s32 *)(runtime + 0x77a8) = 8;
            }
        }

        if (frame == 302) {
            s32 actor;
            Func_08002dd8(47);
            Func_08002dd8(46);
            Func_080e0524(0x98, runtime, 1, 0);
            Func_080e0524(0xc0, runtime + 0x1680, 1, 1);
            Func_080ed408(46, 7, 7, 3, 2);
            tiles_a = graphics_header[46];
            Func_080ed408(47, 7, 7, 7, 2);
            tiles_b = graphics_header[47];
            *(volatile u16 *)0x04000050 = 0x3f46;
            *(volatile u16 *)0x04000020 = 0x80;
            *(volatile u32 *)0x04000028 = 0;
            *(s32 *)(runtime + 0x7780) = 2;
            *(s32 *)(runtime + 0x7784) = 75;
            Func_080041d8((const void *)0x080cd261, 0x480);

            for (actor = 0; actor < scene->actor_count; actor++) {
                struct Object_080d6970 *object =
                    Func_080b5098(scene->actor_ids[actor])->object;
                struct Particle_080d6970 *particle =
                    ribbons + actor * 10;
                for (i = 0; i < 10; i++, particle++) {
                    s32 angle = i * 0x3334;
                    particle->x = object->x;
                    particle->y = 0x140000;
                    particle->z = object->z;
                    particle->velocity_x = Func_08002322(angle) << 2;
                    particle->velocity_y =
                        0x10000 + (Func_08004458() & 0x7fff);
                    particle->velocity_z = Func_0800231c(angle) << 2;
                    particle->age = 0;
                }
            }
        }

        if (frame > 301 && scene->actor_count != 0) {
            s32 actor;
            for (actor = 0; actor < scene->actor_count; actor++) {
                s32 actor_start = 302 + actor * 4;
                s32 delay = actor * 4;
                s32 projected[3];
                s32 source[3];

                if (frame >= actor_start &&
                    frame < delay + 314) {
                    struct Object_080d6970 *object =
                        Func_080b5098(scene->actor_ids[actor])->object;
                    s32 scale = (frame - 302 - delay) / 2;
                    source[0] = object->x;
                    source[1] = 0;
                    source[2] = object->z;
                    Func_080e3944(source, projected);
                    projected[0] /= 2;
                    scale = scale * 15 * 32;
                    Publish_080d6970(
                        tiles_a,
                        render_context,
                        runtime + scale,
                        projected[0] - 20,
                        projected[1] - 24,
                        20,
                        24);
                    Publish_080d6970(
                        tiles_b,
                        render_context,
                        runtime + scale,
                        projected[0],
                        projected[1] - 24,
                        20,
                        24);
                }

                if (frame >= actor_start + 6) {
                    struct Particle_080d6970 *particle =
                        ribbons + actor * 10;
                    for (i = 0; i < 5; i++, particle++) {
                        s32 size;
                        Func_080e3944(
                            (const s32 *)particle,
                            projected);
                        projected[0] /= 2;
                        if ((u32)particle->age <= 26) {
                            size = *(const u16 *)
                                (0x080ee966 + 12);
                            Publish_080d6970(
                                tiles_a,
                                render_context,
                                runtime +
                                    *(const u16 *)(0x080ee958 + 12),
                                projected[0] - size / 2,
                                projected[1] - size / 2,
                                size,
                                size);
                        }
                        Func_080e3908(particle, 60, 0x1000);
                        particle->age++;
                    }
                }
            }
        }

        if (frame <= 127)
            Func_080e155c(4, 16);
        else if (frame <= 301)
            Func_080e155c(2, 2);
        else
            Func_080e155c(4, 8);

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080b50e8(134);
    Func_080e6eac(2, 0x800000, fade);
    for (i = 0; i < 8; i++)
        Func_08009038(*(void **)(runtime + 0x77d8 + i * 4));
    Func_080cdbc0();
}
