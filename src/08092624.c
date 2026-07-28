#include "types.h"

struct Child_08092624 {
    u8 pad_00[9];
    u8 low_09 : 2;
    u8 copied_09 : 2;
    u8 high_09 : 4;
    u8 pad_0a[28];
    u8 field_26;
};

struct Object_08092624 {
    u8 pad_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad_14[28];
    s32 field_30;
    s32 field_34;
    u8 pad_38[24];
    struct Child_08092624 *child;
    u8 pad_54;
    u8 mode_55;
    u8 pad_56[14];
    u16 field_64;
    u8 pad_66[6];
    void (*callback_6c)(void);
};

extern struct Object_08092624 *Func_080090c8(s32, s32, s32, s32);
extern s32 Func_08004458(void);
extern void Func_08009080(struct Object_08092624 *, s32);
extern void Func_08009098(struct Object_08092624 *, const void *);
extern void Func_080929d8(struct Object_08092624 *);
extern s32 Func_08002304(s32, s32);

void Func_08092624(struct Object_08092624 *source, s32 optional)
{
    struct Object_08092624 *object;
    struct Child_08092624 *child;
    s32 value;

    object = Func_080090c8(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Func_08004458() & 1) {
        case 1:
            Func_08009080(object, 2);
            Func_08009098(object, (const void *)0x0809fbec);
            break;
        default:
            Func_08009080(object, 1);
            Func_08009098(object, (const void *)0x0809fc04);
            break;
        }

        if (optional != 0)
            Func_080929d8(object);

        object->mode_55 = 0;
        value = Func_08002304(Func_08004458(), 10) + 5;
        object->field_34 = -0x1999 * value;
        value = Func_08002304(Func_08004458(), 15) - 7;
        value <<= 1;
        object->field_30 = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = (void (*)(void))0x080925e1;
        child->field_26 = 0;
        child->copied_09 = source->child->copied_09;
    }
}
