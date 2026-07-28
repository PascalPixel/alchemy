typedef signed int s32;
typedef unsigned char u8;

struct Object {
    u8 filler00[0x30];
    s32 field30;
    s32 color;
    u8 filler38[0x1d];
    u8 state;
};

extern struct Object *Func_02005462(s32);
extern void Func_02005340(struct Object *, s32);
extern void Func_02005350(struct Object *, s32);
extern void Func_02005426(s32);

void Func_02000a44(void)
{
    struct Object *object;
    s32 color;

    object = Func_02005462(30);
    object->state = 0;
    color = 0x19999;
    object->color = color;
    object->field30 = color;
    Func_02005340(object, 2);
    Func_02005350(object, 0x0200CC48);
    Func_02005426(0x363);
}
