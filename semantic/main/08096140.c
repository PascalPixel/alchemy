#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct Position {
    s32 x;
    s32 y;
    s32 z;
} Position;

s16 Func_080022ec(s32, s32);
s32 Func_080022f4(u32, s32);
void Func_080030f8(s32);
u32 Func_08004458(void);
void Func_08009080(u8 *, s32);
void Func_08009098(u8 *, u32);
void Func_080091f0(s32, s32, s32);
void Func_08009240(u8 *, s32);
void Func_08009248(s32, s32);
u8 *Func_08092054(u32);
void Func_08092560(u32, s32, s32);
void Func_08092adc(u32, s32, s32);
void Func_080958a8(void);
void Func_080958e4(void);
void Func_08096bec(u8 *, s32, u32);
u8 *Func_08096c80(s32, s32, s32, s32);
void Func_080974d8(Position *);
void Func_0809ba70(u8 *, s32);
void Func_0809ba7c(u8 *, u32);
void Func_0809ba90(u8 *, s32, s32, s32);
void Func_080b0048(s32);
void Func_080b0050(void);
void Func_080f9010(s32);

/*
 * Run a scripted object effect, first emitting 24 radial slots and then two
 * waves of spawned objects around the current actor.
 */
void Func_08096140(s32 slot)
{
    u32 current_id = *(u32 *)0x02000434;
    u8 *current = Func_08092054(current_id);
    u8 *target = Func_08092054((u32)slot);
    u8 *effect_runtime;
    Position position;
    s32 i;

    if (target == 0)
        return;

    Func_080958a8();
    effect_runtime = *(u8 **)0x03001f30;
    Func_080b0048(0x201204);
    Func_080030f8(0x1e);

    FIELD(target, s8, 0x5b) = 0;
    Func_080f9010(0x98);
    Func_08092560((u32)slot, 4, 0x0f);
    Func_080f9010(0x98);
    Func_08092560((u32)slot, 4, 0x0f);
    Func_080030f8(0x1e);

    FIELD(target, u32, 0x6c) = 0x0809592d;
    Func_080f9010(0x99);
    Func_08092560((u32)slot, 8, 0x16);
    Func_080f9010(0x8c);
    Func_080091f0(0x14ccc, 0x14ccc, 0x10000);
    FIELD(target, u32, 0x6c) = 0x08095f9d;
    Func_08009080(target, 3);
    Func_080030f8(0x5a);
    Func_08092adc(current_id, 0x4000, 0);
    Func_080030f8(0x14);
    Func_08009080(Func_08092054(current_id), 0x1c);
    Func_080030f8(0x1e);
    Func_080091f0(0x19999, 0x19999, 0x10000);

    Func_080974d8(&position);
    for (i = 0; i < 24; i++) {
        u8 *effect = effect_runtime + 0x58 + i * 0x48;

        Func_0809ba90(effect, 0x11c, position.x, position.z);
        Func_0809ba7c(effect, 0x08096049);
        Func_0809ba70(effect, 7);
        Func_08009248(FIELD(effect, s32, 0), 0x0b);
        FIELD(effect, s32, 0x28) = 0x8000;
        FIELD(effect, s32, 0x2c) = Func_08004458() + 0x18000;
        Func_080030f8(1);
    }

    Func_080030f8(0x8c);
    for (i = 0; i < 24; i++) {
        u8 *effect = effect_runtime + 0x98 + i * 0x48;

        if (FIELD(effect, s8, 5) != 0)
            FIELD(effect, s8, 0) = 2;
    }

    Func_080030f8(0x14);
    Func_080091f0(1, 1, 1);
    Func_080030f8(0x1e);

    for (i = 0; i < 24; i++) {
        u8 *object;
        s32 scale;

        position.x = FIELD(current, s32, 8);
        position.y = FIELD(current, s32, 0x0c) + 0x780000;
        position.z = FIELD(current, s32, 0x10);
        object = Func_08096c80(0x11c, position.x, position.y, position.z);
        if (object != 0) {
            scale = Func_080022f4(Func_08004458(), 3) + 0x10000;
            FIELD(object, s32, 0x1c) = scale;
            FIELD(object, s32, 0x18) = scale;
            FIELD(object, s16, 0x64) = 0x64;
            FIELD(object, s16, 0x66) = Func_080022ec(i << 16, 0x18);
            FIELD(object, u32, 0x6c) = 0x08095fcd;
            FIELD(object, s8, 0x55) = 0;
            Func_08009080(object, 7);
            Func_08009240(object, 0x0b);
        }
    }

    Func_080030f8(0x64);
    Func_080f9010(0x120);
    Func_080030f8(1);
    Func_080f9010(0x97);

    position.x = FIELD(current, s32, 8);
    position.y = FIELD(current, s32, 0x0c) + 0x120000;
    position.z = FIELD(current, s32, 0x10);
    for (i = 0; i < 8;) {
        u8 *object =
            Func_08096c80(0x11c, position.x, position.y, position.z);

        if (object == 0)
            break;

        FIELD(object, s32, 0x1c) = 0x9999;
        FIELD(object, s32, 0x18) = 0x9999;
        FIELD(object, s8, 0x55) = 2;
        FIELD(object, s32, 0x28) = 0x50000;
        FIELD(object, s32, 0x14) = FIELD(object, s32, 0x0c);
        FIELD(object, s32, 0x30) = Func_08004458() + 0x16666;
        Func_08096bec(object, 0x200000, Func_08004458());
        Func_08009240(object, 0x0b);
        FIELD(object, s16, 0x5e) = 8;
        Func_08009098(object, 0x0809f0b0);
        i++;
    }

    Func_080030f8(0x0f);
    Func_080b0050();
    Func_080958e4();
}
