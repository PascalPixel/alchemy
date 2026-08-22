#include "battle_types.h"
#include "types.h"

typedef void (*DrawRectangle)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct EffectArgument {
    u8 unknown_00[4];
    s32 direction;
    s32 source_id;
    u8 unknown_0c[8];
    s32 target_count;
    s32 variant;
    u8 unknown_1c[8];
    s16 target_ids[BATTLE_TARGET_CAPACITY];
};

struct Particle {
    s32 x;
    s32 y;
    u8 unknown_08[16];
    s32 frame;
};

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    struct Particle falling[32];
    struct Particle bursts[32];
    s32 display_mode;
    s32 display_value;
    u8 unknown_7788[0x20];
    s32 impact_mode;
    u8 unknown_77ac[0x78];
    s32 frame_ready;
    struct EffectArgument *argument;
};

struct RuntimeCells {
    struct EffectRuntime *runtime;
    void *draw_destination;
    u8 *graphics;
};

struct DrawRegistry {
    u8 unknown_00[184];
    DrawRectangle rectangles[2];
};

extern struct RuntimeCells Data_03001eec;
extern struct DrawRegistry Data_03001e50;
extern u16 Data_080ede84[];
extern u8 Data_080ede96[];
extern u8 Data_080eded6[];
extern char Value_000000b3;
extern char Value_000000ba;
extern char Value_0000027f;

void Func_080cd594(s32 mode);
void Func_080e0524(void *resource_id, void *destination, s32 upper, s32 palette);
void Func_080c9048(void);
s32 Func_08004458(void);
void Func_080ed408(s32, s32, s32, s32, s32);
s32 Func_080041d8(void (*callback)(void), s32 interval);
void Func_080cd260(void);
void Func_080b50e8(s32 value);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080f9010(s32 value);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080cd52c(void);
void Func_080e155c(s32, s32);
void Func_080030f8(s32 frames);
s32 Func_08004278(void (*callback)(void));
void Func_08002dd8(s32 resource_id);
void Func_080cdbc0(void);

void Func_080c91dc(struct EffectArgument *argument)
{
    struct RuntimeCells *cells;
    void **cell;
    struct EffectRuntime *runtime;
    struct Particle *particle;
    struct Particle *burst;
    DrawRectangle rectangles[2];
    DrawRectangle *draw_functions;
    void *draw_destination;
    u16 *map;
    s32 particle_index;
    s32 burst_index;
    s32 frame;
    s32 x_offset;
    s32 y;
    s32 color;
    u8 *graphics;

    cells = &Data_03001eec;
    cell = (void **)cells;
    runtime = *cell++;
    draw_destination = *cell;
    graphics = cells->graphics;
    runtime->argument = argument;

    Func_080cd594(0x2001);
    *(u16 *)0x04000020 = 0x100;
    Func_080e0524(&Value_000000b3, runtime, 1, 1);
    Func_080e0524(&Value_000000ba, graphics, 0, 0);
    Func_080c9048();
    *(u16 *)0x04000050 = 0x3f44;
    *(u16 *)0x04000048 = 0x3337;

    particle_index = 0;
    y = -128;
    x_offset = -16;
    particle = runtime->falling;
    do {
        s32 x;

        x = (Func_08004458() & 0x3f)
            + (Func_08004458() & 7)
            + 24;
        if (runtime->argument->direction == 1)
            x += x_offset + 24;
        else
            x += -x_offset + 80;
        particle->y = y;
        particle->x = x * 8;
        particle_index++;
        y -= 64;
        particle->frame = -1;
        x_offset -= 8;
        particle++;
    } while (particle_index != 32);

    particle_index = 0;
    do {
        runtime->bursts[particle_index].frame = -1;
        particle_index++;
    } while (particle_index != 32);

    if (runtime->argument->direction == 0) {
        Func_080ed408(46, 7, 7, 2, 2);
        Func_080ed408(47, 7, 7, 2, 3);
    } else {
        Func_080ed408(46, 7, 7, 6, 2);
        Func_080ed408(47, 7, 7, 6, 3);
    }

    rectangles[0] = Data_03001e50.rectangles[0];
    rectangles[1] = Data_03001e50.rectangles[1];
    draw_functions = rectangles;

    if (runtime->argument->direction == 0) {
        map = (u16 *)0x02010000;
        particle_index = 0;
        color = 0x7000;
        do {
            if ((u32)(particle_index - 8) <= 95)
                *map = (0xf0 - particle_index) | color;
            else if (particle_index <= 135)
                *map = 0x888;
            else
                *map = 0x100;
            particle_index++;
            map++;
            color -= 0x100;
        } while (particle_index != 160);
    } else {
        map = (u16 *)0x02010000;
        particle_index = 0;
        color = 0x1800;
        do {
            if ((u32)(particle_index - 8) <= 87)
                *map = (particle_index - 8 + 160) | color;
            else if (particle_index <= 135)
                *map = 0x78f8;
            else
                *map = 0x100;
            particle_index++;
            map++;
            color += 0x100;
        } while (particle_index != 160);
    }

    Func_080041d8((void (*)(void))0x080c91a5, 0x480);
    runtime->display_mode = 2;
    if (runtime->argument->variant == 1)
        runtime->display_value = 75;
    else
        runtime->display_value = 50;
    Func_080041d8(Func_080cd260, 0x480);

    frame = 0;
    if (Data_080eded6[runtime->argument->variant * 2 + 1] != 0) {
        do {
            if (frame
                == Data_080eded6[runtime->argument->variant * 2 + 1] - 16) {
                Func_080b50e8(132);
            }

            particle_index = 0;
            if (Data_080eded6[runtime->argument->variant * 2] != 0) {
                particle = runtime->falling;
                do {
                    if (particle->frame == -1) {
                        s32 draw_x;
                        s32 draw_y;
                        DrawRectangle draw;

                        draw_x = particle->x / 8;
                        draw_y = particle->y / 8;
                        draw = draw_functions[runtime->argument->variant == 2];
                        draw(
                            draw_destination,
                            runtime,
                            draw_x,
                            draw_y,
                            32,
                            32);
                        if (particle->y <= (s32)&Value_0000027f) {
                            if (runtime->argument->direction == 0)
                                particle->x -= 64;
                            else
                                particle->x += 64;
                            particle->y += 64;
                        } else {
                            if ((particle_index & 3) == 0)
                                Func_080f9010(115);
                            runtime->impact_mode = 2;
                            particle->frame = 0;

                            burst_index = 0;
                            if (runtime->argument->target_count != 0) {
                                do {
                                    Func_080d6888(
                                        runtime->argument->target_ids[burst_index],
                                        9,
                                        5,
                                        burst_index,
                                        8);
                                    burst_index++;
                                } while (burst_index
                                    != runtime->argument->target_count);
                            }
                        }
                    }

                    if (particle->frame != -1) {
                        s32 draw_x;
                        s32 draw_y;

                        draw_x = particle->x / 8;
                        draw_y = particle->y / 8;
                        if ((u32)(particle->frame - 1) <= 13) {
                            s32 draw_index;
                            const void *image;
                            DrawRectangle draw;

                            draw_index = runtime->argument->variant == 2;
                            image = (u8 *)runtime + 0x400
                                + (Func_080022ec(particle->frame, 3) << 10);
                            draw = draw_functions[draw_index];
                            draw(
                                draw_destination,
                                image,
                                draw_x,
                                draw_y,
                                32,
                                32);
                        }

                        if ((u32)(particle->frame - 9) <= 2) {
                            burst_index = 0;
                            burst = runtime->bursts;
                            do {
                                if (burst->frame == -1) {
                                    s32 burst_y;

                                    burst->frame = 18;
                                    burst->x = ((Func_08004458() & 31)
                                        + particle->x / 8) * 8 + 8;
                                    burst_y = (Func_08004458() & 15)
                                        + particle->y / 8 - 15;
                                    burst->y = burst_y * 8;
                                    break;
                                }
                                burst_index++;
                                burst++;
                            } while (burst_index != 32);
                        }

                        if (particle->frame <= 14)
                            particle->frame++;
                    }
                    particle_index++;
                    particle++;
                } while (particle_index
                    != Data_080eded6[runtime->argument->variant * 2]);
            }

            burst_index = 0;
            burst = runtime->bursts;
            do {
                if (burst->frame != -1) {
                    if (burst->frame <= 17) {
                        s32 image;
                        s32 size;
                        s32 half_size;
                        s32 draw_x;
                        s32 draw_y;
                        DrawRectangle draw;

                        image = burst->frame / 2;
                        draw_x = burst->x / 8;
                        size = Data_080ede96[image];
                        half_size = (u32)size >> 1;
                        draw_x -= half_size;
                        draw_y = burst->y / 8 - half_size;
                        draw = draw_functions[runtime->argument->variant == 2];
                        draw(
                            draw_destination,
                            graphics + Data_080ede84[image],
                            draw_x,
                            draw_y,
                            size,
                            size);
                    }
                    if (burst->frame > -1)
                        burst->frame--;
                }
                burst_index++;
                burst++;
            } while (burst_index != 32);

            Func_080cd52c();
            Func_080e155c(4, 4);
            runtime->frame_ready = 1;
            Func_080030f8(1);
            frame++;
        } while (frame
            != Data_080eded6[runtime->argument->variant * 2 + 1]);
    }

    Func_08004278(Func_080cd260);
    Func_08004278((void (*)(void))0x080c91a5);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    Func_080c9048();
}
