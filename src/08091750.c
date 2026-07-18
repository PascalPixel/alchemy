typedef unsigned char u8;
typedef signed int s32;

struct Fields_08091750 {
    u8 filler[0x1f4];
    s32 value;
};

extern struct Fields_08091750 Data_02000240;

void Func_08004278(s32);
void Func_080772f0(void);
void Func_0809335c(s32, s32);

void Func_08091750(void) {
    Func_08004278(0x080915ed);
    Func_0809335c(Data_02000240.value, 1);
    Func_080772f0();
}
