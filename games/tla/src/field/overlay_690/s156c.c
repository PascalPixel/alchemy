#include "local.h"

struct Ov690Vector {
    s32 x;
    s32 y;
    s32 z;
};

struct Ov690Spawn {
    s32 unknown_00;
    s32 unknown_04;
    s32 scale_x;
    s32 scale_y;
    s32 speed;
    s32 acceleration;
    u16 kind;
    const u8 *script;
    s32 unknown_20;
    void *update;
};

extern const u8 Data_02002310[];

void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags, struct Ov690Spawn *spawn);

void Func_0200156c(s32 id)
{
    /* FAKEMATCH: unused; its twelve bytes are the gap the native frame keeps above the spawn record, where Func_02001620 holds its velocity. */
    struct Ov690Vector velocity;
    struct Ov690Spawn spawn;
    struct ObjectRuntime *object = Object_GetById(id);

    spawn.kind = 0x115;
    spawn.scale_x = 0xcccc;
    spawn.scale_y = 0xcccc;
    spawn.speed = 0x10000;
    spawn.acceleration = 0x10000;
    spawn.script = Data_02002310;
    Func_020021b0(154);
    Func_020000b8(object->x, object->y, object->z, 0, 0, 0, 0x1c0000, &spawn);
}
