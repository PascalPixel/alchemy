typedef unsigned char u8;
typedef unsigned int u32;

struct Target_080b8144 {
    u8 filler0[0x30];
    u32 value30;
    u32 value34;
    u8 filler38[0x0C];
    u32 value44;
    u32 value48;
    u8 filler4c[0x0E];
    u8 value5a;
};

struct Wrapper_080b8144 {
    struct Target_080b8144 *target;
    u8 filler4[8];
    u32 first;
    u32 fourth;
};

void Func_08009150(struct Target_080b8144 *, u32, u32, u32);

void Func_080b8144(struct Wrapper_080b8144 *wrapper)
{
    struct Target_080b8144 *target = wrapper->target;

    target->value34 = 0x20000;
    target->value30 = 0x80000;
    target->value48 = 0xAB85;
    target->value44 = 0;
    target->value5a = 0;
    Func_08009150(target, wrapper->first, 0, wrapper->fourth);
}
