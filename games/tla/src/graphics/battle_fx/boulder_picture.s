	.section .rodata
	.global BattleEffect_BoulderPicture
BattleEffect_BoulderPicture:
	.incbin "games/tla/src/graphics/battle_fx/boulder_picture.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/boulder_picture.bitmap.lz"
	.byte 0x1d, 0x6f	@ PACKER: left in the buffer by BattleEffect_SheepCardSheet+0xa1e
