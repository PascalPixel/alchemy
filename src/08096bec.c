typedef unsigned char u8;
typedef signed int s32;

struct Object_08096bec {
    u8 padding[8];
    s32 x;
    s32 y;
    s32 z;
};

void Func_0800447c(s32, s32, s32 *);
void Func_08009150(struct Object_08096bec *, s32, s32, s32);

void Func_08096bec(struct Object_08096bec *object, s32 arg1, s32 arg2)
{
    s32 values[3];

    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        Func_0800447c(arg1, arg2, values);
        Func_08009150(object, values[0], values[1], values[2]);
    }
}
