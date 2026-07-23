typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_080f95e0(s32, s32);
extern u8 Data_080fb830[];
extern u32 Data_080fb8e4[];

s32 Func_080fa1fc(void *arg0, u8 arg1, u8 arg2)
{
    u32 index;
    u32 scale;
    u32 first;
    u32 second;

    index = arg1;
    scale = arg2 << 24;
    if (index > 178) {
        index = 178;
        scale = 255 << 24;
    }

    first = Data_080fb830[index];
    first = Data_080fb8e4[first & 15] >> (first >> 4);
    second = Data_080fb830[index + 1];
    second = Data_080fb8e4[second & 15] >> (second >> 4);

    return Func_080f95e0(*(s32 *)((u8 *)arg0 + 4),
        first + Func_080f95e0(second - first, scale));
}

void Func_080fa260(void)
{
}
