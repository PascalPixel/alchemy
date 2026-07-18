typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08091858 {
    u8 padding[0x220];
    u16 values[2];
};

extern struct State_08091858 Data_02000240;
s32 Func_08091814(u16);

void Func_08091858(void)
{
    if (Func_08091814(Data_02000240.values[0]) != 0) {
        Data_02000240.values[0] = 0;
    }
    if (Func_08091814(Data_02000240.values[1]) != 0) {
        Data_02000240.values[1] = 0;
    }
}
