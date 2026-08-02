/* Store the caller's halfword in the scene workspace field at offset 220. */
typedef unsigned short u16;
typedef unsigned char u8;

void Func_020045d0(u16 value)
{
    u8 *workspace = *(u8 **)0x03001f3c;
    *(u16 *)(workspace + 220) = value;
}
