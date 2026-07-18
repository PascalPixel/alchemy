typedef unsigned char u8;
typedef unsigned int u32;

struct Value_0800c548 {
    u8 padding[5];
    u8 unknown_0 : 2;
    u8 field_2 : 2;
    u8 unknown_4 : 4;
};

void Func_0800c548(u8 *arg0, u32 arg1)
{
    if (arg0 != 0 && arg0[84] == 1) {
        struct Value_0800c548 *value = *(struct Value_0800c548 **)(arg0 + 80);
        value->field_2 = arg1;
    }
}
