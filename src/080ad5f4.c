typedef unsigned char u8;
typedef signed int s32;

struct Fields_080ad5f4 {
    u8 filler[0x244];
    s32 values[1];
};

extern struct Fields_080ad5f4 *Data_03001f2c;

void Func_080ad5f4(s32 arg0, s32 arg1) {
    Data_03001f2c->values[arg0] = arg1;
}
