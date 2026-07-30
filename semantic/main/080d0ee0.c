#include "types.h"

typedef void (*Transfer_080d0ee0)(void *, const void *, u32);

struct Scene_080d0ee0 {
    u8 unknown_00[8];
    s32 focus_id;
    u8 unknown_0c[24];
    s16 secondary_id;
};

struct Object_080d0ee0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[20];
    s32 effect_40;
    u8 unknown_44[4];
    s32 effect_48;
    s32 effect_52;
    u8 unknown_56[16];
    void *callback;
};

struct ObjectContext_080d0ee0 {
    struct Object_080d0ee0 *object;
};

struct Motion_080d0ee0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 unused;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_080049e8(void);
void Func_08004a5c(void);
void Func_08004c1c(s32);
void Func_08004c6c(s32);
void Func_08004cb4(void *);
void Func_08004cf0(s32 *);
void Func_080051d8(void *, void *);
void Func_08005340(const void *, void *);
void Func_080072f0(void *, const void *, u32, Transfer_080d0ee0);
void Func_080072f4(
    const void *, const void *, s32, s32, s32, s32, s32, s32);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_08009140(struct Object_080d0ee0 *);
void Func_08009150(struct Object_080d0ee0 *, s32, s32, s32);
struct ObjectContext_080d0ee0 *Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
s32 Func_080e3944(const s32 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080d0ee0(struct Scene_080d0ee0 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    void *effect_context = *(void **)0x03001e80;
    Transfer_080d0ee0 transfer = (Transfer_080d0ee0)0x03001388;
    struct Object_080d0ee0 *focus;
    struct Object_080d0ee0 *target;
    struct Motion_080d0ee0 *motions =
        (struct Motion_080d0ee0 *)(runtime + 0x7080);
    s32 transformed[3];
    s32 frame;
    s32 phase;
    s32 i;

    *(struct Scene_080d0ee0 **)(runtime + 0x7828) = scene;
    target = Func_080b5098(scene->focus_id)->object;
    Func_080cd594(1);

    Func_080072f0(
        (void *)0x05000000, Func_08002f40(0x9d), 0x80, transfer);
    Func_08005340(Func_08002f40(0x9e), graphics);
    Func_08009080(target, 2);
    Func_08009088(target, 48);
    Func_080ed408(46, 7, 7, 3, 2);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    focus = Func_080b5098(scene->focus_id)->object;
    target = Func_080b5098(scene->secondary_id)->object;
    for (i = 0; i < 3; i++) {
        struct Motion_080d0ee0 *motion = &motions[i];

        motion->x = focus->x;
        motion->y = focus->y + 0x280000;
        motion->z = focus->z;
        motion->velocity_x =
            Func_080022ec(target->x * (i == 0 ? 1 : 2) - motion->x, 12);
        motion->velocity_y =
            Func_080022ec(target->y - motion->y + 0x280000, 12);
        motion->velocity_z =
            Func_080022ec(target->z - motion->z, 12);
        motion->unused = 0;
    }

    for (frame = 0; frame < 60; frame++) {
        volatile u16 *angle = (volatile u16 *)((u8 *)effect_context + 54);

        if (frame <= 47) {
            s32 turn = frame <= 39 ? 0x80 : 0x300 - frame * 16;

            if (*(s32 *)(runtime + 0x7824) == 0)
                *angle -= turn;
            else
                *angle += turn;
        }

        for (phase = 0; phase < 3; phase++) {
            s32 phase_frame = frame - phase * 12;
            s32 path_count;
            s32 maximum = 0;

            if (phase_frame < 0)
                continue;

            path_count = (phase_frame + (phase_frame < 0 ? 3 : 0)) / 4 + 2;
            if (path_count > 10)
                path_count = 10;

            Func_080049ac();
            Func_080051d8(effect_context, (u8 *)effect_context + 12);
            Func_08004cb4(&motions[phase]);

            for (i = 0; i < 10; i++) {
                s32 scale[3];
                s32 amount = phase_frame << 12;
                s32 stride = phase * 7 * 4;
                s32 value;

                Func_080049e8();
                Func_08004c6c(phase_frame << 10);
                Func_08004c1c(0x4000);
                scale[0] = amount + 0x1000;
                if (scale[0] > 0x10000)
                    scale[0] = 0x10000;
                scale[1] = scale[0];
                scale[2] = scale[0];
                Func_08004cf0(scale);
                Func_08004c6c(i * 0x1c72);

                value = Func_080e3944(
                    (const s32 *)(0x080ee200 + (i & 1) * 12 + stride),
                    transformed);
                if (maximum < value)
                    maximum = value;

                transformed[0] >>= 1;
                motions[i].velocity_x = transformed[0];
                motions[i].velocity_y = transformed[1];
                Func_08004a5c();
            }

            if (maximum <= 0x9fff) {
                s32 half = (path_count + (path_count < 0)) / 2;

                for (i = 0; i < 10; i++) {
                    s32 next = Func_080022fc(i + 1, 10) + phase;
                    struct Motion_080d0ee0 *from = &motions[i + phase];
                    struct Motion_080d0ee0 *to = &motions[next];
                    s32 step;

                    for (step = 1; step <= 16; step++) {
                        s32 x = from->velocity_x +
                            ((to->velocity_x - from->velocity_x) * step) / 16;
                        s32 y = from->velocity_y +
                            ((to->velocity_y - from->velocity_y) * step) / 16;
                        u16 tile = *(const u16 *)(0x080ee280 +
                            (path_count * 2 - 2));

                        Func_080072f4(
                            render_context,
                            (const u8 *)graphics + tile,
                            x - half,
                            y - path_count,
                            path_count,
                            path_count * 2,
                            runtime_header[7],
                            runtime_header[6]);
                    }
                }
            }

            motions[phase].x += motions[phase].velocity_x;
            motions[phase].y += motions[phase].velocity_y;
            motions[phase].z += motions[phase].velocity_z;

            if (frame == phase * 12 + 10) {
                target->effect_52 = 0x10000;
                target->effect_48 = 0x80000;
                target->effect_40 = 0x1400000;
                target->callback = (void *)0x080d1341;
                *((u8 *)target + 90) = 0;
                Func_08009140(target);
                if (target->x < 0)
                    Func_08009150(
                        target, target->x + 0x280000, 0, target->z);
                else
                    Func_08009150(
                        target, target->x + 0x500000, 0, target->z);

                if (phase == 2)
                    Func_080b50e8(134);
                else {
                    Func_080f9010(134);
                    Func_080d6888(scene->secondary_id, 7, 5, 0, 8);
                    *(s32 *)(runtime + 0x77a8) = 4;
                }
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
