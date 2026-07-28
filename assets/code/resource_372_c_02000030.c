typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[92];
    s16 counter;
};

extern s32 Func_02004698(void);
extern s32 Func_0200469e(void);
extern s32 Func_02004684(s32, s32);

s32 Func_02000030(struct Object *object)
{
    s32 loaded = object->counter;
    s32 counter = (s16)loaded;

    if (loaded == 0) {
        object->x = Func_02004698();
        counter = Func_02004684(Func_0200469e(), 20) + 20;
        object->counter = counter;
    }
    object->counter = counter - 1;
    return 1;
}
