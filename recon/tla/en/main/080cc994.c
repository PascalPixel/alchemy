/*
 * Draft: Stream_ReadU16, ahead of BattleMap_ApplySceneEntranceView in ⚓️'s
 * DEBUG/DEBUG2.C. Score 2: the listing advances the cursor in the high byte
 * load's delay. The rest is load scheduling: the native issues loads (and
 * fills their delay) earlier than agscc at -mcpu=arm7tdmi; with
 * -mtune=arm9tdmi added it scores 0.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RUNTIME_MEM.H"

s32 Func_080cc67c(s32);
s32 Func_080cc7c4(void);
void UiWork_Finalize(void *, s32);
extern const u8 Data_0805f5ec[];

u32 Stream_ReadU16(u8 **cursor)
{
    u32 low = *(*cursor)++;
    u32 high = *(*cursor)++;

    return low | (high << 8);
}
