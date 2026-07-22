typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u16 Data_080b4100[];

void Func_080b06c0(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = Data_080b4100;
        do {
            struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
            record->values[0] = selector;
            record->values[4] = selector;
            record->values[8] = selector;
            record->values[12] = selector;
            record->values[16] = selector;
            record->values[20] = selector;
            count--;
        } while (count != 0);
    }
}
