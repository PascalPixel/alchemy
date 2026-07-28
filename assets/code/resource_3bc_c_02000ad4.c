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

extern struct Object *Func_020054fa(s32);
extern void Func_020055e2(s32, s32);
extern void Func_020053e2(struct Object *, s32);
extern void Func_02005428(struct Object *, s32, s32, s32);
extern void Func_02005436(struct Object *);

void Func_02000ad4(void)
{
    struct Object *object;
    s16 *table;
    s32 *slot;
    s32 z;

    table = Data_02000240;
    slot = (s32 *)&table[250];
    object = Func_020054fa(*slot);
    object->color = 0x10000;
    object->field30 = 0x20000;
    Func_020055e2(*slot, 258);
    Func_020053e2(object, 5);
    z = object->z & 0xFFF00000;
    Func_02005428(object, object->x, object->y, z + 0x180000);
    Func_02005436(object);
}
