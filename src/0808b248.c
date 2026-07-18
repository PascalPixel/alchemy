typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;

struct Fields_0808b248 {
    u8 filler[0x1d6];
    s16 value;
};

extern struct Fields_0808b248 Data_02000240;

s16 Func_0808b248(void) {
    return Data_02000240.value;
}
