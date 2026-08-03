typedef unsigned int u32;
typedef unsigned char u8;

void Func_02000128(u32 index)
{
    const volatile u8 *map = (const volatile u8 *)0x02009940;
    u32 offset = index << 2;
    const u32 *source = (const u32 *)0x02009928;
    u32 *destination = (u32 *)0x02002224;
    u8 slot = map[index];
    u32 value = *(const u32 *)((const u8 *)offset + (u32)source);

    *(u32 *)((u8 *)((u32)slot << 2) + (u32)destination) = value;
}
