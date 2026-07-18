typedef unsigned char u8;
typedef signed int s32;

extern u8 Data_02000240[];

void Func_0801c46c(s32 arg0)
{
    u8 value = Data_02000240[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    Data_02000240[0x205] = value;
}
