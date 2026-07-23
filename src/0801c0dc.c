typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Object_0801c0dc {
    u8 filler0[5];
    u8 field_50 : 2;
    u8 field_52 : 2;
    u8 field_54 : 1;
    u8 field_55 : 1;
    u8 field_56 : 2;
    u8 filler6;
    u8 field_70 : 1;
    u8 field_71 : 5;
    u8 field_76 : 2;
    u16 field_80 : 10;
    u16 field_8a : 2;
    u16 field_8c : 4;
};

s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, const void *);

void Func_0801c0dc(struct Object_0801c0dc *object, s32 *slot)
{
    const void *data = (const void *)0x080342f8;
    s32 value = Func_08004080();

    *slot = value;
    object->field_80 = Func_08003fa4(value, 0x80, data);
    object->field_52 = 0;
    object->field_54 = 0;
    object->field_55 = 1;
    object->field_50 = 0;
    object->field_71 = 0;
    object->field_76 = 0;
    object->field_56 = 2;
    object->field_8a = 0;
}
