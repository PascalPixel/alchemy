typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Object {
    u8 filler00[6];
    u16 attributes;
    s32 x;
    u8 filler0C[4];
    s32 z;
};

extern s16 Data_02000240[];

extern struct Object *Func_0200507e(s32);
extern void Func_02004ffe(s32, s32, s32, s32);

void Func_02000658(void)
{
    struct Object *object;
    s16 *table;
    s32 x;
    s32 z;
    s32 message;

    table = Data_02000240;
    object = Func_0200507e(*(s32 *)&table[250]);
    x = object->x >> 20;
    message = 23;
    z = object->z >> 20;
    if (x == 81 && z == 12) {
        if ((object->attributes & 0xE000) == 0x4000) {
            message = 253;
        }
        Func_02004ffe(0, x << 20, z << 20, message);
    }
}
