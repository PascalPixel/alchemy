typedef unsigned char u8;
typedef unsigned int u32;
extern u8 Data_0000443c[];

void Func_080f7f30(u8 *destination)
{
    u32 countOffset = 0x4404;
    u8 **basePointer = (u8 **)0x02004c00;
    u8 *base = *basePointer;
    u32 index = 0;
    u32 *countAddress = (u32 *)(base + countOffset);
    u32 savedCountOffset = countOffset;
    u32 count = *countAddress;

    if (count != 0) {
        u32 displacement = (u32)Data_0000443c;
        u32 *offset = (u32 *)(base + displacement);
        u32 *countPointer;
        u8 *source;
        displacement -= 52;
        countAddress = (u32 *)displacement;
        countPointer = (u32 *)(base + savedCountOffset);
        source = base + (u32)countAddress;
        do {
            destination[*offset] = *source;
            (*offset)++;
            index = index + 1;
            source = source + 1;
        } while (index != *countPointer);
    }
}
