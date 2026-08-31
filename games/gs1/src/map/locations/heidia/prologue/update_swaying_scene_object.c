#include "types.h"

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

extern s32 Func_02005e42(s32 arg0);
extern s32 Func_02005e58(s32 arg0);
extern s32 Func_02005e72(s32 arg0);
extern s32 Func_02005e70(void);
extern s32 Func_02005e76(void);

#define UpdateSwayingSceneObject Func_02002e70

s32 UpdateSwayingSceneObject(struct S *object) {
    struct T *sprite = object->f50;
    s32 vertical_offset = Func_02005e42(object->f30) * 2;
    s32 random_b;
    s32 random_a;

    if (vertical_offset > 0) {
        vertical_offset = -vertical_offset;
    }
    object->f08 = object->f38 + Func_02005e58(object->f30) * 2;
    object->f0c = object->f3c + vertical_offset;
    sprite->f1e = (u16)(Func_02005e72(object->f30 + 0x8000) / 8);
    random_a = Func_02005e70();
    random_b = Func_02005e76();
    object->f30 += (((u32)(random_a << 9)) >> 16) + (((u32)(random_b << 9)) >> 16) + 0x400;
    return 0;
}
