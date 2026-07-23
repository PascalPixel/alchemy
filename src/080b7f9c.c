typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct State_080b7f9c {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Local_080b7f9c {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080b7f9c *Data_03001e80;

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080072f0(struct Local_080b7f9c *, struct State_080b7f9c *);

void Func_080b7f9c(void) {
    struct State_080b7f9c *state = Data_03001e80;
    struct Local_080b7f9c local;

    state->field36 = 192 << 6;
    state->field34 = 254 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field1c = 0;
    state->field18 = 0;

    Func_080049ac();
    Func_08004cb4(&state->field0c);
    Func_08004c1c(state->field36);
    Func_08004bd4(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    ((void (*)(struct Local_080b7f9c *, struct State_080b7f9c *))0x03000250)(&local, state);
}
