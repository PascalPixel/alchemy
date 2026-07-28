typedef signed int s32;
typedef unsigned char u8;

typedef struct Object {
    u8 pad00[8];
    s32 x;
    u8 pad0C[4];
    s32 y;
    u8 pad14[4];
    s32 scaleX;
    s32 scaleY;
    u8 pad20[0x10];
    s32 field30;
    s32 field34;
} Object;

void Func_02004bae(Object *, s32, s32, s32);
void Func_02004bc4(Object *, s32, s32, s32);
void Func_02004c0e(s32, s32, s32, s32, s32, s32);
void Func_02004c20(s32, s32, s32, s32, s32, s32);
void Func_02004c72(s32);
Object *Func_02004c94(s32);
Object *Func_02004ca2(s32);
Object *Func_02004cbc(s32);

void Func_02000274(void)
{
    Object *object;

    object = Func_02004c94(9);
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;

    object = Func_02004ca2(11);
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_02004bae(object, object->x, 0x200000, object->y);

    object = Func_02004cbc(10);
    object->field34 = 0x6666;
    object->field30 = 0xCCCC;
    Func_02004bc4(object, object->x, 0x40000, object->y);

    Func_02004c72(0x362);
    Func_02004c0e(15, 12, 1, 1, 13, 12);
    Func_02004c20(14, 12, 1, 1, 9, 12);
}
