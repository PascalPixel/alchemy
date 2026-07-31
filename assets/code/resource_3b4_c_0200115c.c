typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001ebc;

extern void Func_0200221a(s32);
extern u8 *Func_020036a8(s32);
extern void Func_02003750(void);

void Func_0200115c(void)
{
    u8 *slot;
    s16 *counter;
    s32 reset;

    Func_0200221a(3);
    slot = Func_020036a8(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        counter = (s16 *)(Data_03001ebc + 412);
        if (*counter > 12) {
            Func_02003750();
            reset = 0;
            *counter = reset;
        }
    }
}
