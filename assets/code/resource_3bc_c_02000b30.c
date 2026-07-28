typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

struct Object {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x1c];
    s32 field30;
    s32 color;
};

extern s16 Data_02000240[];

extern struct Object *Func_02005556(s32);
extern void Func_02005440(struct Object *, s32);
extern void Func_02005486(struct Object *, s32, s32, s32);
extern void Func_02005494(struct Object *);
extern void Func_0200566e(s32, s32);
extern void Func_02005620(s32, s32, s32);

void Func_02000b30(void)
{
    struct Object *object;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    object = Func_02005556(*slot);
    if (object->x > 0x2980000) {
        object->x = 0x2980000;
    }
    object->color = 0x10000;
    object->field30 = 0x20000;
    Func_02005440(object, 5);
    z = object->z & 0xFFF00000;
    Func_02005486(object, object->x, object->y, z + 0xC0000);
    Func_02005494(object);
    Func_0200566e(*slot, 258);
    Func_02005620(*slot, 6, 0);
}
