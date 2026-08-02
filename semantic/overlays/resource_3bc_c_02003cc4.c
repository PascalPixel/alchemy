typedef unsigned char u8;
typedef unsigned short u16;

extern u8 Data_02001000[];

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Func_02003cc4(void)
{
    *(u16 *)Data_02001000 = 9;
}
