typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern void Func_02001838(s32 strength);
extern u16 Func_020017d0(u16 colour, s32 strength);
extern void Func_02001878(void);
extern void Func_02001858(void);
extern void Func_0808a330(s32 value, s32 mode);

void Func_02001768(s32 strength)
{
    s32 phase;

    Func_02001838(strength);

    for (phase = 0; (u32)phase <= 0x00df0000; phase += 0x10000) {
        s32 index = (u32)phase >> 16;

        if ((u32)(phase + (s32)0xffef0000) > 0x60000 &&
            (u32)((index + 0xff3f) << 16) > 0x70000) {
            volatile u16 *palette = (volatile u16 *)0x05000000;
            palette[index] = Func_020017d0(palette[index], strength);
        }
    }

    Func_02001878();
    Func_02001858();
    Func_0808a330(0x10000, 0);
}
