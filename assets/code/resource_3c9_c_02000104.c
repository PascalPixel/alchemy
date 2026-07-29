typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_02000104(s32 *w)
{
    w[2] = w[2] + w[17];
    w[3] = w[3] + w[18];
    w[4] = w[4] + w[19];
    w[6] = w[6] + w[12];
    w[7] = w[7] + w[13];
    {
        u16 *s = *(u16 **)(w + 20);
        s[15] = s[15] + *(u16 *)(w + 25);
    }
}
