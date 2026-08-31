#include "types.h"

#define FIELD(p, type, off) (*(type *)((u8 *)(p) + (off)))
#define QUEUE_DMA(control) do { \
    u16 saved_ime = *(volatile u16 *)0x04000208; \
    *(volatile u16 *)0x04000208 = 0x0208; \
    if (*dma_count <= 31) { \
        u32 *entry = (u32 *)((u8 *)dma_count + *dma_count * 12 + 4); \
        (*dma_count)++; \
        entry[0] = (control); \
        entry[1] = 0x04000000; \
        entry[2] = 0x00020000; \
    } \
    *(volatile u16 *)0x04000208 = saved_ime; \
} while (0)

void Func_080cd508(void);
void Func_080030f8(s32);
void Func_080b5038(s32,u16,s32);
void Func_080041d8(void *,s32);
void Func_080b5028(s32,s32,s32,s32);
void Func_08004278(void *);

void Func_080cd594(s32 bg_control)
{
    void **cache=(void **)0x03001eec;
    void *work=cache[0];
    u8 *battle=*(u8 **)0x03001e74;
    void *canvas=cache[1];
    u8 *display=cache[5];
    u16 *dma_count=(u16 *)0x02002090;
    u16 *vram;
    s32 row,col;
    s32 tile_base;
    s32 palette_base;

    Func_080cd508();
    FIELD(display,s32,12)=1;
    Func_080030f8(1);
    *(volatile u16 *)0x04000050=0;
    QUEUE_DMA(0x7741);
    *(u16 *)0x03001ad6=32;
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
    *(volatile u16 *)0x04000042=0xf0;
    *(volatile u16 *)0x04000044=0x1088;
    *(volatile u16 *)0x04000046=0x1088;
    *(volatile u16 *)0x04000048=0x3537;
    *(volatile u16 *)0x0400004a=0x3f21;
    QUEUE_DMA(0x7741);

    vram=(u16 *)0x06003800;
    row=0;
    do {
        tile_base=row*0x1000+0x100;
        palette_base=row*16;
        col=0;
        do {
            *vram++=(u16)(tile_base|palette_base);
            tile_base+=0x200;
            palette_base+=2;
            col++;
        } while(col!=8);
        row++;
    } while(row!=16);

    ((void (*)(void *,s32))0x03000164)(canvas,0x4000);
    ((void (*)(void *,s32))0x03000164)((void *)0x06004000,0x4000);
    FIELD(work,s32,0x77a8)=0;
    FIELD(work,s32,0x77a0)=*(u16 *)0x03001ad4;
    FIELD(work,s32,0x77a4)=*(u16 *)0x03001ad6;
    Func_080030f8(1);
}
