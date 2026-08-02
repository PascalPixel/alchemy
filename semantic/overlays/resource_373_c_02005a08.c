/* Play sound 0x83 only while the low runtime nibble is clear. */
typedef unsigned int u32;

void Func_080f9010(int sound);

void Func_02005a08(void)
{
    if ((*(u32 *)0x03001e40 & 15) == 0)
        Func_080f9010(0x83);
}
