typedef unsigned int u32;
void Func_08007318(void *, void *, void *, void *);
extern char Value_000000d4;
extern char Data_080021c4;

void Func_08005584(void *a, void *b, void *c, void *d)
{
    u32 words = (u32)&Value_000000d4 >> 2;
    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_080021c4;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        Func_08007318(a, b, c, d);
    }
}
