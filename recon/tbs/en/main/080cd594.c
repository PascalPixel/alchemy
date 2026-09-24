/* Draft, not exact (2026-09-24): 716 of 728 bytes, 158 differing halfwords
   (was 360 at 756). BattleFx_BeginCanvasLayer: the four queued display-control
   writes are QueueIoWriteDelay2 written out inline with the function-level
   queue and IME pointers the reference keeps in r6 and r5, the saved IME word
   copied inside a one-pass loop (FAKEMATCH, as in SYSTEM/IO_WRITE_QUEUE.C);
   the work pointers come from gWorkSlot (the battle pointer is derived from
   the cache address), the scroll pair from gBgScroll, and the window
   registers are written 0x40, 0x44, 0x42, 0x46 as BattlePres_Configure-
   EffectDisplay does. Everything up to 0x160 matches. Residual: the order of
   the display-register pool loads, a literal pool the reference places
   before the tile-map loop (12 bytes), and that loop's registers. */
#include "TYPES.H"

#define FIELD(p, type, off) (*(type *)((u8 *)(p) + (off)))
#include "IO_WRITE_QUEUE.H"
extern volatile u16 Data_04000208;
#define QUEUE_DMA(control) do {                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (control);                                     \
            *destination++ = 0x04000000;                                    \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
} while (0)

extern u8 gWorkSlot[];
struct Cells03001ad0 { u16 unk00; u16 unk02; u16 unk04; u16 unk06; };
extern struct Cells03001ad0 gBgScroll;
void Func_080cd508(void);
void Func_080030f8(s32);
void Func_080b5038(s32,u16,s32);
s32 Func_080041d8(void *,s32);
void Func_080b5028(s32,s32,s32,s32);
void Func_08004278(void *);

void Func_080cd594(s32 bg_control)
{
    void **cache=(void **)(gWorkSlot + 39 * 4);
    void *work=cache[0];
    u8 *battle=*(u8 **)(gWorkSlot + 9 * 4);
    void *canvas=cache[1];
    u8 *display=cache[5];
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u16 *vram;
    s32 row,col;
    s32 tile_base;
    s32 palette_base;

    Func_080cd508();
    FIELD(display,s32,12)=1;
    Func_080030f8(1);
    *(volatile u16 *)0x04000050=0;
    q = &gIoWriteQueue;
    ime = &Data_04000208;
    QUEUE_DMA(0x7741);
    gBgScroll.unk06=32;
    Func_080030f8(1);
    Func_080b5038(1,FIELD(battle,u16,0x648),0);
    FIELD(work,s32,0x77b4)=0;
    FIELD(work,s32,0x77b8)=0;
    Func_080041d8((void *)0x080cd4b5,0xc80);
    QUEUE_DMA(0x7341);
    Func_080030f8(1);
    *(volatile u16 *)0x0400000c=(u16)(bg_control|0x784);
    QUEUE_DMA(0x7341);
    Func_080b5028(0,0,0,100);
    FIELD(display,s32,12)=0;
    Func_080030f8(1);

    *(volatile u16 *)0x04000050=0x3f44;
    *(volatile u16 *)0x04000052=0x100e;
    *(volatile u32 *)0x04000028=0;
    *(volatile s32 *)0x0400002c=-0x1000;
    *(volatile u16 *)0x04000020=0x80;
    *(volatile u16 *)0x04000022=0;
    *(volatile u16 *)0x04000024=0;
    *(volatile u16 *)0x04000026=0x100;
    *(volatile u16 *)0x04000040=0xf0;
    *(volatile u16 *)0x04000044=0x1088;
    *(volatile u16 *)0x04000042=0xf0;
    *(volatile u16 *)0x04000046=0x1088;
    *(volatile u16 *)0x04000048=0x3537;
    *(volatile u16 *)0x0400004a=0x3f21;
    QUEUE_DMA(0x7741);

    vram = (u16 *)0x06003800;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++) {
            vram[row * 8 + col] = (s16)((row * 0x1000 + col * 0x200 + 0x100) | (row * 16 + col * 2));
        }
    }

    ((void (*)(void *,s32))0x03000164)(canvas,0x4000);
    ((void (*)(void *,s32))0x03000164)((void *)0x06004000,0x4000);
    FIELD(work,s32,0x77a8)=0;
    FIELD(work,s32,0x77a0)=gBgScroll.unk04;
    FIELD(work,s32,0x77a4)=gBgScroll.unk06;
    Func_080030f8(1);
}
