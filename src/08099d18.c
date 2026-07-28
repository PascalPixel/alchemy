#include "types.h"
#include "object_effect.h"

struct Vector_08099d18 {
    s32 x;
    s32 y;
    s32 z;
};

struct Source_08099d18 {
    u8 padding[8];
    s32 x;
    s32 y;
    s32 z;
};

struct State_08099d18 {
    u8 padding[20];
    struct Source_08099d18 *source;
};

#define STATE_08099D18_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char Vector_08099d18_size[
    sizeof(struct Vector_08099d18) == 0x0c ? 1 : -1
];
typedef char Source_08099d18_x_offset[
    STATE_08099D18_OFFSET(struct Source_08099d18, x) == 8 ? 1 : -1
];
typedef char State_08099d18_source_offset[
    STATE_08099D18_OFFSET(struct State_08099d18, source) == 0x14 ? 1 : -1
];
#undef STATE_08099D18_OFFSET

extern struct State_08099d18 *Data_03001f30;
extern u8 Data_0809f0b0[];

u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Vector_08099d18 *);
void Func_08009080(u8 *, s32);
void Func_08009098(u8 *, void *);

void Func_08099d18(void)
{
    struct Source_08099d18 *source;
    struct Vector_08099d18 position;
    u8 *object;
    u32 angle;
    u16 *timer;
    s32 timer_value;

    source = Data_03001f30->source;
    position.x = source->x;
    position.y = (s32)((u32)source->y -
        Func_08004458() * 16 + 0x180000);
    position.z = source->z;

    angle = Func_08004458() * 3;
    Func_0800447c((s32)(angle * 16), Func_08004458(), &position);

    object = Func_08096c80(
        0x11d,
        position.x,
        position.y,
        position.z);
    if (object != 0) {
        object[85] = 2;
        *(s32 *)(object + 72) = 0x1999;
        Func_08009080(object, 0);
        timer = (u16 *)(object + 94);
        timer_value = 12;
        *timer = timer_value;
        Func_08009098(object, Data_0809f0b0);
    }
}
