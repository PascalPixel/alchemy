	.section .rodata
	.global BattleFx_CloudSwirlSheet
BattleFx_CloudSwirlSheet:
	.incbin "games/tla/src/graphics/battle_fx/cloud_swirl.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/cloud_swirl.bitmap.lz"
	.byte 0xee	@ PACKER: left in the buffer by BattleFx_PetalSheet+0x157b
