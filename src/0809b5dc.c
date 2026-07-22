typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s16 Data_02000240[];
extern u8 Value_00000001;

s32 Func_080022fc(s32, s32);
void Func_0809b450(void *);

void Func_0809b5dc(void *arg0) {
    s16 field64;
    s16 counter;

    field64 = *(s16 *)((u8 *)arg0 + 0x64);
    counter = (s16)((*(volatile u16 *)((u8 *)arg0 + 0x66))++);

    if (Data_02000240[237] == (s32)&Value_00000001) {
        if (Func_080022fc(counter, 7) == 0)
            Func_0809b450(arg0);
    } else if (Func_080022fc(counter, 5) == 0) {
        Func_0809b450(arg0);
    }

    if (field64 == 1)
        *(u16 *)((u8 *)arg0 + 6) += 0xC00;
}
