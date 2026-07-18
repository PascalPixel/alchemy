typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct State_080a5534 {
    u8 padding[0x392];
    s16 values[2];
};

extern struct State_080a5534 *Data_03001f2c;
extern u8 Data_080aebcc[];
extern u8 Data_080aeb4c[];
s32 Func_08004080(void);
void Func_08003fa4(s32, s32, const u8 *);

void Func_080a5534(void)
{
    struct State_080a5534 *state = Data_03001f2c;
    s32 value = Func_08004080();

    state->values[0] = value;
    Func_08003fa4(value, 128, Data_080aebcc);
    value = Func_08004080();
    state->values[1] = value;
    Func_08003fa4(value, 128, Data_080aeb4c);
}
