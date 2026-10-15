/* Near miss, differ 2 of 68 (one reordered row): The Broken Seal's BattleFx_StartWindowHBlankDma on
 * this game's heap slot. The native reads DMA0's control (ldrh r1, [r3, #10]) before it starts
 * the first mask (movs r2, #197); here the mask is started first: sched2 gives the three-insn
 * constant priority 62 and the load 61 (alchemy diff --rtl). Measured: a one-pass block around
 * the channel address fixes that pair but keeps REG_DISPCNT's movs r4, #128 from rising to
 * the idle slot after the page load, where the native has it (differ 6), so the native has no
 * barrier there; a temporary for the control makes the mask a halfword pool load (differ 10);
 * ~0x3a00 for the mask changes nothing. The four one-pass blocks below are needed as they
 * are: empty asm barriers in their place each move the literal pool two instructions early. */
#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

//--------------------------------------------------------------------------
// ウィンドウ 0/1 を有効にし、ページの値を WININ, WINOUT, WIN0H, WIN1H に入れてから
// 残りを HBlank ごとに DMA0 で WIN0H と WIN1H へ 1 ワードずつ送る
//--------------------------------------------------------------------------
void BattleFx_StartWindowHBlankDma(void)
{
	struct DisplayTransitionState *work;
	u16							  *src;
	volatile u16				  *channel;
	s32							   in;
	s32							   out;
	s32							   value;
	s32							   other;

	/* FAKEMATCH: the one-pass block loads the transition work before the page offset is started (differ 2 without it). */
	do
	{
		work = Ram_HeapSlots->display_transition;
	} while (0);
	src = work->pages[work->page];
	channel = REG_DMA0;
	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
	/* FAKEMATCH: the one-pass block keeps the value load before the register address is started (differ 2 without it). */
	do
	{
		in = *src;
	} while (0);
	REG_WININ = in;
	src++;
	out = *src;
	REG_WINOUT = out;
	src++;
	/* FAKEMATCH: the one-pass block keeps the value load before the register address is started (differ 2 without it). */
	do
	{
		value = *src;
	} while (0);
	REG_WIN0H = value;
	src++;
	/* FAKEMATCH: the one-pass block keeps the value load before the register address is started (differ 2 without it). */
	do
	{
		other = *src;
	} while (0);
	REG_WIN1H = other;
	src++;
	other = 160;
	REG_WIN0V = other;
	REG_WIN1V = other;
	Dma_Set(src, (void *)&REG_WIN0H, 0xa6600001, (volatile u32 *)channel);
}
