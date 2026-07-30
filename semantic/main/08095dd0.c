#include "types.h"

#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define S32_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define S8_AT(base, offset) (*(s8 *)((u8 *)(base) + (offset)))
#define U8_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

u8 *Func_08092054(s32 slot);
void Func_080958a8(void);
void Func_080b0048(s32 resources);
void Func_080030f8(u32 frames);
void Func_08092adc(s32 slot, s32 mode, s32 flags);
void Func_080f9010(s32 sound);
void Func_080925cc(s32 slot, s32 mode);
void Func_08009080(void *object, s32 mode);
void Func_080974d8(s32 *position);
void Func_0809ba90(
    void *particle,
    s32 resource,
    s32 x,
    s32 z);
void Func_0809ba7c(void *particle, const void *callback);
void Func_0809ba70(void *particle, s32 mode);
void Func_08009248(s32 object, s32 animation);
u32 Func_08004458(void);
s32 Func_080022f4(u32 value, s32 divisor);
void Func_080b0050(void);
void Func_080958e4(void);

/*
 * Run the scripted effect sequence for one slot, emit 24 randomized particles
 * around its normalized position, then advance those particles through their
 * two cleanup states before restoring the scene objects.
 */
void Func_08095dd0(s32 slot)
{
    u8 *primary = Func_08092054(slot);
    s32 current_slot = *(s32 *)(0x02000240 + 500);
    u8 *secondary = Func_08092054(current_slot);
    u8 *battle;
    s32 position[3];
    s32 index;

    if (primary == 0)
        return;

    Func_080958a8();
    battle = *(u8 **)0x03001F30;
    Func_080b0048(0x201090);
    Func_080030f8(30);
    Func_08092adc(slot, 0x4000, 0);
    Func_080030f8(20);

    Func_080f9010(0xAD);
    Func_080925cc(slot, 1);
    Func_080f9010(0xAE);
    Func_080925cc(slot, 1);
    Func_080f9010(0xAF);
    Func_080925cc(slot, 1);
    Func_080030f8(20);

    Func_080f9010(0x8C);
    S32_AT(primary, 0x6C) = 0x08095BAD;
    S16_AT(primary, 0x64) = 0;
    Func_080030f8(80);
    S32_AT(primary, 0x6C) = 0x08095BD9;
    Func_08009080(primary, 3);

    position[0] = S32_AT(primary, 8);
    position[1] = S32_AT(primary, 0xC);
    position[2] = S32_AT(primary, 0x10);
    Func_080974d8(position);

    for (index = 0; index < 24; index++) {
        u8 *particle = battle + 0x58 + index * 0x48;
        s32 scale;

        Func_0809ba90(particle, 0x11C, position[0], position[2]);
        Func_0809ba7c(particle, (const void *)0x08095C09);
        Func_0809ba70(particle, 7);
        Func_08009248(S32_AT(particle, 0), 10);
        scale = Func_080022f4(Func_08004458(), 3) + 0x10000;
        S32_AT(particle, 0x2C) = scale;
        S32_AT(particle, 0x28) = scale;
        Func_080030f8(1);
    }

    Func_080030f8(60);
    Func_08092adc(current_slot, 0x4000, 0);
    Func_080030f8(20);
    Func_08009080(Func_08092054(current_slot), 0x1C);
    Func_080030f8(20);

    for (index = 0; index < 24; index++) {
        u8 *state = battle + 0x98 + index * 0x48;

        if (S8_AT(state, 5) != 0)
            U8_AT(state, 0) = 2;
    }

    Func_080030f8(60);
    S32_AT(secondary, 0x6C) = 0x08095B8D;
    Func_080030f8(100);

    for (index = 0; index < 24; index++) {
        u8 *state = battle + 0x98 + index * 0x48;

        if (S8_AT(state, 5) != 0)
            U8_AT(state, 0) = 5;
    }

    Func_080030f8(10);
    S32_AT(secondary, 0x6C) = 0;
    S32_AT(secondary, 0x18) = 0x10000;
    S32_AT(secondary, 0x1C) = 0x10000;
    Func_080030f8(30);
    Func_080b0050();
    Func_080958e4();
}
