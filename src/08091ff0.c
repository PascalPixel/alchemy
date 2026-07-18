typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct State_08091ff0 {
    u8 padding[0xcc8];
    s16 value;
};

extern struct State_08091ff0 *Data_03001ebc;
void Func_080f9010(s32);

void Func_08091ff0(s32 value)
{
    Data_03001ebc->value = value;
    if ((s16)value == -1) {
        value = 0x121;
    }
    Func_080f9010(0x12a);
    Func_080f9010(value);
}
