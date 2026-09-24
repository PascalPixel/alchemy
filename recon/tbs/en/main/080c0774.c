/* Draft, not exact (2026-09-24): 308 of 308 bytes, 12 differing halfwords
   (was 178 at 360). BattlePresentation_ConfigurePaletteFade: the queued BG
   control write is the IO write queue idiom of SYSTEM/IO_WRITE_QUEUE.C
   (FAKEMATCH one-pass loops, u16 count store), and the green and blue masks
   are a u16 held from the 0x1f link symbol, a halfword pool constant whose
   60-byte reach places the literal pool inside the fade loop as the
   reference does. Residual: register choice in the fade loop (the reference
   has the mask in r7, the index in r6, the source offset in r0 and the
   destination in r4). */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

#define BattlePres_ConfigurePaletteFade Func_080c0774

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208
extern u8 Value_0000001f;
#define MASK mask

s32 Func_080041d8(u32 callback, s32 interval);
void Func_080c0098(void *);
void Func_080c00d8(void *);

void BattlePres_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    s32 *transition = *(s32 **)0x03001f00;

    if (transition[2] == 0) {
        Func_080041d8(0x080c0131, 0x4ff);
    }
    transition[2] = mode;

    if (mode == 1) {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        do {
            do {
                ime = &REG_IME;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            count = q->count;
            if (count <= 31) {
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
                *(u16 *)&q->count = count + 1;
                *destination++ = 0x1f83;
                *destination++ = 0x0400000a;
                *destination = 0x20000;
            }
            *ime = saved;
        } while (0);
    }

    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (volatile u32 *)0x040000d4);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        Dma_Set(*(u8 **)0x03001e74 + 0x544, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else if (fade != 0) {
        u16 *source = (u16 *)(*(u8 **)0x03001e74 + 0x544);
        u16 *destination = (u16 *)0x050000c0;
        s32 i;
        u16 mask = (u16)(s32)&Value_0000001f;

        for (i = 0; i != 128; i++) {
            s32 red = source[i] & 31;
            s32 green = (source[i] >> 5) & MASK;
            s32 blue = (source[i] >> 10) & MASK;

            if (red > fade) {
                red -= fade;
            } else {
                red = 0;
            }
            if (green > fade) {
                green -= fade;
            } else {
                green = 0;
            }
            if (blue > fade) {
                blue -= fade;
            } else {
                blue = 0;
            }
            destination[i] = (blue << 10) | (green << 5) | red;
        }
    }

    Func_080c0098((void *)0x06003800);
    Func_080c00d8((void *)0x0600f800);
}
