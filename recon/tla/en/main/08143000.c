/*
 * Unmatched canonical draft: recon/tla/en/main/08143000.c.
 * BattlePresentation_ProcessPendingGraphicsTransfer (raw Func_08143000).
 * Measured 2026-10-02 with the ordinary TLA compiler plan, all six editions.
 * Native complete extent is 276 bytes, pools included. This source emits
 * 272 and differs by 124 bytes in every edition; no exact match is claimed.
 * The native callback has identical complete instructions/scalars/pools in
 * all six. All four direct colour-loader destinations and the IME, canvas,
 * copy and fill literal values are proved from their actual native bodies.
 * The four complete raw colour loaders (128 bytes) stay uncredited.
 *
 * The LOCAL DRAFT VIEW below gives only the four observed fields; it is
 * neither a second maintained header nor the complete shared work type.
 * It replaces the private full-prefix proposal without changing instructions,
 * relocations or offsets. Canonical publication needs no private include.
 *
 * Critical-order limitation: ordinary work fields let GCC read pending
 * before writing the interrupt mask and delay restoration past a work read.
 * Native claims/clears the pending flag inside the interrupt boundary and
 * restores IME before reading the transfer mode. It also discards a counter
 * read before assigning 1; this plain source omits that access. Registers,
 * argument scheduling, pool order and the final counter store differ.
 * No runnable equivalence is claimed. No assembly, fixed register, forced
 * unread storage, new work volatile or compiler option is retained.
 *
 * Eight ordinary source forms, EN complete extent/differing bytes:
 * u16 saved/direct I/O: 284/271
 * s32 saved/I/O pointer: 272/124 (retained)
 * u32 saved/I/O pointer: 272/124
 * s32 saved/direct I/O: 280/246
 * single final counter store: 272/125
 * idle branch returns first: 272/250
 * transfer mode captured before restore: 272/124
 * canvas loaded only for a transfer: 272/135
 * This self-contained conversion is a representation check, not a ninth form.
 *
 * The private proof initially resolved the four ColorBuffer_* loaders through
 * their complete raw instruction definitions at the actual native bytes.
 * All six editions now define those names at their raw/scaffold starts; the
 * localized definitions preserve the same128-byte scaffold and carry no credit.
 * Existing reviewed resident entries remain IwramCopyWords/IwramFillWords.
 */

#include "TYPES.H"
#include "IO_REG.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "LAYOUT_GUARD.H"

/* LOCAL DRAFT VIEW: the four fields observed in the canvas callback.
 * The intervening bytes belong to the existing heap work block; this view
 * does not allocate another buffer or claim the rest of the record's shape. */
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

/* Send a ready battle canvas to BG character block 1 and count the frames
 * since that transfer. The effect selects copy, clear, fade or colour shift.
 * Claim the ready flag while interrupts are masked; the pixel work happens
 * after the previous interrupt state is restored. */
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
