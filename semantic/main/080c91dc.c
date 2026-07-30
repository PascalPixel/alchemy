typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*Renderer_080c91dc)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080c91dc {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    s32 variant;
    u8 unknown_1c[8];
    s16 object_ids[1];
};

struct Effect_080c91dc {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 phase;
};

struct RuntimeHeader_080c91dc {
    u8 *runtime;
    void *render_context;
    u8 *graphics;
};

u32 Func_080022ec(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080b50e8(s32);
void Func_080c9048(void);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static s32 PixelToTile_080c91dc(s32 value)
{
    if (value < 0)
        value += 7;
    return value >> 3;
}

void Func_080c91dc(struct Scene_080c91dc *scene)
{
    struct RuntimeHeader_080c91dc *header =
        (struct RuntimeHeader_080c91dc *)0x03001eec;
    u8 *runtime = header->runtime;
    struct Effect_080c91dc *effects =
        (struct Effect_080c91dc *)(runtime + 0x7080);
    struct Effect_080c91dc *sparks =
        (struct Effect_080c91dc *)(runtime + 0x7400);
    Renderer_080c91dc renderers[2];
    u8 *timing = (u8 *)0x080eded6;
    u16 *spark_sources = (u16 *)0x080ede84;
    u8 *spark_sizes = (u8 *)0x080ede96;
    u16 *scanline_table = (u16 *)0x02010000;
    s32 y = -0x80;
    s32 offset = -16;
    s32 i;
    s32 frame;
    s32 count;
    s32 duration;

    *(struct Scene_080c91dc **)(runtime + 0x7828) = scene;
    Func_080cd594(0x2001);
    *(volatile u16 *)0x04000020 = 0x100;
    Func_080e0524(0xb3, runtime, 1, 1);
    Func_080e0524(0xba, header->graphics, 0, 0);
    Func_080c9048();
    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000048 = 0x3337;

    for (i = 0; i < 32; i++) {
        s32 random = (Func_08004458() & 0x3f)
            + (Func_08004458() & 7) + 24;
        s32 x;

        if (scene->direction == 1)
            x = random + offset + 24;
        else
            x = random - offset + 80;
        effects[i].x = x * 8;
        effects[i].y = y;
        effects[i].phase = -1;
        y -= 64;
        offset -= 8;
    }
    for (i = 0; i < 32; i++)
        sparks[i].phase = -1;

    if (scene->direction == 0) {
        Func_080ed408(46, 7, 7, 2, 2);
        Func_080ed408(47, 7, 7, 2, 3);
    } else {
        Func_080ed408(46, 7, 7, 6, 2);
        Func_080ed408(47, 7, 7, 6, 3);
    }
    renderers[0] = *(Renderer_080c91dc *)0x03001f08;
    renderers[1] = *(Renderer_080c91dc *)0x03001f0c;

    if (scene->direction == 0) {
        s32 high = 0x7000;

        for (i = 0; i < 160; i++, high -= 0x100) {
            if ((u32)(i - 8) <= 95)
                scanline_table[i] = (0xf0 - i) | high;
            else if (i <= 135)
                scanline_table[i] = 0x888;
            else
                scanline_table[i] = 0x100;
        }
    } else {
        s32 high = 0x1800;

        for (i = 0; i < 160; i++, high += 0x100) {
            if ((u32)(i - 8) <= 87)
                scanline_table[i] = (i - 8 + 0xa0) | high;
            else if (i <= 135)
                scanline_table[i] = 0x78f8;
            else
                scanline_table[i] = 0x100;
        }
    }

    Func_080041d8((const void *)0x080c91a5, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = scene->variant == 1 ? 75 : 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    count = timing[scene->variant * 2];
    duration = timing[scene->variant * 2 + 1];
    for (frame = 0; frame < duration; frame++) {
        Renderer_080c91dc renderer =
            renderers[scene->variant == 2 ? 1 : 0];

        if (frame == duration - 16)
            Func_080b50e8(0x84);

        for (i = 0; i < count; i++) {
            struct Effect_080c91dc *effect = &effects[i];

            if (effect->phase == -1) {
                renderer(
                    header->render_context,
                    runtime,
                    PixelToTile_080c91dc(effect->x),
                    PixelToTile_080c91dc(effect->y),
                    32,
                    32);
                if (effect->y <= 0x27f) {
                    effect->x += scene->direction == 0 ? -64 : 64;
                    effect->y += 64;
                } else {
                    s32 object;

                    if ((i & 3) == 0)
                        Func_080f9010(0x73);
                    *(s32 *)(runtime + 0x77a8) = 2;
                    effect->phase = 0;
                    for (object = 0; object < scene->object_count; object++)
                        Func_080d6888(
                            scene->object_ids[object], 9, 5, object, 8);
                }
            }

            if (effect->phase != -1) {
                s32 x = PixelToTile_080c91dc(effect->x);
                s32 tile_y = PixelToTile_080c91dc(effect->y);

                if ((u32)(effect->phase - 1) <= 13) {
                    const void *source = runtime + 0x400
                        + (Func_080022ec(effect->phase, 3) << 10);
                    renderer(
                        header->render_context,
                        source,
                        x,
                        tile_y,
                        32,
                        32);
                }
                if ((u32)(effect->phase - 9) <= 2) {
                    s32 spark;

                    for (spark = 0; spark < 32; spark++) {
                        if (sparks[spark].phase == -1) {
                            sparks[spark].phase = 18;
                            sparks[spark].x =
                                ((Func_08004458() & 31) + x) * 8 + 8;
                            sparks[spark].y =
                                ((Func_08004458() & 15) + tile_y - 15) * 8;
                            break;
                        }
                    }
                }
                if (effect->phase <= 14)
                    effect->phase++;
            }
        }

        for (i = 0; i < 32; i++) {
            struct Effect_080c91dc *spark = &sparks[i];

            if (spark->phase != -1) {
                if (spark->phase <= 17) {
                    s32 stage = spark->phase / 2;
                    s32 size = spark_sizes[stage];
                    s32 half_size = size >> 1;

                    renderer(
                        header->render_context,
                        header->graphics + spark_sources[stage],
                        PixelToTile_080c91dc(spark->x) - half_size,
                        PixelToTile_080c91dc(spark->y) - half_size,
                        size,
                        size);
                }
                if (spark->phase > -1)
                    spark->phase--;
            }
        }

        Func_080cd52c();
        Func_080e155c(4, 4);
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080c91a5);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
    Func_080c9048();
}
