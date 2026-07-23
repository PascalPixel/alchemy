typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_0808a540(u16);
void Func_08015140(void);
s32 Func_080b2884(s32);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_080030f8(u32);

extern void *Data_03001f2c;

struct State080b2928 {
    u8 padding0[0x380];
    void *modeState;
    u8 padding1[0x20];
    u16 value;
};

void Func_080b2928(s32 arg0)
{
    struct State080b2928 *state;
    void **modeSlot;
    s32 value;
    u8 mode;

    state = Data_03001f2c;
    modeSlot = &state->modeState;
    mode = *(u8 *)((u8 *)*modeSlot + 5);
    value = Func_0808a540(state->value);
    arg0 = Func_080b2884(arg0);
    *(u8 *)((u8 *)*modeSlot + 5) = 13;
    Func_08015140();
    Func_08015038(arg0, 5, 0, (value << 16) | 0x22);
    while (Func_08015048() == 0)
        Func_080030f8(1);
    Func_080030f8(1);
    *(u8 *)((u8 *)state->modeState + 5) = mode;
}
