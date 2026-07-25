typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

void Func_0800bdd4(void *);

void Func_0800c0f4(u8 *object)
{
    u32 zero;
    u32 *source;
    u32 *dma;
    u32 control;
    u32 kind;
    void **slot;
    void *child;
    s32 remaining;

    if (object != 0) {
        kind = *(u8 *)(object + 84) & 15;
        switch (kind) {
        case 1:
            Func_0800bdd4(*(void **)(object + 80));
            break;
        case 2:
            slot = *(void ***)(object + 80);
            remaining = 3;
            do {
                child = *slot++;
                if (child != 0) {
                    Func_0800bdd4(child);
                }
                remaining--;
            } while (remaining >= 0);
            break;
        }
        source = &zero;
        *source = 0;
        dma = (u32 *)0x040000d4;
        control = 0x8500001c;
        dma[0] = (u32)source;
        dma[1] = (u32)object;
        dma[2] = control;
    }
}
