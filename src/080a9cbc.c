typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Object_080a9cbc {
    u8 padding[6];
    s16 value1;
    s16 value2;
};

struct State_080a9cbc {
    u8 padding[72];
    struct Object_080a9cbc *objects[32];
};

extern struct State_080a9cbc *Data_03001f2c;
void Func_080a17c4(struct Object_080a9cbc *);

void Func_080a9cbc(void)
{
    struct State_080a9cbc *state = Data_03001f2c;
    s32 value1 = 248;
    struct Object_080a9cbc **entry = state->objects;
    s32 value2 = 168;
    s32 remaining = 31;

    do {
        struct Object_080a9cbc *object = *entry++;

        if (object != 0) {
            object->value1 = value1;
            object->value2 = value2;
            Func_080a17c4(object);
        }
        remaining--;
    } while (remaining >= 0);
}
