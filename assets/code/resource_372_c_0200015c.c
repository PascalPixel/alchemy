typedef unsigned char u8;
typedef signed int s32;

extern s32 Func_02004840(s32);
extern void Func_020049c8(void);
extern void Func_020049c4(u8 *);
extern u8 Value_00000834;

void Func_0200015c(u8 *o)
{
    u8 *state;

    if (Func_02004840((s32)&Value_00000834) != 0) {
        Func_020049c8();
    }
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x100;
    *(s32 *)(state + 0x1C8) = 16;
    Func_020049c4(o);
}
