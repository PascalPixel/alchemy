typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Position_0800c880 {
    s32 x;
    s32 y;
    s32 z;
};

struct Scene_0800c880 {
    struct Position_0800c880 position;
    struct Position_0800c880 target;
    struct Position_0800c880 *position_override;
    struct Position_0800c880 *target_override;
};

struct Particle_0800c880 {
    void *active;
    u16 unknown_04;
    u16 angle;
    struct Position_0800c880 position;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[2];
    u8 parameter;
    u8 unknown_23[0x2d];
    void **targets;
    u8 mode;
    u8 unknown_55[0x1b];
};

typedef void (*Transfer_0800c880)(const void *, s32, s32);

void *Func_080048b0(s32, s32);
s32 Func_080044d0(s32, s32);
void Func_080049ac(void);
s32 Func_080770c0(s32);
void Func_080051e8(struct Position_0800c880 *, struct Position_0800c880 *);
void Func_080051d8(struct Position_0800c880 *, struct Position_0800c880 *);
void Func_0800b388(void *, struct Position_0800c880 *, s32 *, s32, s32);
void Func_08002dd8(s32);

void Func_0800c880(void)
{
    struct Scene_0800c880 *scene = *(struct Scene_0800c880 **)0x03001e80;
    s16 *global_angle = *(s16 **)0x03001e68;
    struct Position_0800c880 *position;
    struct Position_0800c880 *target;
    struct Particle_0800c880 *particle;
    s32 angle;
    s32 index;
    void *allocation;

    allocation = Func_080048b0(52, 0x2c4);
    *(u32 *)0x040000d4 = 0x08009bb8;
    *(void **)0x040000d8 = allocation;
    *(u32 *)0x040000dc = 0x840000b1;

    position = scene->position_override;
    if (position == 0)
        position = &scene->position;
    target = scene->target_override;
    if (target == 0)
        target = &scene->target;

    angle = (s16)Func_080044d0(
        (position->x - target->x) >> 16,
        (position->z - target->z) >> 16);
    *global_angle = 0;
    Func_080049ac();

    if (Func_080770c0(0x16b)) {
        Transfer_0800c880 transfer =
            (Transfer_0800c880)0x030002c0;

        angle -= 0x2000;
        transfer((const void *)0x08013190, 0x10000, 8);
        Func_080051e8(position, target);
    } else {
        Func_080051d8(position, target);
    }

    particle = (struct Particle_0800c880 *)
        (*(u8 **)0x03001e64 + 0x1b90);
    for (index = 63; index >= 0; index--, particle--) {
        s32 scale[2];

        if (particle->active == 0)
            continue;

        switch (particle->mode & 0xf) {
        case 1:
            scale[0] = particle->scale_x;
            scale[1] = particle->scale_y;
            if (Func_080770c0(0x16b)) {
                scale[0] = 0x10000;
                scale[1] = 0x10000;
            }
            Func_0800b388(particle->targets, &particle->position, scale,
                          particle->angle + angle, particle->parameter);
            break;
        case 2: {
            s32 target_index;

            scale[0] = particle->scale_x;
            scale[1] = particle->scale_y;
            if (Func_080770c0(0x16b)) {
                scale[0] = 0x10000;
                scale[1] = 0x10000;
            }
            for (target_index = 0; target_index < 4; target_index++) {
                if (particle->targets[target_index] != 0) {
                    Func_0800b388(particle->targets[target_index],
                                  &particle->position, scale,
                                  particle->angle + angle,
                                  particle->parameter);
                }
            }
            break;
        }
        }
    }

    Func_08002dd8(52);
}
