	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.arm
	.global Cartridge_Restart
Cartridge_Restart:
	b	Entry				@ 起動
	.space	156				@ 任天堂ロゴ (gbafix が書き込む)
	.ifdef TLA_EDITION_JA
	.ascii	"OUGONTAIYO_B"			@ ゲームタイトル
	.else
	.ascii	"GOLDEN_SUN_B"
	.endif
	.ascii	"AGF"				@ ゲームコード
	.ifdef TLA_EDITION_JA
	.ascii	"J"
	.endif
	.ifdef TLA_EDITION_EN
	.ascii	"E"
	.endif
	.ifdef TLA_EDITION_DE
	.ascii	"D"
	.endif
	.ifdef TLA_EDITION_ES
	.ascii	"S"
	.endif
	.ifdef TLA_EDITION_FR
	.ascii	"F"
	.endif
	.ifdef TLA_EDITION_IT
	.ascii	"I"
	.endif
	.ascii	"01"				@ メーカーコード
	.byte	0x96				@ 固定値
	.byte	0				@ 本体コード
	.byte	0				@ デバイスタイプ
	.space	7				@ 予約
	.byte	0				@ ソフトバージョン
	.byte	0				@ 補数チェック (gbafix が計算)
	.space	2				@ 予約
