typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct State_0808d5a4 {
    u8 padding[0x24a];
    s16 value;
};

extern struct State_0808d5a4 Data_02000240;
s32 Func_0808d48c(s32, s32);

s32 Func_0808d5a4(s32 arg0)
{
    s32 result = Func_0808d48c(0, arg0);
    s32 value = Data_02000240.value;

    if (value == arg0) {
        s32 next = Func_0808d48c(7, value);

        if (next != 0) {
            return next;
        }
    }
    return result;
}
