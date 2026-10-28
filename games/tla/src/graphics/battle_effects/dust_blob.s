	.section .rodata
	.global BattleEffect_DustBlobSheet
BattleEffect_DustBlobSheet:
	.incbin "games/tla/src/graphics/battle_effects/dust_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/dust_blob.bitmap.lz"
	.byte 0x54, 0x7b, 0x1f	@ PACKER: left in the buffer by BattleEffect_CloudSwirlSheet+0x329
