typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct Object_080a51d0 {
    u8 padding0[5];
    u8 enabled;
    u16 first;
    u16 second;
    u8 padding10[4];
    u8 value;
};

struct State_080a51d0 {
    u8 padding0[268];
    void *target;
    u8 padding272[104];
    u16 field376;
    u8 padding378[160];
    u8 index;
    u8 padding539;
    struct Object_080a51d0 * volatile object;
};

extern struct State_080a51d0 *Data_03001f2c;
extern u8 Value_00000182;
extern void Func_08015298(s32, s32, s32, s32);
extern void Func_080a17c4(struct Object_080a51d0 *);
extern s32 Func_08077008(s32);
extern void Func_08015090(s32, void *, s32, s32);
extern void Func_08015080(s32, void *, s32, s32);

void Func_080a51d0(void)
{
    struct State_080a51d0 *state = Data_03001f2c;

    Func_08015298(2, state->field376, state->object->value, 0);
    state->object->enabled = 1;
    state->object->first = 112;
    state->object->second = 8;
    Func_080a17c4(state->object);
    Func_08015090(Func_08077008(state->index), state->target, 16, 0);
    Func_08015080(
        (state->field376 & 0x1FF) + (s32)&Value_00000182,
        state->target,
        16,
        8);
}
