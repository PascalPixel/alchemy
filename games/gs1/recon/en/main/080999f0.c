#include "types.h"

struct Vec3 { s32 x, y, z; };
struct EffectTarget { u8 reserved_00[8]; struct Vec3 position; };
struct EffectObject {
    u8 reserved_00[8];
    s32 x, y, z;
    s32 altitude;
    u8 reserved_18[4];
    s32 scale_x, scale_y;
    u8 reserved_20[0x30];
    u8 *child;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[0x16];
    void *callback;
};
struct Effect05State {
    s32 angle;
    s32 x, y, z;
    struct EffectTarget *target;
    s32 initialized;
    u8 reserved_18[8];
    s8 high_arc;
    u8 reserved_21[0x13];
    s8 variant;
    u8 reserved_35[0x10];
    s8 alternate;
};

extern struct Effect05State *Data_03001f30;
s32 Func_080022ec(s32, s32);
#define Math_Div Func_080022ec
void WaitFrames(s32);
s32 Func_08004458(void);
#define Random_Next Func_08004458
void Func_0800447c(s32, s32, struct Vec3 *);
void Func_08009080(struct EffectObject *, s32);
void Func_080090d0(struct EffectObject *);
s32 Func_080091a8(s32, s32, s32);
void Func_08009240(struct EffectObject *, s32);
struct EffectObject *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080f9010(s32);

static inline s32 Interpolate(s32 start, s32 end, s32 step)
{
    return start + Math_Div(step * (end - start), 10);
}

void Func_080999f0(void)
{
    struct Effect05State *state = Data_03001f30;
    struct EffectTarget *target = state->target;
    struct EffectObject *main;
    struct Vec3 spawn;
    struct Vec3 start;
    struct Vec3 end;
    s32 i;
    s32 count;

    main = Func_08096c80(0xef, 0, 0, 0);
    if (main == 0)
        return;
    Func_08097384();
    Func_080f9010(0x8a);
    if (state->initialized == 0) {
        state->x = target->position.x;
        state->z = target->position.z;
        Func_0800447c(0x100000, state->angle, (struct Vec3 *)&state->x);
        state->y = Func_080091a8(0, state->x, state->z);
    }
    start.x = target->position.x;
    start.y = target->position.y + 0x100000;
    start.z = target->position.z;
    end.x = state->x;
    end.y = state->y + (state->variant == 0 ? 0x200000 : 0x500000);
    end.z = state->z;

    for (i = 0; i < 11; i++) {
        s32 scale;
        main->x = Interpolate(start.x, end.x, i);
        main->y = Interpolate(start.y, end.y, i);
        main->z = Interpolate(start.z, end.z, i);
        scale = Math_Div(i * 0xc000, 10) + 0x4000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);

    if (state->alternate == 0) {
        count = state->high_arc ? 10 : 24;
        for (i = 0; i < count; i++) {
            struct EffectObject *particle;
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Func_0800447c(Random_Next() * 5 + 0x30000, Random_Next(), &spawn);
            if (i == count - 1) {
                WaitFrames(25);
                spawn.x = main->x;
                spawn.y = main->y;
                spawn.z = main->z;
            }
            particle = Func_08096c80(0xf0, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->altitude = spawn.y - 0x200000;
                particle->callback = (void *)0x08099921;
                particle->mode = 2;
            }
            Func_080f9010(0x84);
            WaitFrames(6);
        }
        WaitFrames(10);
    } else {
        count = state->high_arc ? 10 : 30;
        for (i = count; i != 0; i--) {
            struct EffectObject *particle;
            spawn.x = main->x;
            spawn.y = main->y;
            spawn.z = main->z;
            Func_0800447c(Random_Next() * 5 + 0x30000, Random_Next(), &spawn);
            particle = Func_08096c80(0x11c, spawn.x, spawn.y, spawn.z);
            if (particle != 0) {
                particle->callback = (void *)0x080999a9;
                particle->mode = 0;
                particle->child[9] = (particle->child[9] & ~12) | 8;
                Func_08009080(particle, 8);
                Func_08009240(particle, 7);
            }
            WaitFrames(6);
        }
        WaitFrames(70);
    }

    for (i = 0; i < 11; i++) {
        s32 scale;
        main->x = Interpolate(end.x, start.x, i);
        main->y = Interpolate(end.y, start.y, i);
        main->z = Interpolate(end.z, start.z, i);
        scale = Math_Div(i * -0xc000, 10) + 0x10000;
        main->scale_x = scale;
        main->scale_y = scale;
        WaitFrames(1);
    }
    Func_080090d0(main);
    Func_0809748c();
}
