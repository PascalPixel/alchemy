typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Inner {
    u8 unknown_000[40];
    s16 *value;
};

struct Object {
    u8 unknown_000[80];
    struct Inner *inner;
    u8 active;
};

struct State {
    u8 unknown_000[20];
    struct Object *objects[4096];
};

extern struct State *Data_03001ebc;

s32 Func_08092ba8(s32 key)
{
    s32 result = -1;
    struct Object *object = Data_03001ebc->objects[key & 0x0fff];

    if (object != 0 && object->active == 1) {
        result = *object->inner->value;
    }
    return result;
}
