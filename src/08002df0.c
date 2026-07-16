typedef unsigned int u32;
typedef unsigned char u8;

void Func_08002df0(u32 arg0)
{
    register u32 base asm("r4") = 0x03001e50;
    register u32 mask asm("r1") = 4;
    register u8 *address asm("r2") =
        (u8 *)((arg0 >> 22) & mask);
    *(u32 *)(address + base) = arg0;
}
