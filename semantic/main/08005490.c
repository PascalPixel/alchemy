typedef unsigned int u32;
void Func_08007318(void *, void *, void *, void *);
extern char Value_00000080;
extern char Data_08001f38;

void Func_08005490(void *a, void *b, void *c, void *d)
{
    u32 words = (u32)&Value_00000080 >> 2;
    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_08001f38;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        Func_08007318(a, b, c, d);
    }
}
