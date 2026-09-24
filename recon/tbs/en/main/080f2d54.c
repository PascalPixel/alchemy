/* Draft, not exact (2026-09-24): 33 differing halfwords, 356 of 356 bytes.
   Control flow, pools and stores match. Residual is allocation: the
   reference keeps the resource id and then the decode buffer in r6 (a
   copy of the 0x02010000 destination in r5) and both loop counters in r5;
   here the buffer takes r5, the counters r6 and the zero r7. Declaration
   orders, shared or separate counters and five buffer spellings did not
   move it; the zero in the offset loop is what brings it from 68 to 33. */
#include "DMA.H"
#include "SYSTEM.H"

struct BgOffset {
    u16 x;
    u16 y;
};

extern u8 Data_03001d18;
extern u8 Value_00000019;
extern struct BgOffset Data_03001ad0[4];
extern u32 Data_03001e40;
extern u32 Data_03001c94;

void Scheduler_ResetTaskTable(void);
void Blend_SetDarkenTarget16(s32 duration);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
void Blend_SetBrightenTarget0(s32 duration);
void Blend_WaitForTransition(void);
void *Resource_GetTableEntry(s32 id);
s32 Resource_DecodeType01(const void *source, void *destination);

s32 Func_080f2d54(void)
{
    s32 resource;
    u32 i;
    u8 *buffer;
    u32 frame;
    struct BgOffset *offset;
    struct BgOffset *walk;
    s32 zero;
    u8 *dst;

    Data_03001d18 = 1;
    resource = (s32)&Value_00000019;
    Scheduler_ResetTaskTable();
    Blend_SetDarkenTarget16(1);
    Bg0_ClearTilemap();
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = 0x685;
    *(volatile u16 *)0x04000000 = 0x1440;
    offset = Data_03001ad0;
    zero = 0;
    offset[2].y = zero;
    dst = (u8 *)0x02010000;
    Resource_DecodeType01(Resource_GetTableEntry(resource), dst);
    buffer = dst;
    Dma_Set(dst, (void *)0x05000000, 0x84000070, (volatile u32 *)0x040000d4);
    buffer += 0x1c0;
    Dma_Set(buffer, (void *)0x06003000, 0x84000200, (volatile u32 *)0x040000d4);
    buffer += 0x800;
    Dma_Set(buffer, (void *)0x06004000, 0x84001000, (volatile u32 *)0x040000d4);
    buffer += 0x4000;
    walk = offset;
    for (i = 0; i < 4; i++, walk++)
        walk->x = walk->y = zero;
    Dma_Set(Data_03001ad0, (void *)0x04000010, 0x84000004, (volatile u32 *)0x040000d4);
    Ui_LoadWindowGraphics();
    Bg0_ClearTilemap();
    Blend_SetBrightenTarget0(1);
    Blend_WaitForTransition();
    *(volatile u16 *)0x04000000 = 0x1540;
    for (i = 0; i < 120; i++) {
        Dma_Set(buffer + (((Data_03001e40 >> 3) & 3) << 10), (void *)0x06004100, 0x840000d0, (volatile u32 *)0x040000d4);
        if (Data_03001c94 & 9)
            break;
        WaitFrames(1);
    }
    return 0;
}
