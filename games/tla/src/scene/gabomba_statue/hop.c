#include "types.h"
#include "object_runtime.h"

struct Orbiter {
    u8 unk00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[15];
    u8 flags23;
    u8 unk24[44];
    void **animation;
    u8 kind;
    u8 unk55[13];
    u8 phase;
    u8 unk63;
    s16 centerX;
    s16 centerZ;
    void *link;
};

extern s32 Data_02005af0;
struct Orbiter *Object_GetById(s32 id);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
struct Orbiter *Func_020005b4(struct Orbiter *origin, s32 id);
s32 Func_0200584c(void);
void Func_020057d4(struct ObjectRuntime *object);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
void Func_02005854(s32 sound);
void Func_0200565c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005664(struct ObjectRuntime *object);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void WaitFrames(s32 frames);

#define DIFF_WITHIN(a, b, limit) ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

void Func_020006ac(struct ObjectRuntime *object, s32 dx, s32 dz, s32 velocity)
{
    u8 *flags = &object->motionFlags;
    u8 saved;

    Func_020057d4(object);
    saved = *flags;
    Func_02005624(object, 6);
    WaitFrames(6);
    Func_02005854(152);
    Func_02005624(object, 7);
    object->speedLimit = 0x30000;
    object->acceleration = 0x20000;
    object->velocityY = velocity;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_0200565c(object, object->x + dx, object->y, object->z + dz);
    Func_02005664(object);
    *flags = saved;
    Func_02005624(object, 6);
    ObjectDispatch_SetSingleChildFlags(object, 1);
    WaitFrames(3);
}
