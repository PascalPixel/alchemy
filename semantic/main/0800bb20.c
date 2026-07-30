typedef unsigned int u32;
typedef signed int s32;

void *Func_080048f4(s32, s32);
void *Func_080048b0(s32, s32);
void Func_08004838(void);
void Func_08003fa4(s32, s32, const void *);
extern u32 Value_0000007c;

void Func_0800bb20(s32 arg0)
{
    void *second;
    void *first;
    u32 *dma;
    s32 zero;
    s32 size;

    if (arg0 == 3) {
        first = Func_080048f4(4, 0xe00);
        second = Func_080048f4(3, 0x600);
    } else {
        first = Func_080048b0(4, 0xe00);
        second = Func_080048b0(3, 0x600);
    }

    Func_08004838();
    zero = 0;
    dma = (u32 *)0x040000d4;
    *dma++ = (u32)&zero;
    *dma++ = (u32)first;
    *dma++ = 0x85000380;
    dma -= 3;

    zero = 0;
    *dma++ = (u32)&zero;
    *dma++ = (u32)second;
    *dma++ = 0x85000180;
    dma -= 3;

    Func_08003fa4(0x5d, 0x80, (const void *)0x08012f20);
    size = (s32)&Value_0000007c;
    second = Func_080048b0(0x35, size);
    *dma++ = 0x0800a418;
    *dma++ = (u32)second;
    *dma++ = 0x84000000 | ((u32)size >> 2);
    dma -= 3;
}
