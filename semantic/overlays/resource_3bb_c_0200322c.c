typedef unsigned char u8;
typedef unsigned short u16;

extern u8 Data_02001000[];

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void Func_0200322c(void)
{
    *(u16 *)Data_02001000 = 9;
}
