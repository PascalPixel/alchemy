#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Spark_080d3854 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 age;
};

void Func_080cd594(s32);
void *Func_08002f40(s32);
void Func_080072f0(s32, s32, s32, s32);
void Func_08005340(const void *, void *);
void Func_080ed408(s32, s32, s32, s32, s32);
u32 Func_08004458(void);
void Func_080041d8(const void *, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
s32 Func_08002322(s32);
void Func_080b50e8(s32);
void Func_080072fc(s32, s32, s32);
s32 Func_0800231c(s32);
s32 Func_080022fc(s32, s32);
void Func_080072f4(s32, const void *, s32, s32, s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080b5088(s16, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_08002dd8(s32);
void Func_08004278(const void *);
void Func_080cdbc0(void);

void Func_080d3854(void *argument)
{
    u8 *runtime = *(u8 **)0x03001eec;
    s32 graphics = *(s32 *)0x03001ef0;
    struct Spark_080d3854 *sparks =
        (struct Spark_080d3854 *)(runtime + 0x7080);
    void *palette;
    s32 variant;
    s32 frame;
    s32 i;

    PTR_AT(runtime, 0x7828) = argument;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    palette = Func_08002f40(0xce);
    Func_080072f0(
        0x05000000, (s32)palette, 0x80, 0x03001388);
    Func_08005340((u8 *)palette + 0x80, runtime);
    Func_080ed408(0x2e, 7, 7, 3, 2);
    Func_080ed408(0x2f, 7, 7, 7, 2);

    for (i = 0; i < 16; i++) {
        sparks[i].x = Func_08004458() & 0x1f;
        sparks[i].y = (Func_08004458() & 0x3f) + 0x10;
        sparks[i].age = -(s32)(Func_08004458() & 0x0f);
    }

    S32_AT(runtime, 0x7780) = 2;
    S32_AT(runtime, 0x7784) = 0x32;
    Func_080041d8((const void *)0x080cd261, 0x480);
    *(volatile u16 *)0x04000052 = 0x1000;
    Func_080030f8(1);
    Func_080f9010(0x8d);

    variant = S32_AT(argument, 0x18);
    for (frame = 0; frame < 0x50; frame++) {
        s32 horizontal = Func_08002322(frame << 10) * 0x10;
        s32 group_count;
        s32 group;

        if (frame == 0x20)
            Func_080b50e8(0x85);
        for (i = 0; i < 7; i++) {
            if (frame == i * 8 + 0x10)
                Func_080072fc(graphics, 0x4000, 0x08080808);
        }

        horizontal +=
            S32_AT(argument, 4) == 1 ? 0x200000 : (s32)0xffe00000;
        if (frame <= 0x10)
            *(volatile u16 *)0x04000052 = frame | 0x1000;
        if (frame > 0x3f)
            *(volatile u16 *)0x04000052 =
                (0x4f - frame) | 0x1000;

        group_count = U8_AT((void *)0x080ee1ca, variant * 3);
        for (group = 0; group < group_count; group++) {
            s32 angle = frame << 11;
            s32 center_x =
                ((s32)(U8_AT((void *)0x080ee1ca, variant * 3 + 1) *
                       Func_08002322(angle)) +
                 horizontal) >>
                    16;
            s32 center_y =
                ((s32)(Func_0800231c(angle) * 2) >> 16);
            s32 animation = Func_080022fc(frame / 2, 3);
            struct Spark_080d3854 *spark = &sparks[group * 4];

            center_x += 0x28;
            center_y += 0x10;
            Func_080072f4(
                graphics, runtime + animation * 0xa00 + 0xc56,
                center_x, center_y, 0x28, 0x20);
            Func_080072f4(
                graphics, runtime + animation * 0x500 + 0x2a56,
                center_x, center_y + 0x20, 0x28, 0x20);
            Func_080072f4(
                graphics, runtime + animation * 0xa00 + 0x1156,
                center_x, center_y + 0x40, 0x28, 0x20);

            for (i = 0; i < 4; i++, spark++) {
                if (spark->age >= 0) {
                    s32 image =
                        (spark->age + ((u32)spark->age >> 31)) / 2 +
                        (i / 2) * 3;
                    Func_080072f4(
                        graphics,
                        runtime +
                            U16_AT((void *)0x080edebe, image * 2),
                        spark->x + center_x,
                        spark->y + center_y - 0x10,
                        U8_AT((void *)0x080edeca, image),
                        U8_AT((void *)0x080eded0, image));
                }
                spark->age++;
                if (spark->age == 6) {
                    spark->x = Func_08004458() & 0x1f;
                    spark->y = (Func_08004458() & 0x3f) + 0x10;
                    spark->age = 0;
                }
            }
        }

        for (i = 0; i < S32_AT(argument, 0x14); i++) {
            s32 pulse;
            for (pulse = 0; pulse < 7; pulse++) {
                if (frame == i * 3 + pulse * 8 + 0x10) {
                    s16 character =
                        S16_AT(argument, 0x24 + i * 2);
                    Func_080d6888(character, 7, 5, i, 4);
                    Func_080b5088(character, 6);
                }
            }
        }

        S32_AT(runtime, 0x77a8) = 1;
        {
            u8 layers =
                U8_AT((void *)0x080ee1ca, variant * 3 + 2);
            Func_080e155c(layers, layers * 2);
        }
        Func_080cd52c();
        S32_AT(runtime, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(0x2f);
    Func_08002dd8(0x2e);
    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
}
