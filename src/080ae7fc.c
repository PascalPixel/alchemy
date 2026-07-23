typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_08077008(s32);

extern u8 *Data_03001f2c;

void Func_080ae7fc(u8 *output)
{
    u8 *state;
    u16 *ids;
    u8 *object;
    u32 *row;
    s32 outer;
    s32 rowIndex;
    s32 bit;
    s32 active;
    s32 count;
    u32 mask;
    u32 one;

    state = Data_03001f2c;
    outer = 0;
    if (outer < state[0x219]) {
        one = 1;
        ids = (u16 *)(state + 0x208);
        do {
            object = Func_08077008(*ids);
            rowIndex = 0;
            count = 0;
            row = (u32 *)(object + 0xf8);
            do {
                active = row[4];
                bit = 0;
                do {
                    mask = one << bit;
                    if (active & mask)
                        count++;
                    else if (row[0] & mask)
                        count++;
                    bit++;
                } while (bit <= 19);
                rowIndex++;
                row++;
            } while (rowIndex <= 3);
            output[outer] = count;
            outer++;
            ids++;
        } while (outer < state[0x219]);
    }
}
