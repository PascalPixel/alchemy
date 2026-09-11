#include "fixed_math.h"
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/particles/spawn_burst.h"

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

extern struct Object_08092624 *Battle_Run(s32, s32, s32, s32);
extern s32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
extern void Object_SetMode(struct Object_08092624 *, s32);
extern void Object_SetCallback(struct Object_08092624 *, const void *);
extern void ObjectGroup_SetChildValue(struct Object_08092624 *);

extern const u8 gRom[];
extern const u8 gRom2[];
extern u8 gRom3;

void BattleFx_SpawnBurstParticle(struct Object_08092624 *source, s32 optional)
{
    struct Object_08092624 *object;
    struct Child_08092624 *child;
    s32 value;

    object = Battle_Run(222, source->x, source->y, source->z);
    if (object != 0) {
        child = object->child;
        switch (Rand() & 1) {
        case 1:
            Object_SetMode(object, 2);
            Object_SetCallback(object, gRom);
            break;
        default:
            Object_SetMode(object, 1);
            Object_SetCallback(object, gRom2);
            break;
        }

        if (optional != 0)
            ObjectGroup_SetChildValue(object);

        object->mode_55 = 0;
        value = Battle_Apply(Rand(), 10) + 5;
        object->field_34 = -0x1999 * value;
        value = Battle_Apply(Rand(), 15) - 7;
        value <<= 1;
        object->field_30 = 0x1999 * value;
        object->field_64 = 0;
        object->callback_6c = (void (*)(void))&gRom3;
        child->field_26 = 0;
        child->copied_09 = source->child->copied_09;
    }
}
