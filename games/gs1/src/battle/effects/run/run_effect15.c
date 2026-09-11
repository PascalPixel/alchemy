#include "types.h"
#include "scene.h"
#include "abi/battle/effects/run/run_effect15.h"

struct EffectPosition { s32 x, y, z; };
struct EffectObject {
    u8 pad00[100];
    s16 timer;
    u8 pad66[6];
    void (*callback)(void);
};

extern u8 *gIw;
extern u8 gCell[];

void BattleFx_RunEffect15(void)
{
    u8 *scene = gIw;
    u8 *main_object = *(u8 **)(scene + 16);
    struct EffectObject *effect = (struct EffectObject *)main_object;
    struct EffectPosition position;
    u8 *particle;
    u8 *entry;
    u8 stopped;
    u32 index;

    Battle_Run();
    Battle_SetMode(-1, -1, -1, 0);
    Battle_Run2();
    Battle_Do(10);
    Battle_Place(*(s16 *)(scene + 24), 0x4000, 0);
    Battle_Do(30);
    *(void (**)(void))(main_object + 108) = Battle_Run3;
    Battle_Do2(0x83);
    Battle_Apply(main_object, 28);
    Battle_Do(40);
    Battle_Do2(0xdc);
    Battle_Apply2(main_object, 0);
    Battle_Apply(main_object, 3);
    *(void (**)(void))(main_object + 108) = Battle_Run4;
    effect->timer = 0;
    Battle_Do(70);
    Battle_Apply3(main_object, 0);
    *(u8 *)(main_object + 85) = 0;
    *(void (**)(void))(main_object + 108) = Battle_Run5;
    *(u32 *)(main_object + 56) = 0x80000000;
    position.x = *(s32 *)(main_object + 8);
    position.y = *(s32 *)(main_object + 12);
    position.z = *(s32 *)(main_object + 16);
    Battle_Do3(&position);
    index = 0;
    particle = scene;
    particle += 88;
    do {
        s32 speed;
        Battle_SetMode2(particle, 0x11c, position.x, position.z);
        Battle_Apply4(particle, Battle_Run6);
        Battle_Apply5(particle, 7);
        Battle_Apply6(*(void **)particle, (Battle_Run7() * 7) >> 16);
        speed = (Battle_Run7() >> 1) + 0x13333;
        *(s32 *)(particle + 44) = speed;
        *(s32 *)(particle + 40) = speed;
        index++;
        Battle_Do(1);
        particle += 72;
    } while (index <= 23);
    Battle_Do(70);
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
    Battle_Do(40);
    Battle_Run8();
    Battle_Do(10);
}
