typedef unsigned char u8;
typedef unsigned short u16;

struct State_08091fa8 {
    u8 filler0[0x170];
    u16 value;
};

struct Data_08091fa8 {
    u8 filler0[0x1C0];
    u16 earlyFirst;
    u16 earlySecond;
    u8 filler1c4[14];
    u16 lateFirst;
    u16 lateSecond;
};

extern struct State_08091fa8 * volatile Data_03001ebc;
extern struct Data_08091fa8 Data_02000240;

void Func_08091fa8(u16 first, u16 second)
{
    Data_02000240.lateFirst = first;
    Data_02000240.lateSecond = second;
}

void Func_08091fc0(u16 first, u16 second)
{
    struct State_08091fa8 *state = Data_03001ebc;
    Data_02000240.earlyFirst = first;
    Data_02000240.earlySecond = second;
    state->value = 999;
}
