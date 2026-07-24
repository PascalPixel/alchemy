typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct Object_080a9d84 {
    u8 padding[6];
    s16 value1;
    s16 value2;
    u8 padding2[5];
    u8 flag;
};

struct State_080a9d84 {
    u8 padding[200];
    struct Object_080a9d84 *objects[32];
};

extern struct State_080a9d84 *Data_03001f2c;
void Func_080a17c4(struct Object_080a9d84 *);

void Func_080a9d84(void)
{
    struct State_080a9d84 *state = Data_03001f2c;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct Object_080a9d84 *object = state->objects[index];

        if (object != 0) {
            object->value1 = 248;
            object->value2 = 168;
            object->flag = 240;
            Func_080a17c4(object);
        }
    }
}
