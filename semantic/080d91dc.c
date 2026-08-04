#include "types.h"

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Nine `bl`
 * sites land in the 0x080072e4 bank: six __call_via_r4 (0x080072f4), two
 * __call_via_r8 (0x08007304) and one __call_via_sl (0x0800730c). The two
 * __call_via_r3 sites (0x080d92e8, 0x080d9360) were already resolved to
 * 0x03001388 by this file's `transfer`, and 0x080d96e6 to 0x030001d8 by its
 * `square_root`; only the remaining nine are new here.
 *
 * All nine callees are Func_080048b0 allocator slots 46 (0x03001f08) and 47
 * (0x03001f0c), and EVERY read of them is bracketed: Func_080ed408(id, ...)
 * publishes, the slot is read, the renderer is called, Func_08002dd8(id)
 * releases. The brackets, from the ROM:
 *
 *   0x080d94e2 pub 46, 0x080d94ea read 46, 0x080d94fc pub 47, 0x080d9502
 *     read 47, 0x080d952c call 46, 0x080d9544 call 47, releases 0x080d954a
 *     and 0x080d9550.
 *   0x080d960a / 0x080d9620 pub, 0x080d9628 read 47 into [sp, #60], the
 *     32-iteration particle loop calls it at 0x080d9756, releases 0x080d9792
 *     and 0x080d9798.
 *   0x080d97f0 pub 46 / 0x080d9808 read / 0x080d981e call / 0x080d9824 rel;
 *     and the same shape at 0x080d9862-0x080d988c, 0x080d98d0-0x080d98f8,
 *     0x080d9a00-0x080d9a2c-0x080d9a32.
 *   0x080d9936 pub 46, 0x080d993e read 46, 0x080d9952 pub 47, 0x080d9960
 *     read 47, 0x080d997e call 46, 0x080d9994 call 47, releases 0x080d999a
 *     and 0x080d99a0.
 *
 * So NOTHING here may be hoisted to the top of the function. Each read is
 * reproduced inside its own publish/release bracket, where the ROM performs
 * it -- the slot is republished with different parameters between brackets,
 * and a cached pointer would serve a stale renderer configuration.
 *
 * STRUCTURE CORRECTED, TWICE, AND IT IS THE SAME ERROR BOTH TIMES. The draft
 * read the __call_via_r8 callee as `header[8]`, which is 0x03001eec + 32 =
 * 0x03001f0c -- slot 47. The ROM reads slot 46 at both sites: 0x080d94e6
 * loads [sp, #36] = pool 0x080d968c = 0x03001e80 and adds 0x88, and
 * 0x03001e80 + 0x88 is 0x03001f08; 0x080d993a takes 0x03001f08 straight from
 * pool 0x080d99c0. The draft was one slot off, in the direction that made the
 * pair look like one renderer used twice. That is a THIRD base for this word
 * -- 0x03001e50 + 184, 0x03001eec + 28 and now 0x03001e80 + 0x88 -- which is
 * why the resolver reports it as a struct field rather than a global.
 *
 * ARITY: six at all nine sites. At the r8 and sl sites the callee is above
 * r0-r3, so the draft's seventh `renderer` argument to Func_08007304 was the
 * callee in an argument slot and is gone.
 *
 * UNCERTAINTY, left standing: this settles which pointer is called and when
 * it is read. It settles nothing about what slots 46 and 47 CONTAIN.
 */

typedef void (*Transfer_080d91dc)(void *, const void *, u32);
typedef s32 (*SquareRoot_080d91dc)(s32);
typedef void (*Renderer_080d91dc)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080d91dc {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    u8 unknown_18[4];
    s32 layout;
    u8 unknown_20[4];
    s16 object_ids[1];
};

struct Object_080d91dc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080d91dc {
    struct Object_080d91dc *object;
};

struct Particle_080d91dc {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, void *);
s32 Func_080b5070(s16);
struct ObjectContext_080d91dc *Func_080b5098(s16);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(
    void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 HalfTowardZero_080d91dc(s32 value)
{
    return (value + ((u32)value >> 31)) >> 1;
}

void Func_080d91dc(
    struct Scene_080d91dc *scene, s32 mode)
{
    void **header = (void **)0x03001eec;
    u8 *runtime = header[0];
    void *render_context = header[1];
    Transfer_080d91dc transfer =
        (Transfer_080d91dc)0x03001388;
    SquareRoot_080d91dc square_root =
        (SquareRoot_080d91dc)0x030001d8;
    struct Particle_080d91dc *particles =
        (struct Particle_080d91dc *)0x02010000;
    void *effect_context;
    s32 layout_x = 0;
    s32 layout_y = 0;
    s32 camera_offset;
    s32 renderer_mode;
    s32 frame;
    s32 i;

    *(struct Scene_080d91dc **)(runtime + 0x7828) =
        scene;
    Func_080cd594(0);

    if (scene->layout == 1) {
        s32 selector;

        if (mode == 3)
            selector = 0;
        else if (mode == 2 || mode == 4)
            selector = 3;
        else
            selector = 2;
        Func_080de2f8(
            scene, selector, scene->direction ^ 1, 1,
            &layout_x, &layout_y);
        layout_x = Func_080022ec(layout_x * 4, 5);
    }

    *(volatile u16 *)0x04000020 = 0x00cc;
    Func_080e0524(0x76, runtime, 0, 0);
    Func_080e0524(0xb7, runtime + 0x60e, 1, 1);

    if (mode == 3 || mode == 5) {
        Func_080e0524(0xb0, runtime + 0x2b8e, 1, 1);
        transfer(
            (void *)0x05000000,
            Func_08002f40(mode == 3 ? 0x93 : 0x8d),
            0x80);
    } else {
        s32 palette;

        if (mode == 4)
            Func_080e0524(
                0xa5, runtime + 0x2b8e, 1, 1);
        else if (mode == 0)
            Func_080e0524(
                0x9c, runtime + 0x2b8e, 1, 0);
        else
            Func_080e0524(
                0x9b, runtime + 0x2b8e, 1, 0);

        if (mode == 0 || mode == 1)
            palette = 0x8d;
        else if (mode == 2 || mode == 4)
            palette = 0x8f;
        else
            palette = 0xbb;
        transfer(
            (void *)0x05000000,
            Func_08002f40(palette),
            0x80);
    }

    Func_080e0524(
        mode == 3 ? 0x93 :
        (mode == 2 || mode == 4) ? 0x8f : 0x8d,
        runtime + 0x65c0, 1, 0);

    for (i = 0; i < 512; i++) {
        particles[i].x =
            (Func_08002304(Func_08004458(), 200) - 100)
            << 14;
        particles[i].y =
            (Func_08002304(Func_08004458(), 200) - 100)
            << 15;
        particles[i].z =
            (Func_08002304(Func_08004458(), 200) - 100)
            << 14;
        particles[i].age = 0;
    }

    if (scene->object_count == 1) {
        s32 position[3];

        Func_080e396c(scene->object_ids[0], position);
        camera_offset =
            Func_080022ec(-position[0] * 4, 5) + 64;
    } else if (scene->direction == 1) {
        camera_offset = -64;
    } else {
        camera_offset = 0;
    }
    *(volatile s32 *)0x04000028 = camera_offset << 8;

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    renderer_mode = scene->direction == 1 ? 7 : 3;
    Func_080f9010(0x8e);

    for (frame = 0;
         frame < scene->object_count * 8 + 108;
         frame++) {
        effect_context = *(void **)0x03001e80;

        if (frame == 80)
            Func_080b50e8(0);

        if (scene->layout == 1) {
            s32 angle = frame << 11;
            s32 x =
                layout_x + camera_offset +
                ((Func_08002322(angle) * 20) >> 16) -
                20;
            s32 y =
                layout_y +
                ((Func_0800231c(angle) * 4) >> 16) -
                24;
            Renderer_080d91dc renderer;
            Renderer_080d91dc renderer_47;

            if (frame > 32)
                y += 64 - frame * 2;
            Func_080ed408(
                46, 7, 7, renderer_mode ^ 4, 2);
            renderer = *(Renderer_080d91dc *)0x03001f08;
            Func_080ed408(
                47, 7, 7, renderer_mode ^ 4, 3);
            renderer_47 = *(Renderer_080d91dc *)0x03001f0c;
            renderer(
                render_context, runtime + 0x65c0,
                x, y, 40, 40);
            if (frame <= 3)
                renderer_47(
                    render_context, runtime + 0x65c0,
                    x, y, 40, 40);
            Func_08002dd8(47);
            Func_08002dd8(46);
        }

        for (i = 0; i < scene->object_count; i++) {
            s32 start = i * 8;
            s16 object_id = scene->object_ids[i];
            struct Object_080d91dc *object =
                Func_080b5098(object_id)->object;
            s32 position[3];
            s32 projected[3];
            s32 particle_index;
            Renderer_080d91dc particle_renderer;

            if (frame == start + 80)
                Func_080f9010(0xd4);

            Func_080049ac();
            Func_080051d8(
                effect_context,
                (u8 *)effect_context + 12);
            position[0] = object->x;
            position[1] =
                Func_080022ec(
                    Func_080b5070(object_id) * 2, 3);
            position[2] = object->z;
            Func_08004cb4(position);

            if (frame == start + 48)
                Func_080d6888(
                    object_id, 7, -1, i, 16);

            if (frame > start) {
                if (mode == 0)
                    Func_08004bd4(-(frame << 10));
                else if (mode == 2)
                    Func_08004c1c(frame << 10);
                else if (mode != 1) {
                    Func_08004c1c(frame << 10);
                    Func_08004c6c(frame << 10);
                }

                Func_080ed408(
                    46, 7, 7, renderer_mode, 2);
                Func_080ed408(
                    47, 7, 7, renderer_mode, 3);
                particle_renderer =
                    *(Renderer_080d91dc *)0x03001f0c;

                for (particle_index = 0;
                     particle_index < 32;
                     particle_index++) {
                    struct Particle_080d91dc *particle =
                        &particles[i * 64 + particle_index];
                    s32 magnitude;
                    s32 depth;
                    s32 radius;

                    if (frame <= start + particle_index)
                        continue;

                    magnitude = square_root(
                        (particle->x >> 8) *
                            (particle->x >> 8) +
                        (particle->y >> 8) *
                            (particle->y >> 8) +
                        (particle->z >> 8) *
                            (particle->z >> 8));
                    magnitude >>= 9;
                    if (magnitude == 0)
                        continue;

                    Func_080e3944(
                        &particle->x, projected);
                    projected[0] =
                        Func_080022ec(
                            projected[0] * 4, 5) +
                        camera_offset;
                    depth = projected[2];
                    if (depth <= 313)
                        depth = 314;
                    if (depth > 634)
                        depth = 634;
                    projected[2] = depth;
                    radius =
                        6 - ((depth - 314) / 64);
                    particle_renderer(
                        render_context,
                        runtime +
                            ((const u16 *)0x080ede5c)
                                [radius - 1],
                        projected[0] - radius,
                        projected[1] - radius,
                        radius * 2, radius * 2);

                    particle->x -=
                        Func_080022ec(
                            particle->x, magnitude);
                    particle->y -=
                        Func_080022ec(
                            particle->y, magnitude);
                    particle->z -=
                        Func_080022ec(
                            particle->z, magnitude);
                }
                Func_08002dd8(47);
                Func_08002dd8(46);
            }

            position[0] = 0;
            position[1] = 0;
            position[2] = 0;
            Func_080e3944(position, projected);
            projected[0] =
                Func_080022ec(projected[0] * 4, 5) +
                camera_offset;

            if (frame >= start + 52 &&
                frame < start + 76) {
                s32 image =
                    Func_080022fc((frame - start - 52) / 4, 6);

                Func_080ed408(
                    46, 7, 7, renderer_mode, 2);
                (*(Renderer_080d91dc *)0x03001f08)(
                    render_context,
                    runtime + 0x60e + image * 0x640,
                    projected[0] - 20,
                    projected[1] - 20,
                    40, 40);
                Func_08002dd8(46);
            }

            if (mode == 0) {
                if (frame >= start + 80 &&
                    frame < start + 108) {
                    s32 image =
                        Func_080022fc(
                            (frame - start - 80) / 4, 7);

                    Func_080ed408(
                        46, 7, 7, renderer_mode, 2);
                    (*(Renderer_080d91dc *)0x03001f08)(
                        render_context,
                        runtime + 0x2b8e +
                            image * 0x3c0,
                        projected[0] - 12,
                        projected[1] - 20,
                        24, 40);
                    Func_08002dd8(46);
                }
            } else if (mode == 3 || mode == 5) {
                if (frame >= start + 80 &&
                    frame < start + 104) {
                    s32 image =
                        Func_080022fc(
                            (frame - start - 80) / 4, 6);

                    Func_080ed408(
                        46, 7, 7, renderer_mode, 2);
                    (*(Renderer_080d91dc *)0x03001f08)(
                        render_context,
                        runtime + 0x2b8e +
                            image * 0x800,
                        projected[0] - 16,
                        projected[1] - 32,
                        32, 64);
                    Func_08002dd8(46);
                }
            } else if (mode == 4) {
                if (frame >= start + 80 &&
                    frame < start + 104) {
                    s32 image =
                        Func_080022fc(
                            HalfTowardZero_080d91dc(
                                frame - start - 80),
                            6);
                    const void *source =
                        runtime + 0x2b8e +
                        image * 0x800;
                    Renderer_080d91dc renderer;
                    Renderer_080d91dc renderer_47;

                    Func_080ed408(
                        46, 7, 7, renderer_mode, 2);
                    renderer =
                        *(Renderer_080d91dc *)0x03001f08;
                    Func_080ed408(
                        47, 7, 7,
                        renderer_mode | 8, 2);
                    renderer_47 =
                        *(Renderer_080d91dc *)0x03001f0c;
                    renderer(
                        render_context, source,
                        projected[0] - 32,
                        projected[1] - 24,
                        64, 32);
                    renderer_47(
                        render_context, source,
                        projected[0] - 32,
                        projected[1] + 8,
                        64, 32);
                    Func_08002dd8(47);
                    Func_08002dd8(46);
                }
            } else if (frame >= start + 80 &&
                       frame < start + 104) {
                s32 image =
                    Func_080022fc(
                        (frame - start - 80) / 4, 6);

                Func_080ed408(
                    46, 7, 7, renderer_mode, 3);
                (*(Renderer_080d91dc *)0x03001f08)(
                    render_context,
                    runtime + 0x2b8e +
                        image * 0x640,
                    projected[0] - 20,
                    projected[1] - 20,
                    40, 40);
                Func_08002dd8(46);
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
