typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Globals {
    void *target;
    u8 unknown[116];
    u8 *base;
};

extern struct Globals Data_03001e74;
void Func_080b50f0(void *, void *, s32, s32);

void Func_080cd4b4(void)
{
    u8 *base = Data_03001e74.base;
    s32 *remaining = (s32 *)(base + 0x77B4);
    void *target = Data_03001e74.target;

    if (*remaining > 0) {
        s32 *counter = (s32 *)(base + 0x77B8);
        s32 value = ++*counter;

        Func_080b50f0(
            (u8 *)target + 0x544,
            (void *)0x050000C0,
            0x10000 - value * 1092,
            128
        );
        (*remaining)--;
    }
}
