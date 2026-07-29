typedef unsigned int u32;
void Func_08007318(void *, void *, void *);
extern char Value_0000005c;
extern char Data_0800203c;

void Func_0800562c(void *a, void *b, void *c)
{
    u32 words = (u32)&Value_0000005c >> 2;
    {
        u32 buffer[words];
        u32 *dma = (u32 *)0x040000d4;
        u32 source = (u32)&Data_0800203c;
        u32 destination = (u32)buffer;
        u32 control = 0x84000000 | words;
        dma[0] = source;
        dma[1] = destination;
        dma[2] = control;
        Func_08007318(a, b, c);
    }
}
