//--------------------------------------------------------------------------
// wait_sync.c
// バトル：通信対戦の合図あわせ
//--------------------------------------------------------------------------

#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "btlwork.h"

// volatile: gLinkStatus carries its tag where tbs/include/global_cells.h declares it (measured on tbs-en)
extern volatile u16 gLinkStatus;
extern u16			gLinkPeerSignatures[][12];

//--------------------------------------------------------------------------
// 通信対戦なら、相手と "EXEC" の合図をそろえる
// 30 フレームのうちにそろえば 0、通信が切れたりそろわなければ -1
// 通信対戦でなければ 0
//--------------------------------------------------------------------------
s32 BattlePres_WaitSync(void)
{
#if GAME_TLA
	struct BtlWork *work = Ram_HeapSlots->battle_work;
#endif
	u16 *peer;
	u16 *sig;
	s32	 lost = 0;
	s32	 i;

	if (work->two_sided != 0)
	{
		u32 other = work->link_side ^ 1;

		peer = (u16 *)(gSerialPeerPayloads[other] + 4);
		sig = (u16 *)gSerialTransfer.payload;
		if (work->link_paused == 0)
		{
			sig[0] = 'E';
			sig[1] = 'X';
			sig[2] = 'E';
			sig[3] = 'C';

			for (i = 0; i <= 29; i++)
			{
				if ((gLinkStatus & 3) != 3) // 2 台の通信がとぎれた
				{
					lost++;
					if (lost > 24)
						return -1;
				}
				else
				{
					lost = 0;
					if (sig[2] == peer[2] && sig[3] == peer[3])
						return 0;
				}
				WaitFrames(1);
			}
		}
		return -1;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 通信対戦なら、行動のたびとラウンド終了処理のあとに相手と合図を順にそろえる
// "ex" "TU" → "rn" → "EXEC" → "tu" → "RN"
// 通信が切れているか、前にそろえた合図が相手と食いちがうか、
// 通信が 25 フレーム続けてとぎれたら -1。そろえば（通信対戦でなくても）0
//--------------------------------------------------------------------------
s32 BattlePres_SyncTurn(void)
{
#if GAME_TLA
	struct BtlWork *work = Ram_HeapSlots->battle_work;
#endif
	u16 *peer;
	u16 *sig;
	s32	 lost = 0;
	/* FAKEMATCH: unused local, the native frame's twenty bytes that no instruction touches. Without it differ 16 on tla-ja, tla-en and tla-de: no sub sp, #20 and its add back, the movs r7, #0 scheduled earlier. */
	u16 spare[10];

	if (work->two_sided != 0)
	{
		peer = gLinkPeerSignatures[work->link_side ^ 1];
		sig = (u16 *)gSerialTransfer.payload;
		if (work->link_paused != 0)
			return -1;

		// "ex" "TU"
		sig[0] = 'e';
		sig[1] = 'x';
		sig[4] = 'T';
		sig[5] = 'U';
		WaitFrames(1);
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[2] != peer[2] || sig[3] != peer[3])
					return -1;
				if (sig[0] == peer[0] && sig[1] == peer[1] && sig[4] == peer[4]
					&& sig[5] == peer[5])
					break;
			}
			WaitFrames(1);
		}

		// "rn"
		sig[6] = 'r';
		sig[7] = 'n';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[4] != peer[4] || sig[5] != peer[5])
					return -1;
				if (sig[6] == peer[6] && sig[7] == peer[7])
					break;
			}
			WaitFrames(1);
		}

		// "EXEC"
		sig[0] = 'E';
		sig[1] = 'X';
		sig[2] = 'E';
		sig[3] = 'C';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[6] != peer[6] || sig[7] != peer[7])
					return -1;
				if (sig[0] == peer[0] && sig[1] == peer[1] && sig[2] == peer[2]
					&& sig[3] == peer[3])
					break;
			}
			WaitFrames(1);
		}

		// "tu"
		sig[4] = 't';
		sig[5] = 'u';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[0] != peer[0] || sig[1] != peer[1] || sig[2] != peer[2]
					|| sig[3] != peer[3])
					return -1;
				if (sig[4] == peer[4] && sig[5] == peer[5])
					break;
			}
			WaitFrames(1);
		}

		// "RN"：相手が "rn" から変わるのを待つ
		sig[6] = 'R';
		sig[7] = 'N';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (peer[6] != 'r' || peer[7] != 'n')
					break;
			}
			WaitFrames(1);
		}
	}
	return 0;
}
