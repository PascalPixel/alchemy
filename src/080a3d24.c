typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Object_080a3d24 {
    u8 padding[5];
    u8 field5;
};

struct State_080a3d24 {
    u8 padding[72];
    struct Object_080a3d24 *objects[32];
};

extern struct State_080a3d24 *Data_03001f2c;
void Func_080a17c4(struct Object_080a3d24 *);

void Func_080a3d24(u16 *arg0)
{
    struct State_080a3d24 *state = Data_03001f2c;
    s32 i;

    for (i = 0; i < 32; i++) {
        if (arg0[i] == 0) {
            Func_080a17c4(state->objects[i]);
            state->objects[i]->field5 = 13;
        }
    }
}
