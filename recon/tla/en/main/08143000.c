
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "layout_guard.h"

struct CanvasTransferDraftWork {
    u8 unknown_0000[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x15c];
    s32 frames_since_transfer;
    s32 transfer_pending;
};

LAYOUT_OFFSET_GUARD(CanvasTransferDraftWork_Mode,
    struct CanvasTransferDraftWork, transfer_mode, 0x7780);
LAYOUT_OFFSET_GUARD(CanvasTransferDraftWork_Value,
    struct CanvasTransferDraftWork, transfer_value, 0x7784);
LAYOUT_OFFSET_GUARD(CanvasTransferDraftWork_Counter,
    struct CanvasTransferDraftWork, frames_since_transfer, 0x78e4);
LAYOUT_OFFSET_GUARD(CanvasTransferDraftWork_Pending,
    struct CanvasTransferDraftWork, transfer_pending, 0x78e8);

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination, s32 size);

void BattlePresentation_ProcessPendingGraphicsTransfer(void)
{
    struct CanvasTransferDraftWork *work;
    void *canvas;
    s32 saved;
    volatile u16 *ime = &REG_IME;

    work = (struct CanvasTransferDraftWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    saved = *ime;
    *ime = (u32)ime;
    if (work->transfer_pending == 1) {
        work->transfer_pending = 0;
        *ime = saved;
        switch (work->transfer_mode) {
        case 0:
            Iwram_CopyWords(BG_CHAR_BLOCK(1), canvas, 0x4000);
            break;
        case 1:
            Iwram_CopyWords(BG_CHAR_BLOCK(1), canvas, 0x4000);
            Iwram_FillWords(canvas, 0x4000, work->transfer_value);
            break;
        case 2:
            if (work->transfer_value == 50)
                ColorBuffer_Halve(canvas, BG_CHAR_BLOCK(1), 0x4000);
            else
                ColorBuffer_ScaleThreeQuarters(canvas, BG_CHAR_BLOCK(1), 0x4000);
            break;
        case 3:
            ColorBuffer_Darken(canvas, work->transfer_value, BG_CHAR_BLOCK(1), 0x4000);
            break;
        case 4:
            ColorBuffer_Brighten(canvas, work->transfer_value, BG_CHAR_BLOCK(1), 0x4000);
            break;
        }
        work->frames_since_transfer = 1;
    } else {
        *ime = saved;
        work->frames_since_transfer++;
    }
}
