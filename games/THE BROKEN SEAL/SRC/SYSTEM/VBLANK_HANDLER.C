#include "TYPES.H"
#include "DMA.H"

extern u16 Data_03001cb0;            /* link active */
extern u16 Data_03001f64;            /* link exchange status */
extern u8 Data_03001e44;             /* display registers pending */
extern u8 Data_03001d18;             /* OAM buffer pending */
extern u8 *Data_03001e50[];
extern u8 Data_03001ad0[];
extern void (*Data_03001cfc)(void);  /* one-shot VBlank hook */
extern volatile u32 Data_03001ae8;   /* keys held */
extern u32 Data_03001c94;            /* keys newly pressed */
extern u32 Data_03001af8;            /* presses since last read */
extern s32 Data_03001b00;            /* key repeat delay */
extern u32 Data_03001b04;            /* key repeat keys */
extern u32 Data_03001d0c;            /* keys held last frame */
extern s32 Data_03001800;            /* frame counter */
extern u16 Data_03001ccc;
extern u16 Data_03001d28;            /* VBlank seen */

u16 SerialRuntime_ExchangePayloads(void *send, void *receive);
void Func_0800655c(void);
void Func_080f9018(void);
void BlendTransition_Update(void);
void IoWriteQueue_FlushPending(void);
void Runtime_InvokeCallbacksByKey(s32 key);
void Func_080006fc(void);

/* VBlank interrupt handler: stops H-blank DMA 0, runs the link exchange,
   sound and blend updates, flushes the OAM and display register buffers,
   runs the one-shot hook and the frame callbacks, then samples the keys. */
void System_VBlankHandler(void)
{
    u32 keys;
    void (*hook)(void);

    {
        volatile u16 *dma0 = (volatile u16 *)0x040000b0;

        dma0[5] &= 0xc5ff;
        dma0[5] &= 0x7fff;
        dma0[5];
    }
    if (Data_03001cb0 != 0) {
        u16 *status = &Data_03001f64;
        *status = SerialRuntime_ExchangePayloads((void *)0x02002220, (void *)0x02002020);
        Func_0800655c();
    }
    Func_080f9018();
    BlendTransition_Update();
    if (Data_03001e44 != 0) {
        if (Data_03001d18 != 0)
            Dma_Set(Data_03001e50[52], (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
        Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
        IoWriteQueue_FlushPending();
        Data_03001e44 = 0;
    }
    hook = Data_03001cfc;
    if (hook != 0) {
        Data_03001cfc = 0;
        hook();
    }
    Runtime_InvokeCallbacksByKey(0x480);
    keys = *(volatile u16 *)0x04000130 ^ 0x3ff;
    {
        u32 pressed = keys & ~Data_03001ae8;

        Data_03001c94 = pressed;
        Data_03001af8 |= pressed;
    }
    Data_03001ae8 = keys;
    if (keys == 0) {
        Data_03001b00 = 19;
        Data_03001b04 = keys;
    } else if (Data_03001ae8 & (Data_03001d0c ^ 0xffff)) {
        Data_03001b00 = -1;
        Data_03001b04 = keys;
    } else if (Data_03001b00 > 0) {
        Data_03001b00--;
    }
    Data_03001d0c = keys;
    Data_03001800++;
    Data_03001ccc++;
    Data_03001d28 = 1;
    Func_080006fc();
}

