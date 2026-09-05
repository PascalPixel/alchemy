#include "types.h"

struct EffectPosition { s32 x, y, z; };
struct EffectObject {
    u8 pad00[100];
    s16 timer;
    u8 pad66[6];
    void (*callback)(void);
};

extern u8 *Data_03001f30;
extern u8 Data_02000240[];

void Func_080916b0(void);
void Func_080933f8(s32, s32, s32, s32);
void Func_08097384(void);
void Func_080030f8(s32);
void Func_08092adc(s32, s32, s32);
void Func_080f9010(s32);
void Func_08009080(void *, s32);
void Func_08009240(void *, s32);
void Func_080091e0(void *, s32);
void Func_080974d8(struct EffectPosition *);
void Func_0809ba90(void *, s32, s32, s32);
void Func_0809ba7c(void *, const void *);
void Func_0809ba70(void *, s32);
u32 Func_08004458(void);
void Func_08009248(void *, s32);
void Func_0809748c(void);
void Func_08096b88(void);
void Func_0809b0b0(void);
void Func_0809b0dc(void);
void Func_0809b11c(void);

void BattleEffect_RunEffect15(void)
{
    u8 *scene = Data_03001f30;
    u8 *main_object = *(u8 **)(scene + 16);
    struct EffectObject *effect = (struct EffectObject *)main_object;
    struct EffectPosition position;
    u8 *particle;
    u8 *entry;
    u8 stopped;
    u32 index;

    Func_080916b0();
    Func_080933f8(-1, -1, -1, 0);
    Func_08097384();
    Func_080030f8(10);
    Func_08092adc(*(s16 *)(scene + 24), 0x4000, 0);
    Func_080030f8(30);
    *(void (**)(void))(main_object + 108) = Func_08096b88;
    Func_080f9010(0x83);
    Func_08009080(main_object, 28);
    Func_080030f8(40);
    Func_080f9010(0xdc);
    Func_08009240(main_object, 0);
    Func_08009080(main_object, 3);
    *(void (**)(void))(main_object + 108) = Func_0809b0b0;
    effect->timer = 0;
    Func_080030f8(70);
    Func_080091e0(main_object, 0);
    *(u8 *)(main_object + 85) = 0;
    *(void (**)(void))(main_object + 108) = Func_0809b0dc;
    *(u32 *)(main_object + 56) = 0x80000000;
    position.x = *(s32 *)(main_object + 8);
    position.y = *(s32 *)(main_object + 12);
    position.z = *(s32 *)(main_object + 16);
    Func_080974d8(&position);
    index = 0;
    particle = scene;
    particle += 88;
    do {
        s32 speed;
        Func_0809ba90(particle, 0x11c, position.x, position.z);
        Func_0809ba7c(particle, Func_0809b11c);
        Func_0809ba70(particle, 7);
        Func_08009248(*(void **)particle, (Func_08004458() * 7) >> 16);
        speed = (Func_08004458() >> 1) + 0x13333;
        *(s32 *)(particle + 44) = speed;
        *(s32 *)(particle + 40) = speed;
        index++;
        Func_080030f8(1);
        particle += 72;
    } while (index <= 23);
    Func_080030f8(70);
    index = 0;
    entry = scene;
    stopped = 2;
    entry += 152;
    do {
        if (*(s8 *)(entry + 5) != 0) {
            entry[0] = stopped;
        }
        index++;
        entry += 72;
    } while (index <= 23);
    Func_080030f8(40);
    Func_0809748c();
    Func_080030f8(10);
}
