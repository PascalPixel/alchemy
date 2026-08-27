#include "types.h"

#define RunBattleEffect07 Func_08098954

struct BattleEffectOrigin {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
};

struct BattleEffectObject {
    u8 reserved_00[6];
    u16 parameter;
    u8 reserved_08[40];
    s32 phase;
    s32 velocity;
    u8 reserved_38[29];
    u8 mode;
};

extern struct BattleEffectOrigin *Data_03001f30;

void Func_08097384(void);
struct BattleEffectObject *Func_08098a84(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_08096bec(struct BattleEffectObject *, s32, s32);
void Func_08009158(struct BattleEffectObject *);
void Func_080f9010(s32);
struct BattleEffectObject *Func_08096c80(s32);
void Func_08009098(struct BattleEffectObject *, u32);
u32 Func_08004458(void);
void Func_080090d0(struct BattleEffectObject *);
void Func_0809748c(void);

void RunBattleEffect07(void)
{
    struct BattleEffectOrigin *origin = Data_03001f30;
    struct BattleEffectObject *anchors[2];
    s32 position[3];
    s32 *pos;
    s32 index;

    Func_08097384();
    position[2] = origin->z;
    position[1] = origin->y + 0x100000;
    position[0] = origin->x;
    anchors[0] = Func_08098a84(
        position[0] + 0x200000, position[1], position[2], 0x8000);
    anchors[1] = Func_08098a84(
        position[0] - 0x200000, position[1], position[2], 0);

    Func_080030f8(15);
    for (index = 0; index < 2; index++) {
        struct BattleEffectObject *anchor = anchors[index];

        if (anchor != 0)
            Func_08096bec(anchor, 0x180000, anchor->parameter);
    }

    Func_08009158(anchors[0]);
    Func_080f9010(0x86);
    pos = position;
    for (index = 0; index < 24; index++) {
        struct BattleEffectObject *object;
        s32 distance;
        s32 zero;

        pos[1] = origin->y + 0x100000;
        pos[0] = origin->x;
        pos[2] = origin->z;
        object = Func_08096c80(0x11d);
        if (object != 0) {
            Func_08009098(object, 0x0809f0d4);
            object->velocity = 0x20000;
            object->phase = Func_08004458() + 0x20000;
            zero = 0;
            object->mode = zero;
            distance = Func_08004458() * 24 + 0x80000;
            Func_08096bec(object, distance, Func_08004458());
        }
    }

    Func_080090d0(anchors[0]);
    Func_080090d0(anchors[1]);
    Func_0809748c();
}
