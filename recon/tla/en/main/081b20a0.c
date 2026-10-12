/* Near miss, differ 4 of 79 (one block's argument order): The Broken Seal's
 * BattlePres_ProcessPendingTileTransfer on this game's heap slots. Before the fill call the
 * native copies the buffer (adds r0, r4, #0), loads the entry (ldr r3) and only then finishes
 * the size (lsls r1, r1, #8); here the size is finished first. sched2 has the three at equal
 * priority and takes them in RTL order, so the native's size constant came after the other
 * two. Measured: the value in a temporary and a void entry change nothing; the entry held in
 * a local is differ 22; three minutes of alchemy permute found nothing better. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"

struct BattleFxTransferWork
{
	u8	unknown_0000[0x7780];
	s32 transfer_mode;
	s32 transfer_value;
	u8	unknown_7788[0x78e4 - 0x7788];
	s32 frames_since_transfer;
	s32 transfer_pending;
};

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination, s32 size);

//--------------------------------------------------------------------------
// キャンバスの転送（スケジューラのコールバック）
// transfer_pending が立ったフレームだけ、256x128 の 8bpp キャンバスを
// VRAM 0x06003500 へ送る
//--------------------------------------------------------------------------
void BattlePres_ProcessPendingTileTransfer(void)
{
	struct BattleFxTransferWork *work;
	u8							*buf;

	/* FAKEMATCH: the one-pass block loads the effect work before the pending offset is started; assigned directly the offset is started first (differ 2). */
	do
	{
		work = (struct BattleFxTransferWork *)Ram_HeapSlots->battle_fx_work[0];
	} while (0);
	if (work->transfer_pending == 1)
	{
		buf = Ram_HeapSlots->battle_fx_work[1];
		switch (work->transfer_mode)
		{
		case 1: // 送ったあと transfer_value で塗りつぶす
			Dma_Set(buf, (void *)0x06003500, DMA_ENABLE32 | DMA_32BIT | 0x2000, REG_DMA3);
			Iwram_FillWords(buf, 0x8000, work->transfer_value);
			break;
		case 2: // 残像：0 以外の色番号を 1/2 または 3/4 に落とす
			if (work->transfer_value == 50)
				ColorBuffer_HalveNonzero(buf, (u8 *)0x06003500, 0x8000);
			else
				ColorBuffer_ScaleNonzeroThreeQuarters(buf, (u8 *)0x06003500, 0x8000);
			break;
		}
		work->transfer_pending = 0;
		work->frames_since_transfer = 1;
	}
	else
	{
		(work->frames_since_transfer)++;
	}
}
