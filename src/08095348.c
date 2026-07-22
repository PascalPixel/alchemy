typedef unsigned char u8;
typedef signed int s32;

struct Object_08095348 {
    u8 filler0[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x54];
    struct Object_08095348 *target;
};

void Func_08095348(struct Object_08095348 *object)
{
    struct Object_08095348 *target = object->target;

    object->x += (target->x - object->x) / 2;
    object->y += (target->y - object->y) / 2;
    object->z += (target->z - object->z) / 2;
}
