typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s16 Data_02000240[];

union Word {
    s32 value;
};

void Func_080c24b0(void)
{
    u8 *base;
    union Word *words;
    s16 *slots;
    s32 index;

    base = *(u8 **)0x03001E74;
    words = (union Word *)(base + 0x530);
    Data_02000240[286] = 0;
    words[0].value = 0;
    words[1].value = 0;
    words[2].value = 0;
    slots = (s16 *)(base + 0x53C);
    for (index = 3; index >= 0; index--)
        slots[index] = 0;
}
