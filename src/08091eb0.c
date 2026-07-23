typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct State_08091eb0 {
    u8 padding0[380];
    u16 first;
    u8 padding1[32];
    s16 mode;
};

struct Work_08091eb0 {
    u8 padding0[470];
    u16 special;
    u8 padding1[28];
    void *object;
};

extern struct State_08091eb0 *Data_03001ebc;
extern struct Work_08091eb0 Data_02000240;
extern u8 Value_00000021;
s32 Func_0808b05c(s32, s32);
s32 Func_0808ba1c(void *);
void Func_0808adf0(s32);
void Func_0808b320(s32, s32);

void Func_08091eb0(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct State_08091eb0 *state = Data_03001ebc;

    state->first = Func_0808b05c(first, second);
    if (first == 98 && second == 0)
        Data_02000240.special = (u16)(s32)&Value_00000021;
    if (state->mode == 3)
        Func_0808adf0(Func_0808ba1c(Data_02000240.object) + 8);
    Func_0808b320(first, second);
}
