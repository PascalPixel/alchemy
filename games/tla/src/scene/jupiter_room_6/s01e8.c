#include "jupiter_room_6.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern s32 gData_020023c0;
extern u16 gData_020023e8[];

s32 __modsi3();
s32 Map_GetTerrainHeight();
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

s32 Func_020001e8(void)
{
    struct JupiterRoom6EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x204;
    if (JupiterRoom6_FlagCheck(0xa19)) {
        Func_02001b6c(12, 0x480000, 0x2580000);
        Func_020000b4(0);
    }
    if (GameFlag_Test(0x200))
        Func_020000b4(0);
    if ((u16)(gPartyState.entrance - 5) <= 1) {
        Func_02001250(33, 0x280, 0x380, 20, 31);
    } else {
        Func_02001250(43, 0x280, 0x380, 21, 31);
        Func_0200196c(21, -3, 0x204);
    }
    return 0;
}
