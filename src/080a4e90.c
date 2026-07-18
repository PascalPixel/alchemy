typedef unsigned char u8;
typedef signed int s32;

struct Fields_080a4e90 {
    u8 filler[0x10c];
    void *object;
};

extern struct Fields_080a4e90 *Data_03001f2c;

void Func_080a23f4(void *, s32, s32, s32, s32);

void Func_080a4e90(void) {
    Func_080a23f4(Data_03001f2c->object, 13, 0, 17, 6);
}
