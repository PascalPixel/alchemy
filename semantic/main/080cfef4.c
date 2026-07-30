#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void Func_080cdb24(s32);
s32 Func_08002f40(s32);
void Func_080072f0();
void Func_08005340();
void Func_080ed408();
void Func_080041d8();
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080f9010(s32);
void Func_080e396c(s32, s32 *);
void Func_080072f4();
void **Func_080b5098(s32);
void Func_08009150();
void Func_080d6888(s16, s32, s32, s32);
void Func_080030f8(s32);
void Func_08004278(const void *);
void Func_080b5040(s32, u16, s32);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080cfef4(void *scene)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics_source = *(s32 *)0x03001ef0;
    s32 resource_2e;
    s32 side;
    s32 center_x;
    s32 center_y;
    s32 draw_enabled;
    s32 frame;
    s32 local_state[2];

    PTR_AT(runtime, 0x7828) = scene;
    Func_080cdb24(0);
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000052 = 0x1010;

    resource_2e = Func_08002f40(0xab);
    Func_080072f0((void *)0x05000000, resource_2e, 0x80,
                  (void *)0x03001388);
    Func_08005340(resource_2e + 0x80, runtime);
    Func_08005340(Func_08002f40(0xac) + 0x80, (void *)0x02010000);
    Func_080ed408(0x2e, 7, 7, 3, 1);
    local_state[0] = *(s32 *)0x03001f08;
    Func_080ed408(0x2f, 7, 7, 7, 1);
    local_state[1] = *(s32 *)0x03001f0c;

    Func_080041d8((void *)0x080dbb9d, 0x480);
    S32_AT(runtime, 0x7780) = 1;
    S32_AT(runtime, 0x7784) = 0;
    Func_080041d8((void *)0x080cd261, 0x480);

    draw_enabled = 1;
    if (S32_AT(scene, 4) == 1)
        center_x = -0x500000;
    else
        center_x = 0x700000;
    center_y = -0x200000;

    for (frame = 0; frame != 0x84; frame++) {
        s32 angle = frame << 9;
        s32 x = (center_x >> 16) +
                ((Func_08002322(angle) * 16) >> 16);
        s32 y = (center_y >> 16) +
                ((Func_0800231c(angle) * 4) >> 16) + 16;
        s32 spread = 0;
        s32 i;
        s32 wave_angle;
        s32 *wave;

        if (frame == 0x58)
            Func_080f9010(0x86);

        if (frame == 0x20) {
            center_x = S32_AT(scene, 4) == 1 ? -0x200000 : 0x480000;
            center_y = 0x180000;
            draw_enabled = 0;
        }
        if (frame == 0x21) {
            *(volatile u16 *)0x04000052 = 0x1010;
            draw_enabled = 1;
        }
        if (frame == 0x40) {
            s32 destination;
            Func_080e396c(S16_AT(scene, 0x24), &destination);
            center_x =
                (destination - (S32_AT(scene, 4) == 1 ? 0x80 : 0x40))
                << 16;
            center_y = 0;
            draw_enabled = 0;
        }
        if (frame == 0x41) {
            *(volatile u16 *)0x04000052 = 0x1010;
            draw_enabled = 1;
        }

        if (frame <= 0x1f) {
            if (frame > 0x0f) {
                spread = frame * 2 - 0x20;
                *(volatile u16 *)0x04000052 =
                    (u16)((0x1f - frame) | 0x1000);
            }
        } else if (frame <= 0x3f && frame > 0x2f) {
            *(volatile u16 *)0x04000052 =
                (u16)((0x3f - frame) | 0x1000);
            spread = frame * 2 - 0x60;
        }
        if (spread < 0)
            spread = 0;

        wave = (s32 *)(runtime + 0x6980);
        wave_angle = frame << 11;
        for (i = 0; i != 0xa0; i++) {
            *wave++ = ((6 - (x + 0x30)) << 8) -
                      ((spread * Func_08002322(wave_angle)) >> 10);
            wave_angle += 0x800;
        }

        if (draw_enabled) {
            s32 variant = S32_AT(scene, 4) != 0;
            s32 table_index;

            if (frame <= 0x57) {
                Func_080072f4(
                    graphics_source, runtime,
                    U8_AT((void *)0x080ee10c, variant * 7),
                    U8_AT((void *)0x080ee11a, 0) + y, 0x39, 0x62);
            } else {
                table_index = variant * 7;
                if (frame <= 0x5b) {
                    Func_080072f4(
                        graphics_source, runtime,
                        U8_AT((void *)0x080ee10c, table_index),
                        U8_AT((void *)0x080ee11a, 0) + y, 0x39, 0x62);
                }

                Func_080072f4(
                    graphics_source, runtime + 0x15d2,
                    U8_AT((void *)0x080ee10c, table_index + 1),
                    U8_AT((void *)0x080ee11a, 1) + y, 0x63, 0x45);

                if ((u32)(frame - 0x58) <= 1)
                    Func_080072f0(
                        graphics_source, 0x4000, 0x3f3f3f3f,
                        (void *)0x03000168);
                if ((u32)(frame - 0x5a) <= 1)
                    Func_080072f4(
                        graphics_source, runtime + 0x3081,
                        U8_AT((void *)0x080ee10c, table_index + 2),
                        U8_AT((void *)0x080ee11a, 2) + y, 0x80, 0x5b);
                if ((u32)(frame - 0x5c) <= 1)
                    Func_080072f4(
                        graphics_source, (void *)0x02010000,
                        U8_AT((void *)0x080ee10c, table_index + 3),
                        U8_AT((void *)0x080ee11a, 3) + y, 0x80, 0x5b);
                if ((u32)(frame - 0x5e) <= 1)
                    Func_080072f4(
                        graphics_source, (void *)0x02012d80,
                        U8_AT((void *)0x080ee10c, table_index + 4),
                        U8_AT((void *)0x080ee11a, 4) + y, 0x80, 0x3b);
                if ((u32)(frame - 0x60) <= 1)
                    Func_080072f4(
                        graphics_source, (void *)0x02014b00,
                        U8_AT((void *)0x080ee10c, table_index + 5),
                        U8_AT((void *)0x080ee11a, 5) + y, 0x7a, 0x1d);
                if ((u32)(frame - 0x62) <= 1)
                    Func_080072f4(
                        graphics_source, (void *)0x020158d2,
                        U8_AT((void *)0x080ee10c, table_index + 6),
                        U8_AT((void *)0x080ee11a, 6) + y, 0x4c, 0x19);
            }
        }

        if (frame == 0x58) {
            void *object = *Func_080b5098(S16_AT(scene, 0x24));
            S32_AT(object, 0x28) = 0x10000;
            S32_AT(object, 0x34) = 0x20000;
            S32_AT(object, 0x30) = 0x20000;
            S32_AT(object, 0x48) = 0;
            S8_AT(object, 0x5a) = 0;
            S8_AT(object, 0x58) = 0;
            Func_08009150(
                object, S32_AT(object, 8) * 2, 0, S32_AT(object, 0x10));
            Func_080d6888(S16_AT(scene, 0x24), -1, 5, 0);
        }
        if (frame == 0x78)
            S32_AT(*Func_080b5098(S16_AT(scene, 0x24)), 0x48) = 0xab85;

        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08004278((const void *)0x080dbb9d);
    Func_080b5040(1, U16_AT(*(void **)0x03001e74, 0x648), 0x18);
    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_080cdbc0();
}
