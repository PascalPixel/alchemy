	.section .rodata
	.global BattleAnim_FireBlobSheet
BattleAnim_FireBlobSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_blob.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_blob.bitmap.lz"
	.byte 0x1a	@ PACKER: left in the buffer by BattleAnim_RockWallSheet+0xb87
