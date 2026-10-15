	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.arm
	.global Rom_Start
Rom_Start:
	b	System_Entry			@ 起動
	.space	156				@ 任天堂ロゴ (gbafix が書き込む)
	.ifdef TBS_EDITION_JA
	.ascii	"OugonTaiyo_A"			@ ゲームタイトル
	.else
	.ifdef TBS_EDITION_EN
	.ascii	"Golden_Sun_A"
	.else
	.ascii	"GOLDEN_SUN_A"
	.endif
	.endif
	.ascii	"AGS"				@ ゲームコード
	.global gRom_LanguageCode
gRom_LanguageCode:
	.ifdef TBS_EDITION_JA
	.ascii	"J"
	.endif
	.ifdef TBS_EDITION_EN
	.ascii	"E"
	.endif
	.ifdef TBS_EDITION_DE
	.ascii	"D"
	.endif
	.ifdef TBS_EDITION_ES
	.ascii	"S"
	.endif
	.ifdef TBS_EDITION_FR
	.ascii	"F"
	.endif
	.ifdef TBS_EDITION_IT
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
