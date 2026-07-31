typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Slot020008e0 {
    u8 head[6];
    u16 heading;
};

extern struct Slot020008e0 *Func_02001dee(s32);
extern void Func_02001ddc(void);
extern void Func_02001f0c(s32);
extern void Func_02001e84(s32);
extern void Func_02001e9c(s32, s32);
extern void Func_02001e08(void);

void Func_020008e0(void)
{
    u32 heading = Func_02001dee(0)->heading;

    Func_02001ddc();
    if (heading - 0xA001 <= 0x3FFE) {
        Func_02001f0c(15);
    } else {
        Func_02001e84(0x1A1E);
        Func_02001e9c(15, 0);
    }
    Func_02001e08();
}
