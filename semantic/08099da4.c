#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Position_08099da4 {
    s32 x;
    s32 y;
    s32 z;
};

void Func_08097384(void);
void Func_08009098(void *, const void *);
void *Func_0809a3c4(s32, s32, s32, s32);
void Func_0809748c(void);
void Func_080030f8(s32);
void Func_08009150(void *, s32, s32, s32);
void Func_08009158(void *);
void Func_080041d8(const void *, s32);
void Func_080f9010(s32);
void Func_080091e0(void *, s32);
u16 Func_08097b54(void);
void Func_08009080(void *, s32);
void Func_0800447c(s32, s32, struct Position_08099da4 *);
s8 Func_08009250(void *, struct Position_08099da4 *);
s32 Func_080091d8(void *, struct Position_08099da4 *);
void Func_08004278(const void *);
void *Func_08096c80(s32, s32, s32, s32);
u32 Func_08004458(void);
void Func_08096bec(void *, s32, u32);
void Func_080090d0(void *);
void Func_08009240(void *, s32);
void Func_0809a6b8(void *);

void Func_08099da4(void)
{
    u8 *runtime = *(u8 **)0x03001f30;
    u8 *camera = PTR_AT(runtime, 0x10);
    u8 *actor = PTR_AT(runtime, 0x14);
    void *left;
    void *right;
    struct Position_08099da4 position;
    void *particles[20];
    u32 i;

    if (actor == 0)
        return;

    Func_08097384();
    PTR_AT(camera, 0x68) = actor;
    Func_08009098(camera, (const void *)0x0809f0bc);

    position.x = S32_AT(runtime, 4);
    position.y = S32_AT(runtime, 8) + 0x100000;
    position.z = S32_AT(runtime, 0x0c);
    left = Func_0809a3c4(
        position.x + 0x200000, position.y, position.z, 0x8000);
    right = Func_0809a3c4(
        position.x - 0x200000, position.y, position.z, 0);
    if (left == 0 || right == 0) {
        Func_0809748c();
        return;
    }

    Func_080030f8(15);
    position.x = S32_AT(actor, 8);
    position.y = S32_AT(actor, 0x0c) + 0x100000;
    position.z = S32_AT(actor, 0x10);
    Func_08009150(
        left, position.x + 0x100000, position.y, position.z);
    Func_08009150(
        right, position.x - 0x100000, position.y, position.z);
    Func_08009158(left);
    Func_08009158(right);
    S32_AT(left, 0x24) = 0;
    S32_AT(left, 8) = position.x + 0x100000;
    S32_AT(right, 8) = position.x - 0x100000;
    S32_AT(right, 0x24) = 0;

    S32_AT(actor, 0x6c) = 0x08096b89;
    Func_080041d8((const void *)0x08099d19, 0xc80);
    Func_080f9010(0x82);
    S8_AT(actor, 0x55) = 4;
    Func_080091e0(actor, 0);

    while (S32_AT(actor, 0x0c) - S32_AT(actor, 0x14) <= 0x180000) {
        S32_AT(left, 0x0c) += 0x6000;
        S32_AT(right, 0x0c) += 0x6000;
        S32_AT(actor, 0x0c) += 0x6000;
        Func_080030f8(1);
    }

    S32_AT(left, 0x30) = 0x40000;
    S32_AT(left, 0x34) = 0x8000;
    S32_AT(right, 0x30) = 0x40000;
    S32_AT(right, 0x34) = 0x8000;
    S32_AT(actor, 0x30) = 0x6666;
    S32_AT(actor, 0x34) = 0x3333;
    S8_AT(actor, 0x5a) = 0;
    S8_AT(actor, 0x22) = 2;

    for (;;) {
        Func_080030f8(1);
        if (*(u32 *)0x03001c94 & 0x303)
            break;

        {
            u16 tile = Func_08097b54();
            if (tile == 0xffff) {
                position.x = S32_AT(actor, 8);
                position.y = S32_AT(actor, 0x0c) + 0x100000;
                position.z = S32_AT(actor, 0x10);
                Func_08009150(
                    left, position.x + 0x100000, position.y, position.z);
                Func_08009150(
                    right, position.x - 0x100000, position.y, position.z);
                Func_08009080(left, 1);
                Func_08009080(right, 1);
                continue;
            }

            position.x = S32_AT(actor, 8);
            position.y = S32_AT(actor, 0x0c) + 0x100000;
            position.z = S32_AT(actor, 0x10);
            Func_0800447c(0x20000, tile, &position);
            Func_08009150(
                left, position.x + 0x100000, position.y, position.z);
            Func_08009150(
                right, position.x - 0x100000, position.y, position.z);
            Func_08009158(left);
            Func_08009158(right);

            position.x = S32_AT(actor, 8);
            position.y = S32_AT(actor, 0x14);
            position.z = S32_AT(actor, 0x10);
            Func_0800447c(0x100000, tile, &position);
            if (Func_08009250(actor, &position) != 0) {
                Func_08009080(left, 4);
                Func_08009080(right, 4);
                if (!(*(u32 *)0x03001e40 & 15))
                    Func_080f9010(0x72);
                continue;
            }

            S32_AT(actor, 0x14) += 0x100000;
            i = Func_080091d8(actor, &position);
            S32_AT(actor, 0x14) -= 0x100000;
            if (i > 0) {
                Func_08009080(left, 4);
                Func_08009080(right, 4);
                if (!(*(u32 *)0x03001e40 & 15))
                    Func_080f9010(0x72);
                continue;
            }

            Func_080f9010(0xaf);
            Func_08009080(left, 4);
            Func_08009080(right, 4);
            Func_080030f8(15);
            S8_AT(actor, 0x5b) = 0;
            S32_AT(actor, 0x30) = 0x3333;
            S32_AT(actor, 0x34) = 0x3333;
            Func_08009150(actor, position.x, position.y, position.z);
            S32_AT(left, 0x30) = 0x3333;
            S32_AT(left, 0x34) = 0x3333;
            S32_AT(right, 0x30) = 0x3333;
            S32_AT(right, 0x34) = 0x3333;
            Func_08009150(
                left, position.x + 0x100000, position.y, position.z);
            Func_08009150(
                right, position.x - 0x100000, position.y, position.z);
            Func_08009158(actor);
            S32_AT(actor, 8) = position.x;
            S32_AT(actor, 0x10) = position.z;
            S32_AT(actor, 0x24) = 0;
            S32_AT(actor, 0x2c) = 0;
            Func_080030f8(10);
        }
    }

    Func_08009080(left, 4);
    Func_08009080(right, 4);
    Func_08004278((const void *)0x08099d19);
    Func_080f9010(0x87);
    Func_080030f8(15);
    Func_080f9010(0x87);
    Func_080030f8(15);

    position.x = S32_AT(actor, 8);
    position.y = S32_AT(actor, 0x0c) + 0x100000;
    position.z = S32_AT(actor, 0x10);
    for (i = 0; i < 20; i++) {
        void *particle =
            Func_08096c80(0x11d, position.x, position.y, position.z);
        particles[i] = particle;
        if (particle != 0) {
            Func_08009098(particle, (const void *)0x0809f0d4);
            S32_AT(particle, 0x34) = 0x20000;
            S32_AT(particle, 0x30) = Func_08004458() + 0x20000;
            S8_AT(particle, 0x55) = 0;
            Func_08096bec(
                particle, Func_08004458() * 0x18 + 0x80000,
                Func_08004458());
        }
    }

    Func_080f9010(0x83);
    Func_080090d0(left);
    Func_080090d0(right);
    Func_08009240(actor, U8_AT(runtime, 0x44));
    Func_08009098(actor, PTR_AT(runtime, 0x3c));
    S32_AT(actor, 0x6c) = S32_AT(runtime, 0x38);
    S8_AT(actor, 0x55) = 3;
    S32_AT(actor, 0x28) = 0xa0000;
    S32_AT(actor, 0x44) = 0x3333;
    S8_AT(actor, 0x22) = 0;
    S32_AT(camera, 0x6c) = 0;
    Func_08009240(camera, 0);

    if (S8_AT(runtime, 0x34) != 0) {
        for (i = 0; i <= 0x59 && S32_AT(actor, 0x28) >= 0; i++)
            Func_080030f8(1);
        Func_080030f8(1);
        for (i = 0; i <= 0x59 && S32_AT(actor, 0x28) < 0; i++)
            Func_080030f8(1);
        Func_0809a6b8(actor);
        Func_0809748c();
        Func_080030f8(30);
        return;
    }
    Func_0809748c();
}
