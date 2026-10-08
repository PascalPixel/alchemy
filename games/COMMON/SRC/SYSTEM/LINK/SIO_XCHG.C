//--------------------------------------------------------------------------
// SIO_XCHG.C
// 通信：毎フレームのデータの受け渡しと、送るパケットの組み立て
//--------------------------------------------------------------------------

#include "SERIAL_RUNTIME.H"
#include "DMA.H"
#include "IO_REG.H"
#include "EDITION.H"

//--------------------------------------------------------------------------
// phase 1 なら受けたデータを receive に集め、send から次のパケットを作る
// 戻り値は通信の状態
//   ビット 0〜7  : current_mask（mode 8 ならビット 7 も立てる）
//   ビット 8〜15 : received_mask
//   0x1000       : 親
//   0x2000       : SIOCNT のマルチプレイ ID が 2 以上
//--------------------------------------------------------------------------
s32 SerialRuntime_ExchangePayloads(void *send, void *receive)
{
	volatile u32 *siocnt;
	u32			  cnt;
	s32			  st;
	s32			  ret;

	siocnt = &REG_SIOCNT;
	cnt = *siocnt;
	if (gSerialRuntime.phase == 1)
	{
		SerialRuntime_CollectReceivedPayloads(receive);
		SerialRuntime_PrepareSendPacket(send);
		gSerialRuntime.sequence++;
	}
	st = gSerialRuntime.current_mask | (gSerialRuntime.received_mask << 8);
	if (gSerialRuntime.mode == 8)
		st |= 0x80;
	ret = SerialRuntime_AddParentFlag(st);
	if (((cnt << 26) >> 30) > 1) // ビット 4・5
		ret |= 0x2000;
	return ret;
}

#if !GAME_TLA
//--------------------------------------------------------------------------
// 送るパケットを send_buffer[0] に組み立てる
// [0] 番号、[1] まだ受けていない相手のマスク、[2] チェックサム、[4] データ 24 バイト
// チェックサムは先頭 28 バイトを 16 ビットずつ足して反転したもの
//--------------------------------------------------------------------------
void SerialRuntime_PrepareSendPacket(void *payload)
{
	volatile struct SerialRuntime *sr;
	u32							   i;
	u32							   sum;

	sr = SERIAL_RUNTIME;
	{
		u8 *pkt;
		u8	cur;
		u8	recv;

		pkt = (u8 *)sr->send_buffer[0];
		pkt[0] = sr->sequence;
		cur = sr->current_mask;
		recv = sr->received_mask;
		sum = 0;
		pkt[1] = recv ^ cur;
		*(u16 *)(pkt + 2) = sum;
		Dma_Set(payload, pkt + 4, DMA_ENABLE32 | DMA_32BIT | 6, REG_DMA3);
	}
	{
		u16 *pkt;
		u32	 v;

		pkt = sr->send_buffer[0];
		for (i = 0; i <= 13; i++)
		{
			v = *pkt;
			pkt++;
			sum += v;
		}
	}
	{
		u32					 inv;
		struct SerialPacket *pkt;

		inv = ~sum;
		pkt = (struct SerialPacket *)sr->send_buffer[0];
		pkt->checksum = inv;
	}

	// タイマー 3 を止め、送る位置を戻してから動かしなおす
	if (sr->mode != 0)
		REG_TM3CNT_H = 0;
	sr->send_index = -1;
	if (sr->mode != 0 && sr->transfer_enabled != 0)
		REG_TM3CNT_H = 0xc0; // 開始・割り込み
}
#else
//--------------------------------------------------------------------------
// 送るパケットを send_buffer[0] に組み立てる（作りは上と同じ）
//--------------------------------------------------------------------------
void SerialRuntime_PrepareSendPacket(void *payload)
{
	volatile struct SerialRuntime *sr;
	u32							   i;
	u32							   sum;

	sr = SERIAL_RUNTIME;
	{
		u8 *pkt;
		u8	cur;
		u8	recv;

		pkt = (u8 *)sr->send_buffer[0];
		pkt[0] = sr->sequence;
		cur = sr->current_mask;
		recv = sr->received_mask;
		sum = 0;
		/* FAKEMATCH: ⚓️ clears the checksum only after reading both masks */
		*(volatile u16 *)(pkt + 2) = sum;
		pkt[1] = recv ^ cur;
		Dma_Set(payload, pkt + 4, DMA_ENABLE32 | DMA_32BIT | 6, REG_DMA3);
	}
	{
		u16 *pkt;
		u32	 v;

		pkt = sr->send_buffer[0];
		for (i = 0; i <= 13; i++)
		{
			v = *pkt;
			pkt++;
			sum += v;
		}
	}
	{
		u32					 inv;
		struct SerialPacket *pkt;

		inv = ~sum;
		pkt = (struct SerialPacket *)sr->send_buffer[0];
		pkt->checksum = inv;
	}
	if (sr->mode != 0)
		REG_TM3CNT_H = 0;
	sr->send_index = -1;
	if (sr->mode != 0 && sr->transfer_enabled != 0)
		REG_TM3CNT_H = 0xc0;
}
#endif
