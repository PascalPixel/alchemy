#include "types.h"

struct Sprite020003e8 {
    u8 unknown_00[18];
    s16 rotation;
    u8 unknown_14[6];
    u8 next_frame;
};

struct Actor020003e8 {
    u8 unknown_00[0x50];
    struct Sprite020003e8 *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[8];
    s16 unknown_5e;
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
        object->motion_flags = 0;
        /* FAKEMATCH: the empty asm ends the scheduling block: the native reads the fifth argument from the stack before it loads the zero for next_frame (ldr r3, [sp, #16]; ldr r1, =0), and without it the zero comes first (differ 2; a signed or unsigned field measures the same). */
        __asm__("");
        object->unknown_5e = fifth;
        sprite->next_frame = 0;
        sprite->rotation = value;
    }
}
