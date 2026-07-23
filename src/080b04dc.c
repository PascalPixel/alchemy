typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

s32 Func_0808a540(s32);
void Func_08015140(void);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_080030f8(s32);
extern u8 Data_00000c9b[];
extern u8 Data_00000cc6[];
extern u8 Data_00000cf1[];
extern u8 Data_00000d4c[];

void Func_080b04dc(s32 arg0)
{
    s32 *state = *(s32 **)0x03001f2c;
    s32 value = Func_0808a540(*(u16 *)&state[233]);
    s32 result = arg0;
    s8 mode;

    Func_08015140();
    mode = *(s8 *)((u8 *)state + 0x3a9);
    if (mode == 2)
        result += Data_00000cc6 - Data_00000c9b;
    if (mode == 0)
        result += Data_00000cf1 - Data_00000c9b;
    if (*(s8 *)&state[235] != 0)
        result += Data_00000d4c - Data_00000c9b;
    Func_08015038(result, 5, 0, (value << 16) | 0x22);
    while (Func_08015048() == 0)
        Func_080030f8(1);
    Func_080030f8(1);
}
