typedef unsigned char u8;
typedef signed int s32;

struct Fields_08091780 {
    u8 filler[0x1f4];
    s32 value;
};

extern struct Fields_08091780 Data_02000240;

void Func_080030f8(s32);
void Func_0808b674(s32);
void Func_0808ba1c(s32);
void Func_0809177c(void);

void Func_08091780(s32 arg0) {
    Func_0809177c();
    Func_0808b674(arg0);
    Func_080030f8(1);
    Func_0808ba1c(Data_02000240.value);
}
