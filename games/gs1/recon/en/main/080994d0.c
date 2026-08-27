#include "types.h"

#define RunBattleEffect03 Func_080994d0

struct BattleEffect03Object {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[16];
    s32 velocity_x;
    s32 velocity_y;
    u8 reserved_38[24];
    void *visual;
    u8 reserved_54;
    u8 mode;
    u8 reserved_56[4];
    u8 unknown_5a;
    u8 reserved_5b[9];
    s16 angle;
    s16 phase;
    u8 reserved_68[4];
    void *callback;
};

struct BattleEffect03Link {
    u8 reserved_00[28];
    u8 marker;
};

struct BattleEffect03State {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffect03Object *target;
    u8 reserved_14[12];
    s8 long_delay;
    u8 reserved_21[3];
    void (*finish_callback)(void);
};

extern struct BattleEffect03State *Data_03001f30;
extern u8 Data_08099341;
extern u8 Data_080993b1;

void Func_08097384(void);
struct BattleEffect03Object *Func_08096c80(s32, s32, s32, s32);
struct BattleEffect03Link *Func_08096c48(void *, struct BattleEffect03Link *);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_08009240(struct BattleEffect03Object *, s32);
void Func_08096bec(struct BattleEffect03Object *, s32, s32);
void Func_08009158(struct BattleEffect03Object *);
void Func_080090d0(struct BattleEffect03Object *);
void Func_08003f3c(u8);
void Func_0809748c(void);
void Func_080072f0(void);

void RunBattleEffect03(void)
{
    struct BattleEffect03State *state = Data_03001f30;
    struct BattleEffect03Object *target = state->target;
    struct BattleEffect03Object *object;
    struct BattleEffect03Object *particle;
    struct BattleEffect03Link *last;
    u8 link_marker;
    s32 index;

    Func_08097384();
    last = 0;
    index = 0;
    do {
        object = Func_08096c80(
            0xe9, target->x, target->y + 0x200000, target->z);
        if (object != 0) {
            object->scale_y = 0xb333;
            object->scale_x = 0xb333;
            object->callback = &Data_08099341;
            object->angle = 0x78;
            object->phase = index << 13;
            object->mode = 4;
            last = Func_08096c48(object->visual, last);
        }
        index++;
        Func_080030f8(1);
    } while (index <= 7);

    link_marker = last->marker;
    Func_080f9010(0x82);
    Func_080030f8(110);
    object = Func_08096c80(0xe9, 0, 0, 0);
    particle = object;
    if (object != 0) {
        object->scale_y = 0xb333;
        object->scale_x = 0xb333;
        object->x = state->x;
        object->y = state->y + 0x100000;
        object->z = state->z;
        object->mode = 4;
        Func_08009240(object, 7);
    }

    Func_080f9010(0x83);
    Func_080030f8(12);
    if (object != 0) {
        index = 0;
        do {
            if (index & 3)
                Func_08009240(particle, 9);
            else
                Func_08009240(particle, 10);
            index++;
            Func_080030f8(2);
        } while (index <= 29);
    }

    Func_08009240(particle, 0);
    Func_080f9010(0x54);
    if (particle != 0) {
        object->callback = &Data_080993b1;
        object->angle = 0;
        if (state->long_delay != 0)
            Func_080030f8(128);
        else
            Func_080030f8(192);
    }
    if (object != 0) {
        object->angle = -1;
        object->velocity_x = 0x50000;
        object->velocity_y = 0x6666;
        object->unknown_5a = 0;
        Func_08096bec(object, 0xc00000, 0xe800);
        Func_08009158(object);
        Func_080090d0(object);
    }
    if (link_marker != 0x60)
        Func_08003f3c(link_marker);
    if (state->finish_callback != 0)
        Func_080072f0();
    Func_0809748c();
}
