#include "types.h"

typedef s32 (*Scale_080109e8)(s32, s32);
typedef void (*Transform_080109e8)(s32 *, void *);
typedef void (*Build_080109e8)(void *, s32 *, void *, const void *);

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void *Func_08002f40(s32);
void Func_08003bb4(s32);
void Func_080041d8(const void *, u32);
u32 Func_080048b0(s32, u32);
void *Func_080048f4(s32, s32);
void Func_080049ac(void);
void Func_08004bd4(s32);
void Func_08004c1c(s32);
void Func_08004cb4(s32 *);
void Func_080051d8(void *, s32 *);
void Func_08005258(s32, s32, s32);
void Func_08005340(const void *, void *);
void Func_080118d8(void *);
void Func_080123f4(s32, s32 *, void *);

void Func_080109e8(void)
{
    u8 *state;
    u8 *model;
    u8 *work;
    s32 *position;
    s32 transformed[3];
    s32 zero = 0;
    s32 magnitude = 255 << 17;
    s32 angle;
    s32 scaled;
    Scale_080109e8 scale = (Scale_080109e8)0x0300013c;
    Transform_080109e8 transform =
        (Transform_080109e8)0x03000250;

    *(volatile u16 *)0x04000000 &= 0xc1ff;
    Func_08003bb4(0);

    state = (u8 *)Func_080048b0(8, 860);
    *(volatile const void **)0x040000d4 = &zero;
    *(volatile void **)0x040000d8 = state;
    *(volatile u32 *)0x040000dc = 0x850000d7;

    *(s32 *)(state + 228) = 0;
    *(s32 *)(state + 232) = 0;
    *(s32 *)(state + 236) = 0x200000;
    *(s32 *)(state + 240) = 0x400000;
    *(s32 *)(state + 244) = magnitude;
    *(s32 *)(state + 248) = magnitude;
    *(s32 *)(state + 16) = 0;
    *(void **)(state + 272) = Func_08002f40(0xd4);

    Func_08005340(Func_08002f40(0xd6), (void *)0x0202d000);
    Func_080118d8((void *)0x0202d000);
    *(volatile u16 *)0x04000050 = 0x3f9e;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(volatile u16 *)0x04000054 = 0;
    Func_08005340(Func_08002f40(0xd5), (void *)0x02010000);
    Func_08005340(Func_08002f40(0xd7), (void *)0x0202c000);

    *(u16 *)(state + 20) = 0x1f00;
    *(u8 *)(state + 22) = 0x80;
    *(volatile u16 *)0x0400000e = 0xa80a;
    *(volatile u16 *)0x0400000c = 0xaa0e;
    *(volatile u16 *)0x0400000a = 0x0501;
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 0x100;
    *(volatile u32 *)0x04000028 = 0;
    *(volatile u32 *)0x0400002c = 0;
    *(volatile u16 *)0x04000030 = 0x100;
    *(volatile u16 *)0x04000032 = 0;
    *(volatile u16 *)0x04000034 = 0;
    *(volatile u16 *)0x04000036 = 0x100;
    *(volatile u32 *)0x04000038 = 0;
    *(volatile u32 *)0x0400003c = 0;

    work = Func_080048f4(12, 76);
    model = (u8 *)Func_080048b0(7, 0x284);
    position = (s32 *)(work + 12);

    *(s32 *)(state + 840) = magnitude;
    *(s32 *)(state + 844) = magnitude;
    *(s32 *)(state + 852) = 0x10000;
    *(u16 *)(state + 856) = 0;
    *(s32 *)(work + 24) = 0;
    *(s32 *)(work + 28) = 0;
    *(s32 *)(0x03001ce0 + 12) = 120;
    *(s32 *)(0x03001ce0 + 16) = 96;

    Func_08005258(magnitude, magnitude / 2, magnitude * 2);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(*(u16 *)(state + 282));
    Func_08004bd4(*(u16 *)(state + 280));

    transformed[0] = 0;
    transformed[1] = 0;
    transformed[2] = magnitude;
    transform(transformed, work);

    Func_080049ac();
    Func_080051d8(work, position);

    *(volatile const void **)0x040000d4 =
        (const void *)0x0800a0f8;
    *(volatile void **)0x040000d8 = model;
    *(volatile u32 *)0x040000dc =
        0x84000000 | ((u32)0x03001f60 >> 2);

    angle = *(u16 *)(state + 280);
    scaled = scale(Func_0800231c(angle), Func_08002322(angle));
    Func_080123f4(scaled, position, model);

    *(s32 *)0x03001af4 = 0;
    *(s32 *)0x03001e40 = angle;
    {
        u32 selector = *(u32 *)0x03001e50 & 1;
        Build_080109e8 build =
            *(Build_080109e8 *)0x03001f08;

        build(work, position, model,
              model + 3200 + selector * 5120);
    }

    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Func_080049ac();
    *(u16 *)(state + 280) = 0xe000;
    *(u16 *)(state + 282) = 0;
    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(*(u16 *)(state + 282));
    Func_08004bd4(*(u16 *)(state + 280));

    transformed[0] = 0;
    transformed[1] = 0;
    transformed[2] = *(s32 *)(state + 844) + 0x10000;
    transform(transformed, work);

    *(volatile u16 *)0x0400004c = 0;
    *(volatile u16 *)0x04000000 = 66;
    *(volatile u16 *)0x03001ad4 = 0;
    *(volatile u16 *)0x03001ad6 = 0;
    *(volatile u16 *)0x03001ad8 = 0;
    *(volatile u16 *)0x03001ada = 0;
    *(volatile u16 *)0x03001adc = 0;
    *(volatile u16 *)0x03001ade = 0;
    *(u16 *)(state + 256) = 0;
    *(u16 *)(state + 258) = 159;
    Func_080041d8((const void *)0x080111b5, 0x0c85);
    Func_080041d8((const void *)0x08010ff1, 0x480);

    {
        u16 *entry = (u16 *)(state + 0x336);
        s32 value;

        for (value = 255; value >= 0; value--)
            *entry-- = (u16)value;
    }
}
