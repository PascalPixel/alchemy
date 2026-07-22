typedef unsigned char u8;
typedef unsigned short u16;

struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct Data_08091e6c {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c * volatile Data_03001ebc;
extern struct Data_08091e6c Data_02000240;

void Func_08091e6c(u16 first, u16 second)
{
    struct State_08091e6c *state = Data_03001ebc;
    state->value = 999;
    Data_02000240.first = first;
    Data_02000240.second = second;
}

void Func_08091e9c(u16 value)
{
    struct State_08091e6c *state = Data_03001ebc;
    state->value = value;
}
