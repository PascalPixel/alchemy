typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Table_080797fc {
    u8 padding[4];
    u8 values[4];
    u8 rest[16];
};

struct Record_080797fc {
    u8 padding[2];
    u8 values[148];
};

s32 Func_080773d8(s32);
struct Record_080797fc *Func_08078ed8(s32);
extern struct Table_080797fc Data_08088e38[];

s32 Func_080797fc(s32 record, u8 *source, s32 *output) {
    s32 i;
    s32 *cursor;

    if (record > 7) {
        u32 index;

        index = *(u8 *)(Func_080773d8(record) + 52);
        if (index > 43)
            index = 0;

        i = 0;
        cursor = output;
        for (; i <= 3; i++)
            *cursor++ = Data_08088e38[index].values[i] * 10;
    } else {
        cursor = output;
        source += 36;
        for (i = 3; i >= 0; i--) {
            u32 value = *source;
            source++;
            *cursor++ = value * 10;
        }

        if (record <= 7) {
            for (i = 0; i <= 3; i++) {
                *output += Func_08078ed8(record)->values[144 + i];
                output++;
            }
        }
    }

    return 0;
}
