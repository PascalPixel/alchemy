#include "types.h"

struct Position_08097b70 {
    s32 x;
    s32 y;
    s32 z;
};

struct Source_08097b70 {
    u8 pad_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[70];
    u8 flag_5a;
    u8 pad_5b[13];
    struct Source_08097b70 *target;
};

struct Object_08097b70 {
    u8 pad_00[72];
    s32 field_48;
    u8 pad_4c[9];
    u8 mode_55;
    u8 pad_56[8];
    u16 field_5e;
};

extern s32 Func_080044d0(s32, s32);
extern u32 Func_08004458(void);
extern void Func_0800447c(s32, s32, struct Position_08097b70 *);
extern struct Object_08097b70 *Func_08096c80(s32, s32, s32, s32);
extern void Func_08009080(struct Object_08097b70 *, s32);
extern void Func_08009098(struct Object_08097b70 *, const void *);

void Func_08097b70(struct Source_08097b70 *source)
{
    struct Source_08097b70 *target;
    struct Position_08097b70 position;
    struct Object_08097b70 *object;
    s32 turn;
    s32 magnitude;

    target = source->target;
    if (target != 0) {
        s32 xDelta = target->x - source->x;
        s32 zDelta = target->z - source->z;

        if (xDelta != 0 || zDelta != 0) {
            turn = (s16)(Func_080044d0(zDelta, xDelta) - source->angle);
            if (turn > 0x1000)
                turn = 0x1000;
            if (turn < -0x1000)
                turn = -0x1000;
            source->angle += turn;
        }
        source->flag_5a = 0;
    }

    position.x = source->x;
    position.y = source->y - (Func_08004458() << 4) - 0x80000;
    position.z = source->z;
    magnitude = Func_08004458() * 3;
    magnitude <<= 4;
    Func_0800447c(magnitude, Func_08004458(), &position);

    object = Func_08096c80(0x11D, position.x, position.y, position.z);
    if (object != 0) {
        object->mode_55 = 2;
        object->field_48 = 0x1999;
        Func_08009080(object, 0);
        object->field_5e = 12;
        Func_08009098(object, (const void *)0x0809F0B0);
    }
}
