	.section .rodata
	.global BattleFx_CometSheetB
BattleFx_CometSheetB:
	.incbin "games/tbs/src/graphics/battle_fx/comet_b.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/comet_b.bitmap.lz"
	.byte 0xaf	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x2fb
