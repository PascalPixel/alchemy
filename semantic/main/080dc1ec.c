#include "types.h"

struct Particle_080dc1ec {
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 timer;
};

struct Position_080dc1ec {
    s32 x;
    s32 y;
    s32 z;
};

struct Runtime_080dc1ec {
    u8 pad_0000[0x7780];
    s32 frame_ready;
    s32 phase;
    u8 pad_7788[0x9c];
    s32 complete;
    void *argument;
};

struct Globals_080dc1ec {
    struct Runtime_080dc1ec *runtime;
    s32 render_context;
};

extern void *Data_03001e80;
extern const u16 Data_080ede48[];

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080cef64(s32, void *);
s32 Func_08004458(void);
void Func_080041d8(s32, s32);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_08004cb4(void *);
void Func_080049e8(void);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_08004c6c(s32);
void Func_080e3944(struct Particle_080dc1ec *, struct Position_080dc1ec *);
void Func_08004a5c(void);
void Func_080072f4(s32, const void *, s32, s32, s32, s32);
void Func_080e38b8(struct Particle_080dc1ec *, s32, s32);
void Func_080030f8(s32);
void Func_08004278(s32);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080dc1ec(void *argument)
{
    struct Globals_080dc1ec *globals =
        (struct Globals_080dc1ec *)0x03001eec;
    struct Runtime_080dc1ec *runtime = globals->runtime;
    struct Particle_080dc1ec *particles =
        (struct Particle_080dc1ec *)0x02010000;
    s32 render_context = globals->render_context;
    u32 state[2];
    struct Position_080dc1ec camera;
    s32 frame;
    s32 index;

    runtime->argument = argument;
    Func_080cd594(0);
    Func_080e0524(0x8c, runtime, 1, 1);
    Func_080cef64(((s32 *)argument)[1] ^ 1, state);

    for (index = 0; index < 256; index++) {
        particles[index].x = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].y = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].z = ((Func_08004458() & 0xff) - 127) << 16;
        particles[index].dx = 0;
        particles[index].dy = 0;
        particles[index].dz = 0;
        particles[index].timer = 0;
    }

    runtime->frame_ready = 1;
    runtime->phase = 0;
    Func_080041d8(0x080cd261, 0x480);

    camera.x = 0;
    camera.y = 0x500000;
    camera.z = 0;

    for (frame = 0; frame < 160; frame++) {
        void *object = Data_03001e80;
        s32 positive_step = frame << 8;
        s32 negative_step = -frame << 8;

        Func_080049ac();
        Func_080051d8(object, (u8 *)object + 12);
        Func_08004cb4(&camera);

        for (index = 0; index < 64; index++) {
            struct Particle_080dc1ec *particle = &particles[index];
            s32 quarter = index < 0 ? (index + 3) >> 2 : index >> 2;

            if (frame > quarter && particle->timer == 0) {
                struct Position_080dc1ec position;
                s32 size;
                s32 doubled_size;
                s32 phase = index & 3;

                Func_080049e8();
                switch (phase) {
                case 0:
                    Func_08004c1c(positive_step);
                    break;
                case 1:
                    Func_08004bd4(negative_step);
                    break;
                case 2:
                    Func_08004c6c(negative_step);
                    break;
                case 3:
                    Func_08004bd4(negative_step);
                    Func_08004c6c(negative_step);
                    break;
                }

                Func_080e3944(particle, &position);
                position.x >>= 1;
                Func_08004a5c();
                if (position.z < 250)
                    position.z = 250;
                if (position.z > 0x27a)
                    position.z = 0x27a;

                size = 9 - ((position.z - 250 +
                    ((position.z - 250) < 0 ? 63 : 0)) >> 6);
                doubled_size = size << 1;
                Func_080072f4(
                    render_context,
                    (const u8 *)runtime + phase * 0x604 +
                        Data_080ede48[doubled_size - 2],
                    position.x - ((size + ((u32)size >> 31)) >> 1),
                    position.y - size,
                    size,
                    doubled_size);
                Func_080e38b8(particle, 60, 0);

                if (frame > quarter + 30) {
                    particle->dx += -particle->x >> 8;
                    particle->dy += -particle->y >> 8;
                    particle->dz += -particle->z >> 8;
                }
            }

            positive_step += frame << 3;
            negative_step += -frame << 3;
        }

        runtime->complete = 1;
        Func_080030f8(1);
    }

    Func_08004278(0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
