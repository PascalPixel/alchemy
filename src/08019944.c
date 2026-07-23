typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

u32 Func_08019944(u32 value, u32 clear)
{
    u32 nameOffset;
    u32 valueOffset;
    u8 *base;
    u16 name;
    u32 index;
    u32 result;
    u32 zero;

    base = *(u8 **)0x03001E8C;
    result = 0;
    index = 0;
    valueOffset = 0x12BC;
    zero = index;
    nameOffset = 0x12DC;
    name = *(u16 *)(nameOffset + (u32)base);
    if (name == value) {
        result = *(u32 *)((u32)valueOffset + (u32)base);
        if (clear != 0) {
            *(u32 *)((u32)valueOffset + (u32)base) = zero;
            *(u16 *)(nameOffset + (u32)base) = zero;
        }
    } else {
loop:
        index++;
        valueOffset += 4;
        nameOffset += 2;
        if (index <= 7) {
            if (*(u16 *)(nameOffset + (u32)base) == value) {
                result = *(u32 *)((u32)valueOffset + (u32)base);
                if (clear != 0) {
                    *(u32 *)((u32)valueOffset + (u32)base) = zero;
                    *(u16 *)(nameOffset + (u32)base) = zero;
                }
            } else {
                goto loop;
            }
        }
    }
    return result;
}
