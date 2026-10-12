#ifndef ALCHEMY_UNIT_SYSTEM_SCHEDULER_RUNTIME8_H
#define ALCHEMY_UNIT_SYSTEM_SCHEDULER_RUNTIME8_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

s32 Runtime_GetLowTableAddress(void);
void Text_FormatHexToWork(u32 value);
void Text_FormatSignedDecimalToWork(s32 arg0);
void Graphics_FillWordStreamWithF000(u32 count);
void *memcpy(void *destination, const void *source, unsigned long size);
void Runtime_WriteDebugTextTiles(const u8 *src);
void Text_DrawHexRightAligned(u32 value, s32 keta);
void Text_DrawSignedDecimalRightAligned(s32 value, s32 keta);
void Bg0_ClearTilemap(void);
void Ui_LoadWindowGraphics(void);
void PaletteDma_LoadBlock(void);

#endif
