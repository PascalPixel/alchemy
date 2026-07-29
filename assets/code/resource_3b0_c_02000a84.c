typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

extern u8 *Func_02001cbe(void);
extern void Func_02001d38(s32, s32);
extern u32 Func_02001c64(void);
extern u32 Func_02001c70(void);
extern u32 Func_02001c7c(void);
extern u32 Func_02001c8e(void);
extern void Func_02001d2c(s32, void *);
extern s32 Data_020093a4[];

void Func_02000a84(s32 a)
{
    u8 *obj;
    u32 x;

    obj = Func_02001cbe();
    Func_02001d38(a, 1);
    obj[0x55] = 0;
    *(u16 *)(obj + 0x64) = Func_02001c64() >> 15;
    *(u16 *)(obj + 0x66) = Func_02001c70() >> 15;
    x = Func_02001c7c();
    x <<= 2;
    x >>= 16;
    x <<= 16;
    x += 0x60000;
    *(s32 *)(obj + 0xc) = x;
    x = Func_02001c8e();
    *(s32 *)(obj + 0x4c) = ((x * 3 << 13) >> 16) - 0x3000;
    *(s32 *)(obj + 0x18) = 0x14000;
    *(s32 *)(obj + 0x1c) = 0x14000;
    Func_02001d2c(a, Data_020093a4);
}
