typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080cd418(void)
{
    u8 *src = *(u8 **)0x03001eec;

    *(u16 *)0x04000040 = *(u16 *)(src + 0x77bc); /* WIN0H */
    *(u16 *)0x04000044 = *(u16 *)(src + 0x77be); /* WIN0V */
    *(u16 *)0x04000042 = *(u16 *)(src + 0x77c0); /* WIN1H */
    *(u16 *)0x04000046 = *(u16 *)(src + 0x77c2); /* WIN1V */
    *(u16 *)0x04000048 = *(u16 *)(src + 0x77c4); /* WININ */
    *(u16 *)0x0400004a = *(u16 *)(src + 0x77c6); /* WINOUT */
    *(u16 *)0x04000000 = *(u16 *)(src + 0x77c8); /* DISPCNT */
    *(u16 *)0x04000050 = *(u16 *)(src + 0x77ca); /* BLDCNT */
    *(u16 *)0x04000052 = *(u16 *)(src + 0x77cc); /* BLDALPHA */
}
