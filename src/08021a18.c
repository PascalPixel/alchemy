typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u16 Data_06000600[];
extern u16 Data_080372c0[];

void Func_08021a18(u16 *destination)
{
    s32 bank = 0;
    s32 paletteOffset = 0;
    s32 destinationBank = 0;

    do {
        s32 row = 0;
        s32 destinationOffset = (destinationBank + bank) << 6;

        do {
            u16 *output = (u16 *)((u8 *)destination + destinationOffset);
            u16 *source = (u16 *)((u8 *)Data_06000600 + (row << 5));
            s32 column = 0;

            do {
                u32 packed = *source++;
                u32 decoded = 0;
                s32 nibble = 0;

                do {
                    u32 color = Data_080372c0[
                        (packed & 15) + paletteOffset
                    ] << (nibble * 4);
                    nibble++;
                    packed >>= 4;
                    decoded |= color;
                } while (nibble <= 3);

                column++;
                *output++ = decoded;
            } while (column <= 15);

            row++;
            destinationOffset += 32;
        } while (row <= 9);

        paletteOffset += 16;
        destinationBank += 4;
        bank++;
    } while (bank <= 1);
}
