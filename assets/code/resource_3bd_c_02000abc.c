typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

extern u8 Data_02001004;
extern void Func_02001528(void);

void Func_02000abc(s32 arg0)
{
    u8 *state;
    u8 *d = &Data_02001004;

    state = *(u8 **)0x03001ebc;
    *d = arg0;
    if (*(s16 *)(state + 0xcb8) == 0) {
        Func_02001528();
    }
}
