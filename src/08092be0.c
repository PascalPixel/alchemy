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

s32 Func_08092be0(s32 value)
{
    struct State *state = Data_03001ebc;
    s32 result = -1;
    s32 index = 8;
    struct Object *object = state->objects[index];

    if (object != 0 && object->active == 1 && *object->inner->value == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->objects[index];
            if (object == 0 || object->active != 1 ||
                *object->inner->value != value) {
                goto next;
            }
            result = index;
        }
    }
    return result;
}
