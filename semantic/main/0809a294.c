typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Position_0809a294 {
    s32 x;
    s32 y;
    s32 z;
};

struct Runtime_0809a294 {
    u32 field0;
    s32 x;
    s32 y;
    s32 z;
};

struct Object_0809a294 {
    u8 pad00[6];
    u16 field06;
    u8 pad08[40];
    s32 field30;
    s32 field34;
    u8 pad38[29];
    u8 field55;
};

extern struct Runtime_0809a294 *Data_03001f30;

void Func_08097384(void);
struct Object_0809a294 *Func_0809a3c4(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_08096bec(struct Object_0809a294 *, s32, s32);
void Func_08009158(struct Object_0809a294 *);
void Func_080f9010(s32);
struct Object_0809a294 *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(struct Object_0809a294 *, const void *);
s32 Func_08004458(void);
void Func_080090d0(struct Object_0809a294 *);
void Func_0809748c(void);

void Func_0809a294(void)
{
    struct Runtime_0809a294 *runtime = Data_03001f30;
    struct Position_0809a294 position;
    struct Object_0809a294 *objects[2];
    s32 index;

    Func_08097384();
    position.x = runtime->x;
    position.y = runtime->y + 0x100000;
    position.z = runtime->z;
    objects[0] = Func_0809a3c4(position.x + 0x200000,
        position.y, position.z, 0x10000);
    objects[1] = Func_0809a3c4(position.x - 0x200000,
        position.y, position.z, 0);

    Func_080030f8(15);
    for (index = 1; index >= 0; index--) {
        struct Object_0809a294 *object = objects[index];
        if (object != 0)
            Func_08096bec(object, 0x180000, object->field06);
    }

    Func_08009158(objects[0]);
    Func_080f9010(134);

    for (index = 23; index >= 0; index--) {
        struct Object_0809a294 *object;
        s32 random;
        s32 velocity;

        position.x = runtime->x;
        position.y = runtime->y + 0x100000;
        position.z = runtime->z;
        object = Func_08096c80(0x11D, position.x, position.y, position.z);
        if (object != 0) {
            Func_08009098(object, (const void *)0x0809F0D4);
            random = Func_08004458();
            object->field34 = 0x10000;
            object->field30 = random + 0x10000;
            object->field55 = 0;
            velocity = Func_08004458();
            random = Func_08004458();
            Func_08096bec(object, 0x80000 + velocity * 24, random);
        }
    }

    Func_080090d0(objects[0]);
    Func_080090d0(objects[1]);
    Func_0809748c();
}
