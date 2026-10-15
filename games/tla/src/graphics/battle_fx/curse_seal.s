	.section .rodata
	.global BattleFx_CurseSealSheet
BattleFx_CurseSealSheet:
	.incbin "games/tla/src/graphics/battle_fx/curse_seal.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/curse_seal.bitmap.lz"
	.byte 0xe1, 0xef	@ PACKER: left in the buffer by BattleFx_BoltSheet+0x1b5a
