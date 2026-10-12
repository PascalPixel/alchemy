#include "types.h"
#include "ram_buffer.h"

void BattleFx_ApplyColorToTargetBuffer(s32 colors, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void GameFlag_ClearBit(s32 flag);

void Field_BeginPaletteTransition(s32 frames)
{
    u8 *state = Ram_HeapSlots->event_work;
    s32 colors = (s32)(state + 0x222);

    BattleFx_ApplyColorToTargetBuffer(colors, 2);
    if (*(s8 *)(state + 0xca4) != 0)
        BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
    else
        BattleFx_ApplyColorToTargetBuffer(colors, 1);
    GameFlag_ClearBit(0x108);
    BattleFx_StartBufferInterpolation(frames);
}
