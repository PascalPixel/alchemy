typedef unsigned char u8;
typedef unsigned int u32;

struct Value_0800c570 {
    u8 padding[29];
    u8 unknown_0 : 1;
    u8 field_1 : 1;
    u8 unknown_2 : 6;
};

void Func_0800c570(u8 *arg0, u32 arg1)
{
    if (arg0 != 0 && arg0[84] == 1) {
        struct Value_0800c570 *value = *(struct Value_0800c570 **)(arg0 + 80);
        value->field_1 = arg1;
    }
}
