typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_0801c304 {
    u8 filler0[0x39e];
    u16 value;
    u8 filler3a0[0x18];
    u16 active;
};

extern struct State_0801c304 *Data_03001e98;
void Func_0801a7f4(u32);
void Func_0801b228(void);
void Func_0801b010(u32, u32);
void Func_0801a968(void);
u32 Func_0801b424(u32);
void Func_0801b148(void);

u32 Func_0801c304(u32 value)
{
    struct State_0801c304 *state = Data_03001e98;
    u32 result;

    state->value = value;
    state->active = 1;
    Func_0801a7f4(value);
    Func_0801b228();
    Func_0801b010(0, 5);
    Func_0801a968();
    result = Func_0801b424(1);
    Func_0801b148();
    return result;
}
