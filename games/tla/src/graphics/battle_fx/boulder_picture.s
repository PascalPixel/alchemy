	.section .rodata
	.global BattleFx_BoulderPicture
BattleFx_BoulderPicture:
	.incbin "games/tla/src/graphics/battle_fx/boulder_picture.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/boulder_picture.bitmap.lz"
	.byte 0x1d, 0x6f	@ PACKER: left in the buffer by BattleFx_SheepCardSheet+0xa1e
