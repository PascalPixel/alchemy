	.section .rodata
	.global BattleEffect_CloudSwirlSheet
BattleEffect_CloudSwirlSheet:
	.incbin "games/tla/src/graphics/battle_fx/cloud_swirl.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/cloud_swirl.bitmap.lz"
	.byte 0xee	@ PACKER: left in the buffer by BattleEffect_PetalSheet+0x157b
