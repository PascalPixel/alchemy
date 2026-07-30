typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

struct MoveTarget {
    u8 filler00[0x1e];
    u16 angle;
};

struct MoveObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[4];
    s32 a;
    s32 b;
    u8 filler20[0x10];
    s32 va;
    s32 vb;
    u8 filler38[0xc];
    s32 vx;
    s32 vy;
    s32 vz;
    struct MoveTarget *target;
    u8 filler54[0x10];
    u16 spin;
};

extern s32 Func_0200572a(s32, s32);

void Func_0200096c(struct MoveObject *object)
{
    s32 vx;
    s32 vz;
    s32 vy;

    vx = object->vx;
    object->x += vx;
    vy = object->vy;
    object->y += vy;
    vz = object->vz;
    object->z += vz;
    object->vx = vx - Func_0200572a(vx, 18);
    object->vz = vz - vz / 16;
    object->a += object->va;
    object->b += object->vb;
    object->target->angle += object->spin;
}
