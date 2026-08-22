#include "b5_context.h"
#include "battle_types.h"
#include "types.h"

typedef void (*WordCopy)(void *destination, const void *source, s32 size);
typedef void (*DrawRectangle)(
    void *destination,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct EffectArgument {
    u8 unknown_00[8];
    s32 source_id;
    u8 unknown_0c[8];
    s32 target_count;
    u8 unknown_18[12];
    s16 target_ids[BATTLE_TARGET_CAPACITY];
};

struct Position3d {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct MovingPoint {
    s32 x;
    s32 y;
    s32 z;
    s32 step_x;
    s32 step_y;
    s32 step_z;
    s32 start_frame;
};

struct TrailPoint {
    u8 unknown_00[12];
    s32 x;
    s32 y;
    u8 unknown_14[8];
};

struct ScreenPoint {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

struct EffectRuntime {
    u8 unknown_0000[0x7080];
    struct MovingPoint points[8];
    u8 unknown_7160[0x620];
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
    u8 unknown_0c[16];
    DrawRectangle draw_rectangle;
};

extern struct RuntimeCells Data_03001eec;
extern u8 Data_00000073[];
extern u8 Data_00000079[];
extern u16 Data_080ede48[];
extern u8 Data_080ee158[];

void Func_080cd594(s32 mode);
void *Func_08002f40(s32 resource_id);
void Func_08005340(const void *source, void *destination);
void Func_080ed408(s32, s32, s32, s32, s32);
s32 Func_080041d8(void (*callback)(void), s32 interval);
void Func_080cd260(void);
s32 Func_08004458(void);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 numerator, s32 denominator);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_080d40ec(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void Func_080b50e8(s32 value);
void Func_080049ac(void);
s32 Func_080051d8(void *source, void *destination);
s32 Func_080e3944(struct MovingPoint *point, struct ScreenPoint *output);
void Func_080f9010(s32 value);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
s32 Func_08004278(void (*callback)(void));
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080d1350(struct EffectArgument *argument)
{
    struct RuntimeCells *cells;
    struct EffectRuntime *runtime;
    struct Position3d *source;
    struct Position3d *target;
    struct B5Context *context;
    struct MovingPoint *point;
    void *draw_destination;
    s32 frame;
    s32 vertex;
    s32 target_index;
    DrawRectangle draw_rectangle;
    s32 point_index;
    u8 *graphics;
    void *view;
    s32 trail_base;
    struct TrailPoint *trail;
    struct EffectArgument **argument_cell;
    void **cell_cursor;

    cells = &Data_03001eec;
    cell_cursor = (void **)cells;
    runtime = *cell_cursor++;
    draw_destination = *cell_cursor;
    graphics = cells->graphics;
    view = *(void **)((u8 *)cells - 108);
    argument_cell = &runtime->argument;
    *argument_cell = argument;

    Func_080cd594(1);
    ((WordCopy)0x03001388)(
        (void *)0x05000000,
        Func_08002f40((s32)Data_00000079),
        0x80);
    Func_08005340(Func_08002f40((s32)Data_00000073), graphics);
    Func_080ed408(46, 7, 7, 3, 2);

    runtime->display_mode = 2;
    runtime->display_value = 50;
    draw_rectangle = cells->draw_rectangle;
    Func_080041d8(Func_080cd260, 0x480);

    context = Func_080b5098((*argument_cell)->source_id);
    source = context->object;
    context = Func_080b5098((*argument_cell)->target_ids[0]);
    target = context->object;

    point_index = 0;
    point = runtime->points;
    do {
        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->step_x = Func_080022ec(
            target->x + (((Func_08004458() & 0x7f) - 0x40) << 16)
                - point->x,
            12);
        point->step_y = Func_080022ec(
            target->y - point->y + 0x140000,
            12);
        point->step_z = Func_080022ec(target->z - point->z, 12);
        point->start_frame = (Func_08004458() & 0xf) + point_index * 8;
        point_index++;
        point++;
    } while (point_index != 8);

    frame = 0;
    do {
        Func_080d40ec(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            Func_080b50e8(134);

        point_index = 0;
        trail_base = 0;
        point = runtime->points;
point_loop:
        if (frame < point->start_frame)
            goto next_point;
        {
                struct ScreenPoint screen;

                Func_080049ac();
                Func_080051d8(view, (u8 *)view + 12);
                Func_080e3944(point, &screen);
                screen.x >>= 1;

                if ((u32)(screen.x + 8) <= 135) {
                    if (screen.y <= 127) {
                        if (screen.y >= -8) {
                    vertex = 0;
                    trail = &((struct TrailPoint *)0x02010000)[point_index * 10];
                    do {
                        s32 angle;

                        angle = vertex * 0x199a
                            - ((frame - point->start_frame) << 11);
                        trail->x = screen.x
                            + ((Data_080ee158[vertex & 1]
                                * Func_08002322(angle)) / 2 >> 16);
                        trail->y = screen.y
                            - (Data_080ee158[vertex & 1]
                                * Func_0800231c(angle) >> 16);
                        vertex++;
                        trail++;
                    } while (vertex != 10);

                    vertex = 0;
                    do {
                        struct TrailPoint *current;
                        struct TrailPoint *next;
                        s32 step;

                        current = &((struct TrailPoint *)0x02010000)[
                            trail_base + vertex];
                        next = &((struct TrailPoint *)0x02010000)[
                            trail_base + Func_080022fc(vertex + 1, 10)];
                        step = 0;
                        do {
                            s32 x;
                            s32 y;

                            x = current->x;
                            x += Func_080022ec(
                                step * (next->x - x), 12);
                            y = current->y;
                            y += Func_080022ec(
                                step * (next->y - y), 12);
                            draw_rectangle(
                                draw_destination,
                                graphics + Data_080ede48[1],
                                x - 1,
                                y - 2,
                                2,
                                4);
                            step++;
                        } while (step != 12);
                        vertex++;
                    } while (vertex != 10);
                        }
                    }
                }

                if (point->y <= 0x1dffff) {
                    point->step_y = -point->step_y;
                    point->step_x /= 2;
                    point->step_z /= 2;
                    runtime->impact_mode = 4;
                    Func_080f9010(134);

                    target_index = 0;
                    if (runtime->argument->target_count != 0) {
                        do {
                            Func_080d6888(
                                runtime->argument->target_ids[target_index],
                                7,
                                5,
                                target_index,
                                8);
                            target_index++;
                        } while (target_index != runtime->argument->target_count);
                    }
                }

                point->x += point->step_x;
                point->y += point->step_y;
                point->z += point->step_z;
        }

next_point:
        trail_base += 10;
        point_index++;
        point++;
        if (point_index != 8)
            goto point_loop;

        Func_080e155c(4, 4);
        Func_080cd52c();
        runtime->frame_ready = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 128);

    Func_08004278(Func_080cd260);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
