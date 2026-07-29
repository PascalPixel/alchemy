typedef unsigned char u8;
typedef unsigned int u32;
extern u8 Data_0000443c[];

void Func_080f7f30(u8 *destination)
{
    u8 **basePointer = (u8 **)0x02004c00;
    u32 countOffset = 0x4404;
    u8 *base = *basePointer;
    u32 *countAddress = (u32 *)(base + countOffset);
    u32 count = *countAddress;
    u32 savedCountOffset = countOffset;
    u32 index = 0;

    if (count != 0) {
        u32 displacement = (u32)Data_0000443c;
        u32 *offset = (u32 *)(base + displacement);
        u32 *countPointer;
        u8 *source;
        displacement -= 52;
        countPointer = (u32 *)(base + savedCountOffset);
        countAddress = (u32 *)displacement;
        source = base + (u32)countAddress;
        do {
            destination[*offset] = *source;
            (*offset)++;
            index++;
            source++;
        } while (index != *countPointer);
    }
}
