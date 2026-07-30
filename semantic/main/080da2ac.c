typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*Transfer_080da2ac)(void *, const void *, u32);
typedef void (*Renderer_080da2ac)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080da2ac {
    s32 unknown_00;
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    s32 variant;
    u8 unknown_1c[8];
    s16 object_ids[1];
};

struct Object_080da2ac {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[20];
    s32 vertical_impulse;
    u8 unknown_2c[28];
    s32 effect;
};

struct ObjectContext_080da2ac {
    struct Object_080da2ac *object;
};

struct EffectPoint_080da2ac {
    s32 unknown_00[6];
    s32 active;
};

s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
struct ObjectContext_080da2ac *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080da2ac(struct Scene_080da2ac *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080da2ac transfer = (Transfer_080da2ac)0x03001388;
    Renderer_080da2ac renderer;
    struct EffectPoint_080da2ac *effect_points =
        (struct EffectPoint_080da2ac *)(runtime + 0x7080);
    const u8 *timing = (const u8 *)(0x080eea38 + scene->variant * 3);
    s32 horizontal_offset;
    s32 widths[4];
    s32 frame;
    s32 index;

    *(struct Scene_080da2ac **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Func_080e0524((void *)0x8b, runtime + 0x1e00, 1, 1);

    transfer((void *)0x02010000, (const void *)0x06008000, 0x8000);
    for (index = 0; index < 16; index++) {
        s32 x;
        s32 source_y = index + 96;

        for (x = 0; x < 40; x++) {
            s32 source_x = x + 32;
            s32 source_offset =
                (source_x & 7) +
                ((source_x >> 3) << 6) +
                ((source_y & 7) << 3) +
                ((source_y >> 3) << 11);

            runtime[index * 40 + x] =
                *(u8 *)(0x02010000 + source_offset);
        }
    }

    if (scene->direction == 1) {
        *(volatile s32 *)0x04000028 = -0x7000;
        horizontal_offset = -112;
    } else {
        horizontal_offset = 0;
    }

    Func_080ed408(46, 7, 7, 3, 1);
    renderer = *(Renderer_080da2ac *)0x03001f08;

    for (index = 0; index < scene->object_count; index++)
        effect_points[index].active = 0;

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    widths[0] = *(s32 *)0x080eda88;
    widths[1] = *(s32 *)0x080eda8c;
    widths[2] = *(s32 *)0x080eda90;
    widths[3] = *(s32 *)0x080eda94;
    *(s32 *)(runtime + 0x77a8) = 128;
    Func_080f9010(0x8d);

    for (frame = 0; frame < timing[0]; frame++) {
        s32 band;
        s32 left = 0;
        s32 x = 8;
        s32 start = 16;

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        if (frame == timing[0] - 16)
            Func_080b50e8(0x85);

        for (band = 0; band < 3; band++) {
            if ((frame & 31) + 32 == start)
                widths[band] += 32;

            if (frame >= start && frame < start + timing[1]) {
                s32 height =
                    widths[band] *
                    Func_08002322((frame - start) << 10) >> 16;
                s32 boundary;

                if (height < 0)
                    height = -height;
                boundary = 112 - height;

                renderer(
                    render_context,
                    runtime + 0x1e00,
                    x, boundary, 40, height);
                renderer(
                    render_context,
                    runtime,
                    x, 96 - height, 40, 16);

                for (index = 0; index < scene->object_count; index++) {
                    struct Object_080da2ac *object =
                        Func_080b5098(scene->object_ids[index])->object;
                    s32 source_point[3];
                    s32 projected[3];

                    source_point[0] = object->x;
                    source_point[1] = object->y;
                    source_point[2] = object->z;
                    Func_080e3944(source_point, projected);
                    projected[0] += horizontal_offset;

                    if (projected[0] >= left + 8 &&
                        projected[0] <= x + 40 &&
                        projected[1] >= boundary) {
                        object->vertical_impulse = 0xc0000;
                        object->effect = 0xab85;
                    }
                    if (object->y < 0) {
                        Func_080d6888(
                            scene->object_ids[index], 0, 5, -1, 0);
                    }
                }
            }

            left += 40;
            x += 40;
            start += 4;
        }

        for (index = 0; index < scene->object_count; index++) {
            struct Object_080da2ac *object =
                Func_080b5098(scene->object_ids[index])->object;

            if (effect_points[index].active == 0 &&
                object->y <= 0 &&
                object->vertical_impulse < 0) {
                effect_points[index].active = 1;
                Func_080d6888(
                    scene->object_ids[index], 7, 5, index, 5);
            }
        }

        Func_080e155c(timing[2], timing[2]);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
