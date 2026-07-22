typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct State_0801a4c0 {
    u8 filler0[0x600];
    u16 first;
    u16 second;
    u32 value;
};

extern struct State_0801a4c0 *Data_03001e94;
extern u32 Data_080308a0[];
void Func_0801a5a4(struct State_0801a4c0 *, u32);

void Func_0801a4c0(u32 index)
{
    struct State_0801a4c0 *state = Data_03001e94;

    state->value = Data_080308a0[index];
    state->first = 2;
    state->second = 2;
    Func_0801a5a4(state, 0);
}
