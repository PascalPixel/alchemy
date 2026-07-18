typedef unsigned char u8;
typedef signed int s32;

struct Object_080a3480 {
    u8 padding[5];
    u8 value;
};

struct State_080a3480 {
    u8 padding[72];
    struct Object_080a3480 *objects[32];
};

extern struct State_080a3480 *Data_03001f2c;
s32 Func_080022fc(s32, s32);

void Func_080a3480(void)
{
    struct State_080a3480 *state = Data_03001f2c;
    s32 index = 0;
    s32 value = 13;
    struct Object_080a3480 **entry = state->objects;

    do {
        struct Object_080a3480 *object = *entry++;

        if (object != 0 && Func_080022fc(index, 5) == 0) {
            object->value = value;
        }
        index++;
    } while (index <= 31);
}
