typedef unsigned int u32;
typedef signed int s32;

extern u32 Data_08037250[];

void Func_0800730c(void *, s32, u32);

void Func_08021848(void)
{
    s32 group;
    s32 row;
    s32 column;
    s32 index;
    u32 *block;
    u32 *word;

    group = 0;
    do {
        row = 0;
        block = (u32 *)(0x06006280 + group * 0x180);
        do {
            Func_0800730c(block, 64, 0x44444444);
            column = 1;
            word = block + 1;
            do {
                index = row;
                if (group != 1 || column > 1) {
                    if (group == 0 && row > column - 2) {
                        index = column - 2;
                        if (index < 0)
                            index = 0;
                    }
                    word[0] ^= Data_08037250[index * 2];
                    word[8] ^= Data_08037250[index * 2 + 1];
                }
                column++;
                word++;
            } while (column <= 7);
            row++;
            block += 16;
        } while (row <= 5);
        group++;
    } while (group <= 1);
}
