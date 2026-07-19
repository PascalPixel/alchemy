typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

typedef struct {
    s32 value;
    s16 halfword;
    u8 padding06[81];
    u8 byte87;
    u8 padding88[3];
    u8 byte91;
    u8 padding92;
    u8 byte93;
} Object;

void Func_0800c2d8(Object *object, s32 value)
{
    if (object != 0) {
        object->halfword = 0;
        object->value = value;
        object->byte91 = 0;
        object->byte93 = 0;
        object->byte87 = 0;
    }
}
