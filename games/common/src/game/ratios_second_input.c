//--------------------------------------------------------------------------
// ratios_second_input.c
// キャラクタ：EP の設定とゲージの計算
//--------------------------------------------------------------------------

#include "battle_unit.h"

//--------------------------------------------------------------------------
// EP を 0 から最大 EP の間で設定し、HP と EP のゲージを計算し直す
// ゲージは 0x4000 で満タン。残っていれば最低 1 にする
// unit : ユニット
// ep   : 新しい EP
//--------------------------------------------------------------------------
void Owner_UpdateSecondInputAndRatios(struct BtlUnit *unit, s32 ep)
{
	s32 cur;
	s32 rate;

	if (ep > unit->max_ep)
	{
		cur = unit->max_ep;
	}
	else
	{
		cur = 0;
		if (ep >= 0)
			cur = ep;
	}
	/* FAKEMATCH: the one-pass block keeps the EP store ahead of the HP
	 * gauge's loads. Measured in tla-ja and tla-en: the plain store lets
	 * the scheduler lift the hp load above it (differ 2). Its twin in
	 * games/tbs/src/game/chara.c records the other forms measured there. */
	do
	{
		unit->ep = cur;
	} while (0);

	// HP のゲージ
	rate = unit->hp;
	rate <<= 14;
	rate = rate / unit->max_hp;
	{
		s32 gauge = 0x4000;

		if (rate <= gauge)
		{
			gauge = 0;
			if (rate >= 0)
				gauge = rate;
		}
		unit->hp_gauge = gauge;
		if ((gauge << 16) == 0 && unit->hp != 0)
			unit->hp_gauge = 1;
	}

	// EP のゲージ
	{
		s32 num;
		s32 max;

		num = unit->ep;
		max = unit->max_ep;
		rate = (num << 14) / max;
	}
	{
		s32 gauge = 0x4000;

		if (rate <= gauge)
		{
			gauge = 0;
			if (rate >= 0)
				gauge = rate;
		}
		unit->ep_gauge = gauge;
		if ((gauge << 16) == 0 && unit->ep != 0)
			unit->ep_gauge = 1;
	}
}
