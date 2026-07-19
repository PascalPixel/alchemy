typedef unsigned char u8;
typedef unsigned int u32;

struct State {
    u8 unknown[156];
    u32 context;
    u32 source;
};

extern struct State Data_03001e50;
u32 Func_080054e4(void *, void *, u32);

u32 Func_080c1438(void)
{
    void *source = (void *)Data_03001e50.source;
    u8 *context = (u8 *)Data_03001e50.context;

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13C0);

        if (*active != 0) {
            *active = 0;
            return Func_080054e4(source, (void *)0x06004000, 0x4000);
        }
    }
    return (u32)source;
}
