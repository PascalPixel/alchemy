/* Draft, not exact (2026-09-24): 85 differing halfwords, 208 of 204 bytes.
   Residual: the returned pointer and the VRAM address take r7/r6 where the
   reference uses r6/r7, and the palette address is copied to sl in each
   branch instead of once at the join (cross-jumped in the reference). */

#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern void *Data_03001e50[];
extern u8 Value_00000230[];

void Func_080f0254(s32 flag);
u8 *Func_08002f40(u32 value);
void *Func_080048b0(s32 slot, s32 size);
void Func_08002dd8(s32 slot);

static __inline__ void QueueWrite(u32 address, u32 value, u32 control)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;

    q = &gIoWriteQueue;
    ime = &Data_04000208;
    saved = *ime;
    *ime = (u16)ime;
    count = q->count;
    if (count <= 31) {
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
        *(u16 *)&q->count = count + 1;
        *destination++ = value;
        *destination++ = address;
        *destination = control;
    }
    *ime = saved;
}

void Func_080f02b0(u32 value, s32 flag)
{
    u8 *data;
    u32 palette;
    u32 fill;
    u32 vram;
    void *buffer;
    u32 size;

    if (value == 0) {
        Func_080f0254(flag);
        return;
    }
    data = Func_08002f40(value);
    if (flag == 0) {
        fill = 0;
        palette = 0x05000000;
        vram = 0x06000000;
    } else {
        fill = 0x80808080;
        palette = 0x05000100;
        vram = 0x06008000;
    }
    size = (u32)Value_00000230;
    buffer = Func_080048b0(49, size);
    Dma_Set((void *)0x080f0024, buffer, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((void (*)(u8 *, u32, u32))Data_03001e50[49])(data + 256, vram, fill);
    Func_08002dd8(49);
    QueueWrite(palette, (u32)data, 0x84000040);
}
