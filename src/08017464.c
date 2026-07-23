typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

struct State_08017464 {
    u8 filler0[0xEA8];
    u16 ten;
    u16 unusedEaa;
    u16 zero;
    u16 fifteen;
    u8 fillerEb0[0x400];
    u16 nine;
    u16 secondZero;
    u8 filler12b4[4];
    u16 result;
};

extern struct State_08017464 *Data_03001e8c;

s32 Func_08003fa4(s32, s32, s32);
void Func_080041d8(void *, s32);

void Func_08017464(s32 initialize) {
    struct State_08017464 *state = Data_03001e8c;
    s32 transferSize;

    if (initialize != 0)
        state->result = Func_08003fa4(95, 128 << 6, 0);

    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->secondZero = 0;
    transferSize = 200;
    transferSize <<= 4;
    Func_080041d8((void *)0x0801789D, transferSize);
}
