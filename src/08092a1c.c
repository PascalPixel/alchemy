typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Object_08092a1c {
    u8 padding_00[0x30];
    s32 field_30;
    s32 field_34;
    u8 padding_38[0x21];
    u8 field_59;
    u8 padding_5a[0x0a];
    s16 field_64;
    u8 padding_66[2];
    struct Object_08092a1c *field_68;
};

struct Object_08092a1c *Func_0808ba1c(u32);
void Func_08009098(struct Object_08092a1c *, s32);

void Func_08092a1c(u32 arg0, s32 arg1, s32 arg2) {
    struct Object_08092a1c *first = Func_0808ba1c(arg0);
    struct Object_08092a1c *second = Func_0808ba1c(arg1 & 0xff);

    if (first != 0 && second != 0) {
        first->field_68 = second;
        if (!(arg1 & 0x10000)) {
            first->field_64 = 40;
            first->field_34 = second->field_34 * 2;
            first->field_30 = second->field_30;
            first->field_59 = 0;
        }
        Func_08009098(first, arg2);
    }
}
