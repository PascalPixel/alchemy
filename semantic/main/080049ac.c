typedef signed int s32;
typedef unsigned int u32;

struct Descriptor_080049ac {
    u32 value;
    u32 zero_04;
    u32 zero_08;
    u32 zero_0c;
};

extern void *Func_080048f4(s32 kind, s32 size);

void Func_080049ac(void)
{
    struct Descriptor_080049ac *descriptors =
        (struct Descriptor_080049ac *)0x03000350;
    void *buffer = Func_080048f4(2, 48);

    *(s32 *)0x03001cc4 = 0;
    *(void **)0x03001d2c = buffer;

    descriptors[0].value = 0x10000;
    descriptors[0].zero_04 = 0;
    descriptors[0].zero_08 = 0;
    descriptors[0].zero_0c = 0;
    descriptors[1].value = 0x10000;
    descriptors[1].zero_04 = 0;
    descriptors[1].zero_08 = 0;
    descriptors[1].zero_0c = 0;
    descriptors[2].value = 0x10000;
    descriptors[2].zero_04 = 0;
    descriptors[2].zero_08 = 0;
    descriptors[2].zero_0c = 0;
}
