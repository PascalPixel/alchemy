	.section .rodata
	.global BattleEffect_FireBlobSheet
BattleEffect_FireBlobSheet:
	.incbin "games/tla/src/graphics/battle_effects/fire_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleEffect_RockWallSheet+0xb87
