typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

struct Other {
    u8 filler00[30];
    u16 x;
};

struct Object {
    u8 filler00[8];
    s32 x;
    s32 z;
    u8 filler10[64];
    struct Other *other;
};

extern s32 **Data_03001e70;
extern s32 Data_02009938[];
extern s32 Data_02009930[];

s32 Func_02000030(struct Object *object)
{
    s32 *position = *Data_03001e70;
    s32 *origin = Data_02009938;
    s32 *center = Data_02009930;
    s32 q0 = *position++;
    s32 q1 = *position;

    object->x = center[0] + (q0 - origin[0]);
    object->z = center[1] + (q1 - origin[1]) / 2;
    object->other->x += 0x600;
    return 0;
}
