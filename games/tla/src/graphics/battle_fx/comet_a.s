	.section .rodata
	.global BattleFx_CometSheetA
BattleFx_CometSheetA:
	.incbin "games/tla/src/graphics/battle_fx/comet_a.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/comet_a.bitmap.lz"
	.byte 0x2a	@ PACKER: left in the buffer by BattleFx_ThornSheet+0x21b
