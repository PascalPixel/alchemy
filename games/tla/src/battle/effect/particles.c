#include "types.h"
#include "system.h"
#include "objdisp.h"
#include "object_effect_scripts.h"
#include "object_runtime.h"
#include "fx_scene.h"
#include "ram_buffer.h"

struct FallVector
{
    s32 x;
    s32 y;
    s32 z;
};

struct ParticleTimer
{
    u8  unk00[0x5e];
    u16 timer;
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void                  Vector_AddPolarOffset(s32 magnitude, s32 angle, struct FallVector *pos);

// Each frame: one falling particle 8 to 24 pixels above the lifted object,
// 0 to 48 pixels out in a random direction.
void Func_080df174(void)
{
    struct ObjectRuntime *src;
    struct FallVector     pos;
    struct ObjectRuntime *obj;
    u32                   dist;

    src = ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->child;
    pos.x = src->x;
    pos.y = src->y - Random16() * 16 + (24 << 16);
    pos.z = src->z;

    dist = Random16() * 3;
    Vector_AddPolarOffset(dist * 16, Random16(), &pos);

    obj = Object_Spawn(673, pos.x, pos.y, pos.z);
    if (obj != 0)
    {
        obj->motionFlags = 2;
        obj->gravity = 0x1999;
        Object_SetMode(obj, 0);
        ((struct ParticleTimer *)obj)->timer = 12;
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj, gBattleFx_CommonParticleScript);
    }
}
