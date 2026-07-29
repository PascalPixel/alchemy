#include "types.h"

struct DmaTransfer_08002e00 {
    const void *source;
    void *destination;
    u32 control;
};

typedef void (*InterruptHandler_08002e00)(void);

#define DMA_FLAGS_08002E00(dma) (*(volatile u16 *)((u8 *)(dma) + 10))

extern volatile u16 Data_04000204;
extern volatile u16 Data_04000132;
extern u32 Data_02002090;
extern u8 Data_03001ac4;
extern u8 Data_03001f54;
extern u8 Data_03001f58;
extern u32 Data_03007800;
extern u8 Data_03001d18;
extern u8 Data_03001ca0;

void Func_08004858(void);
void Func_0800300c(void);
void Func_08002f10(void);
void Func_0800479c(void);
void Func_08004760(void);
void Func_0800307c(s32 first, s32 second, InterruptHandler_08002e00 callback);
void Func_08003650(void);
void Func_080f9008(void);
void Func_0800403c(void);
void Func_080040e8(void);
void Func_080030f8(s32 value);
void Func_0808a008(s32 value);

void Func_08002e00(void)
{
    struct DmaTransfer_08002e00 *dma0;
    u32 *source;
    u32 zero;
    u32 value;

    dma0 = (struct DmaTransfer_08002e00 *)0x040000b0;
    DMA_FLAGS_08002E00(dma0) =
        (u16)(0xc5ff & DMA_FLAGS_08002E00(dma0));
    DMA_FLAGS_08002E00(dma0) =
        (u16)(0x7fff & DMA_FLAGS_08002E00(dma0));
    (void)DMA_FLAGS_08002E00(dma0);

    value = 0x4014;
    Data_04000204 = value;
    source = &zero;
    *source = 0;
    {
        struct DmaTransfer_08002e00 *dma3;
        void *destination;
        u32 control;

        destination = (void *)0x03000000;
        control = 0x85001e00;
        dma3 = (struct DmaTransfer_08002e00 *)0x040000d4;
        dma3->source = source;
        dma3->destination = destination;
        dma3->control = control;
    }

    Func_08004858();
    Func_0800300c();
    Data_02002090 = 0;
    Data_03001ac4 = 0;
    Data_03001f54 = 0;
    Data_03001f58 = 0;
    Func_08002f10();
    Func_0800479c();
    Func_08004760();
    *(volatile u16 *)0x04000000 = 0x140;
    Func_0800307c(0, 1, Func_08003650);
    value = 0xc00f;
    Data_04000132 = value;
    Func_080f9008();
    Func_0800403c();
    Func_080040e8();
    Data_03007800 = 0;
    Data_03001d18 = 1;
    Data_03001ca0 = 0;
    Func_080030f8(10);
    Func_0808a008(0);
}
