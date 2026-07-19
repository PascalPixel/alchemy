typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    u8 bytes[0xB4];
} Data_080844ec_Record;

extern Data_080844ec_Record Data_080844ec[];

Data_080844ec_Record *Func_08078ed8(s32 index) {
    Data_080844ec_Record *base;

    base = Data_080844ec;
    return &base[index];
}
