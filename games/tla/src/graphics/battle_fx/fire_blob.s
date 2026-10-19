	.section .rodata
	.global BattleFx_FireBlobSheet
BattleFx_FireBlobSheet:
	.incbin "games/tla/src/graphics/battle_fx/fire_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleFx_RockWallSheet+0xb87
