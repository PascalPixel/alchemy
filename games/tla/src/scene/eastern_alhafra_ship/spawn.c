#include "types.h"

struct Sprite020003e8 {
    u8 unk00[18];
    s16 rotation;
    u8 unk14[6];
    u8 nextFrame;
};

struct Actor020003e8 {
    u8 unk00[0x50];
    struct Sprite020003e8 *sprite;
    u8 unk54;
    u8 motionFlags;
    u8 unk56[8];
    s16 unk5e;
};

extern const u8 Data_02003720[];

struct Actor020003e8 *Func_02003110(s32 type, s32 x, s32 y, s32 z);
void Func_02003100(struct Actor020003e8 *object, s32 mode);
void Func_02003108(struct Actor020003e8 *object, const void *script);

void Func_020003e8(s32 x, s32 y, s32 z, s16 value, s32 fifth)
{
    struct Actor020003e8 *object;
    struct Sprite020003e8 *sprite;

    object = Func_02003110(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Func_02003100(object, 1);
        Func_02003108(object, Data_02003720);
        object->motionFlags = 0;
        /* FAKEMATCH: the empty asm ends the scheduling block: the native reads the fifth argument from the stack before it loads the zero for nextFrame (ldr r3, [sp, #16]; ldr r1, =0), and without it the zero comes first (differ 2; a signed or unsigned field measures the same). */
        __asm__("");
        object->unk5e = fifth;
        sprite->nextFrame = 0;
        sprite->rotation = value;
    }
}
