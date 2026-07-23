typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Runtime {
    u8 pad_000[0x1C8];
    s32 field_1C8;
    s32 field_1CC;
    u8 pad_1D0[0xA];
    u16 field_1DA;
    s16 field_1DC;
    s16 field_1DE;
    u8 pad_1E0[0x14];
    s32 field_1F4;
    s32 field_1F8;
    u8 pad_1FC[0xABA];
    s16 field_CB6;
    u8 pad_CB8[0xA];
    s16 field_CC2;
    s16 field_CC4;
} Runtime;

extern Runtime Data_02000240;
extern Runtime *Data_03001ebc;

void Func_08015208(void);
void Func_08091660(void);
void Func_0808e118(void);
void Func_080041d8(const void *, s32);
u32 Func_080770d0(s32);

void Func_080916b0(void) {
    Runtime *state = Data_03001ebc;

    Func_08015208();
    Func_08091660();
    if (state->field_CB6 != 0) {
        Func_0808e118();
    }
    {
        s32 zero = 0;
        state->field_CC2 = zero;
        state->field_CC4 = zero;
        state->field_1C8 = 0x10;
        state->field_1CC = zero;
        state->field_1DA = 0xFFFF;
        state->field_1DC = -1;
        state->field_1DE = -1;
        Func_080041d8((const void *)0x080915ED, 0xC80);
        Func_080770d0(0x132);
        state->field_1F4 = Data_02000240.field_1F4;
        state->field_1F8 = zero;
    }
}
