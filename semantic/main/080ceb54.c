typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*Transfer_080ceb54)(void *, const void *, u32);
typedef void (*Renderer_080ceb54)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080ceb54 {
    s32 unknown_00;
    s32 direction;
    s32 primary_object_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080ceb54 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectContext_080ceb54 {
    struct Object_080ceb54 *object;
};

struct Particle_080ceb54 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
s32 Func_080b5070(s32);
struct ObjectContext_080ceb54 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
s32 Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080ceb54 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e38b8(struct Particle_080ceb54 *, s32, s32);
void Func_080e3944(const s32 *, s32 *);
void Func_080f9010(s32);

s32 Func_080ceb54(struct Scene_080ceb54 *scene, s32 variant)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080ceb54 transfer = (Transfer_080ceb54)0x03001388;
    Renderer_080ceb54 renderers[2];
    struct Particle_080ceb54 *particles =
        (struct Particle_080ceb54 *)0x02010000;
    const u8 *settings = (const u8 *)(0x080ee090 + variant * 2);
    s32 palette;
    s32 frame;
    s32 total_frames;
    s32 object_index;
    s32 particle_index;

    *(struct Scene_080ceb54 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    Func_08005340(Func_08002f40(0x69), runtime);

    if (variant == 0)
        palette = 0xbb;
    else if (variant == 1)
        palette = 0x8d;
    else
        palette = 0x91;
    transfer((void *)0x05000000, Func_08002f40(palette), 0x80);

    Func_080cef64(scene->direction, renderers);

    for (particle_index = 0; particle_index < 1024; particle_index++)
        particles[particle_index].timer = -1;

    for (object_index = 0;
         object_index < scene->object_count;
         object_index++) {
        struct Object_080ceb54 *object =
            Func_080b5098(scene->primary_object_id)->object;
        s32 y = Func_080b5070(scene->primary_object_id);

        for (particle_index = 0; particle_index < 128; particle_index++) {
            struct Particle_080ceb54 *particle =
                &particles[object_index * 128 + particle_index];

            particle->x = object->x;
            particle->y = y;
            particle->z = object->z;
            particle->velocity_x =
                ((s32)(Func_08004458() & 0xff) - 128) << 10;
            particle->velocity_y =
                ((s32)(Func_08004458() & 0xff) - 128) << 10;
            particle->velocity_z =
                ((s32)(Func_08004458() & 0xff) - 128) << 10;
            particle->timer = 0;
        }
    }

    Func_080041d8((const void *)0x080dbb9d, 0x480);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080f9010(0x92);

    total_frames = settings[1] + scene->object_count * 20;
    for (frame = 0; frame < total_frames; frame++) {
        s32 *wave = (s32 *)(runtime + 0x6980);

        if (frame == 80) {
            if (variant == 0)
                Func_080b50e8(0x86);
            else
                Func_080b50e8(0x85);
        }

        Func_080049ac();
        Func_080051d8(effect_context, (u8 *)effect_context + 12);

        for (particle_index = 0; particle_index < 160; particle_index++) {
            wave[particle_index] =
                (0x100000 -
                 Func_08002322((frame + particle_index) << 10) * 16) >> 10;
        }

        for (object_index = 0;
             object_index < scene->object_count;
             object_index++) {
            s32 object_id = scene->object_ids[object_index];
            struct Object_080ceb54 *object =
                Func_080b5098(object_id)->object;
            s32 object_height = Func_080b5070(object_id) / 2;
            s32 delay = object_index * 20;

            if (frame == delay + 71) {
                if (variant == 0)
                    Func_080f9010(0x86);
                else
                    Func_080f9010(0x85);
            }

            if (frame == delay + 70) {
                Func_080d6888(object_id, 7, 5, object_index, 26);
            }

            if (frame > delay && settings[0] != 0) {
                for (particle_index = 0;
                     particle_index < settings[0];
                     particle_index++) {
                    struct Particle_080ceb54 *particle =
                        &particles[object_index * 128 + particle_index];

                    if (frame >
                            ((object_index * 10 + particle_index) * 2) &&
                        particle->timer >= 0) {
                        s32 projected[3];
                        s32 size_source =
                            Func_080022fc(particle_index, 3) * 0x280;

                        Func_080e3944(&particle->x, projected);
                        projected[0] >>= 1;
                        renderers[0](
                            render_context,
                            runtime + size_source,
                            projected[0] - 10,
                            projected[1] - 16,
                            20,
                            32);
                        Func_080e38b8(particle, 62, 0);

                        if (frame > delay + particle_index + 30) {
                            s32 delta_x =
                                (object->x - particle->x) >> 9;
                            s32 delta_y =
                                (object->y + object_height - particle->y)
                                >> 9;
                            s32 delta_z =
                                (object->z - particle->z) >> 9;

                            particle->velocity_x += delta_x;
                            particle->velocity_y += delta_y;
                            particle->velocity_z += delta_z;
                            if ((u32)(delta_x + 0xfff) <= 0x1ffe &&
                                (u32)(delta_z + 0xfff) <= 0x1ffe) {
                                particle->timer = -1;
                            }
                        }
                    }
                }
            }
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((const void *)0x080dbb9d);
    return Func_080cdbc0();
}
