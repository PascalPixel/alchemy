#include "TYPES.H"
#include "DMA.H"

/* main:08077d38 GameState_InitDefaults - hand-written draft, 253 of 260
   halfwords differ, most of them from layout: the ROM is 92 bytes longer
   because it spills the DMA-wait zero to the stack, keeps the 0, 1, 4 and
   8 message symbols in sl, r5, r8 and r6 from right after
   Owner_InitRecords, and places two literal pools mid-function. Indexing
   typed views of the game state (not u8 casts) already gives the ROM's
   base register plus offset register stores. Still open: the DMA status
   read is folded to 0x040000dc instead of channel + 8, and the early
   symbol loads (probably locals initialised up front).

   Clears the game state, flag and party areas by DMA, then seeds the
   default options, the first party member, the build stamp and the
   Psynergy cost table. */

extern u16 Data_02000240[];
extern u8 Data_02001000[];
extern u8 Data_02000040[];
extern u8 Data_02000500[];
extern u8 Value_00000000, Value_00000001, Value_00000004, Value_00000008;
extern u8 Value_ffffffff;

#define GS8(off) (((u8 *)Data_02000240)[off])
#define GS16(off) (Data_02000240[(off) / 2])
#define GS32(off) (((u32 *)Data_02000240)[(off) / 4])
#define DMA3 ((volatile u32 *)0x040000d4)

void Owner_InitRecords(void);
void Party_AddActiveOwner(s32 owner);
u32 System_GetBuildStampTime(void);

void GameState_InitDefaults(void)
{
    volatile s32 zero;

    zero = 0;
    Dma_Set((void *)&zero, Data_02000240, 0x850000b0, DMA3);
    zero = 0;
    Dma_Set((void *)&zero, Data_02001000, 0x850003e1, DMA3);
    while (DMA3[2] & 0x80000000)
        ;
    zero = 0;
    Dma_Set((void *)&zero, Data_02000040, 0x85000080, DMA3);
    Data_02001000[0x104] = 0xff;
    zero = 0;
    Dma_Set((void *)&zero, Data_02000500, 0x85000298, DMA3);
    Owner_InitRecords();
    GS16(0x210) = 1;
    GS16(0x212) = 2;
    GS16(0x214) = 4;
    GS16(0x216) = 8;
    GS16(0x218) = 0x200;
    GS16(0x21a) = 0x100;
    GS16(0x21c) = 2;
    GS16(0x220) = 0;
    GS16(0x222) = 0;
    GS32(0x1f4) = 0;
    Party_AddActiveOwner(0);
    GS32(16) = 0;
    GS8(0x20c) = (u32)&Value_00000001;
    GS8(0x20a) = (u32)&Value_00000001;
    GS8(0x20b) = (u32)&Value_00000001;
    GS8(0x205) = (u32)&Value_00000000;
    GS8(0x206) = (u32)&Value_00000008;
    GS32(0) = 0;
    GS32(0x2b8) = System_GetBuildStampTime();
    *(u32 *)0x03001c9c = 0;
    *(volatile u8 *)0x03001d08 = (u32)&Value_00000000;
    GS32(4) = 0;
    GS8(0x22a) = *(volatile u8 *)0x03001d08;
    *(u16 *)0x03001d24 = 0;
    *(u16 *)0x02002004 = (u32)&Value_ffffffff;
    GS8(0x11d) = (u32)&Value_00000004;
    GS8(0x11e) = (u32)&Value_00000004;
    GS8(0x11f) = (u32)&Value_00000004;
    GS8(0x120) = (u32)&Value_00000008;
    GS8(0x121) = (u32)&Value_00000008;
    GS8(0x122) = (u32)&Value_00000008;
    GS8(0x123) = 16;
    GS8(0x124) = 16;
    GS8(0x125) = 16;
    GS8(0x126) = 32;
    GS8(0x127) = 32;
    GS8(0x128) = 32;
    GS8(0x129) = 64;
    GS8(0x12a) = 64;
    GS8(0x12b) = 64;
}
